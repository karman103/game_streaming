package main

import (
	"context"
	"encoding/json"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"time"

	"github.com/docker/docker/api/types/container"
	"github.com/docker/docker/client"
	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/websocket/v2"
)

type StreamInfo struct {
	SessionID  string `json:"sessionId"`
	StreamURL  string `json:"streamUrl"`
	Status     string `json:"status"`
	Quality    string `json:"quality"`
	Resolution string `json:"resolution"`
	FPS        int    `json:"fps"`
	Bitrate    int    `json:"bitrate"`
}

// StreamHandler handles video streaming from Docker containers
func StreamHandler(c *websocket.Conn) {
	sessionID := c.Params("id")
	log.Printf("Client connected for streaming: %s", sessionID)

	container, err := GetContainer(sessionID)
	if err != nil {
		log.Printf("Container not found for session %s: %v", sessionID, err)
		c.WriteMessage(websocket.TextMessage, []byte(`{"error": "Container not found"}`))
		return
	}

	defer c.Close()

	// Send stream info
	streamInfo := StreamInfo{
		SessionID:  sessionID,
		StreamURL:  fmt.Sprintf("/api/stream/%s/playlist.m3u8", sessionID),
		Status:     "active",
		Quality:    "720p",
		Resolution: "1280x720",
		FPS:        30,
		Bitrate:    2000,
	}

	infoJSON, _ := json.Marshal(streamInfo)
	c.WriteMessage(websocket.TextMessage, infoJSON)

	// Start streaming loop
	streamLoop(c, container)
}

func streamLoop(c *websocket.Conn, container *GameContainer) {
	ticker := time.NewTicker(100 * time.Millisecond) // 10 FPS updates
	defer ticker.Stop()

	for {
		select {
		case <-ticker.C:
			// Send frame data or stream status
			frameData := getFrameData(container)
			if frameData != nil {
				c.WriteMessage(websocket.BinaryMessage, frameData)
			}
		}
	}
}

func getFrameData(container *GameContainer) []byte {
	// In a real implementation, this would capture actual video frames
	// For now, we'll return a placeholder
	return []byte("frame_data_placeholder")
}

// HTTP handlers for streaming
func SetupStreamingRoutes(app *fiber.App) {
	// Serve HLS playlist
	app.Get("/api/stream/:id/playlist.m3u8", func(c fiber.Ctx) error {
		sessionID := c.Params("id")
		return serveHLSPlaylist(c, sessionID)
	})

	// Serve HLS segments
	app.Get("/api/stream/:id/:segment", func(c fiber.Ctx) error {
		sessionID := c.Params("id")
		segment := c.Params("segment")
		return serveHLSSegment(c, sessionID, segment)
	})

	// Get stream info
	app.Get("/api/stream/:id/info", func(c fiber.Ctx) error {
		sessionID := c.Params("id")
		return getStreamInfo(c, sessionID)
	})
}

func serveHLSPlaylist(c fiber.Ctx, sessionID string) error {
	if _, err := GetContainer(sessionID); err != nil {
		return c.Status(404).JSON(fiber.Map{"error": "Container not found"})
	}
	streamDir := filepath.Join("/tmp", "streams", sessionID)
	playlistPath := filepath.Join(streamDir, "playlist.m3u8")
	if _, err := os.Stat(playlistPath); os.IsNotExist(err) {
		playlist := createBasicPlaylist()
		os.WriteFile(playlistPath, []byte(playlist), 0644)
	}
	c.Set("Content-Type", "application/vnd.apple.mpegurl")
	return c.SendFile(playlistPath)
}

func serveHLSSegment(c fiber.Ctx, sessionID, segment string) error {
	if _, err := GetContainer(sessionID); err != nil {
		return c.Status(404).JSON(fiber.Map{"error": "Container not found"})
	}
	streamDir := filepath.Join("/tmp", "streams", sessionID)
	segmentPath := filepath.Join(streamDir, segment)
	if _, err := os.Stat(segmentPath); os.IsNotExist(err) {
		createPlaceholderSegment(segmentPath)
	}
	c.Set("Content-Type", "video/mp2t")
	return c.SendFile(segmentPath)
}

func getStreamInfo(c fiber.Ctx, sessionID string) error {
	container, err := GetContainer(sessionID)
	if err != nil {
		return c.Status(404).JSON(fiber.Map{"error": "Container not found"})
	}

	streamInfo := StreamInfo{
		SessionID:  sessionID,
		StreamURL:  fmt.Sprintf("/api/stream/%s/playlist.m3u8", sessionID),
		Status:     container.Status,
		Quality:    "720p",
		Resolution: "1280x720",
		FPS:        30,
		Bitrate:    2000,
	}

	return c.JSON(streamInfo)
}

func createBasicPlaylist() string {
	return `#EXTM3U
#EXT-X-VERSION:3
#EXT-X-TARGETDURATION:10
#EXT-X-MEDIA-SEQUENCE:0
#EXTINF:10.0,
segment0.ts
#EXT-X-ENDLIST`
}

func createPlaceholderSegment(path string) {
	// Create a minimal MPEG-TS segment
	// In production, this would be actual video data
	placeholder := []byte("placeholder_video_segment")
	os.WriteFile(path, placeholder, 0644)
}

// Advanced streaming functions for production use
func startFFmpegStreaming(container *GameContainer) error {
	streamDir := filepath.Join("/tmp", "streams", container.SessionID)
	os.MkdirAll(streamDir, 0755)

	// FFmpeg command to capture container display and create HLS stream
	cmd := []string{
		"ffmpeg",
		"-f", "x11grab",
		"-i", ":0.0",
		"-c:v", "libx264",
		"-preset", "ultrafast",
		"-tune", "zerolatency",
		"-f", "hls",
		"-hls_time", "2",
		"-hls_list_size", "3",
		"-hls_flags", "delete_segments",
		filepath.Join(streamDir, "playlist.m3u8"),
	}

	log.Printf("Starting FFmpeg streaming for session %s", container.SessionID)
	// In production, you would execute this command
	return nil
}

func captureContainerScreen(containerInfo *GameContainer) ([]byte, error) {
	cli, err := client.NewClientWithOpts(client.FromEnv)
	if err != nil {
		return nil, err
	}

	// Execute screenshot command in container
	execResp, err := cli.ContainerExecCreate(context.Background(), containerInfo.ID, container.ExecOptions{
		Cmd:          []string{"import", "-window", "root", "/tmp/screenshot.png"},
		AttachStdout: true,
		AttachStderr: true,
	})
	if err != nil {
		return nil, err
	}

	if err := cli.ContainerExecStart(context.Background(), execResp.ID, container.ExecStartOptions{}); err != nil {
		return nil, err
	}

	// Copy screenshot from container
	reader, _, err := cli.CopyFromContainer(context.Background(), containerInfo.ID, "/tmp/screenshot.png")
	if err != nil {
		return nil, err
	}
	defer reader.Close()

	return io.ReadAll(reader)
}
