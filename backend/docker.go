package main

import (
	"context"
	"fmt"
	"io"
	"log"
	"os"
	"path/filepath"
	"time"

	"github.com/docker/docker/api/types/container"
	"github.com/docker/docker/api/types/image"
	"github.com/docker/docker/client"
	"github.com/docker/go-connections/nat"
)

type GameContainer struct {
	ID        string
	SessionID string
	GameType  string
	Status    string
	Port      int
	CreatedAt time.Time
}

var activeContainers = make(map[string]*GameContainer)

func StartGameContainer(sessionID, gameType string) (*GameContainer, error) {
	if isKubernetesMode() {
		return startGameK8s(sessionID, gameType)
	}

	cli, err := newDockerClient()
	if err != nil {
		return nil, err
	}

	// Get available port
	port := getAvailablePort()

	// Configure container based on game type
	config, hostConfig := getContainerConfig(sessionID, gameType, port)
	if err := ensureImageAvailable(cli, config.Image); err != nil {
		return nil, err
	}

	resp, err := cli.ContainerCreate(context.Background(), config, hostConfig, nil, nil, sessionID)
	if err != nil {
		return nil, err
	}

	// Start container (v28 uses container.StartOptions)
	if err := cli.ContainerStart(context.Background(), resp.ID, container.StartOptions{}); err != nil {
		return nil, err
	}

	gc := &GameContainer{
		ID:        resp.ID,
		SessionID: sessionID,
		GameType:  gameType,
		Status:    "running",
		Port:      port,
		CreatedAt: time.Now(),
	}

	activeContainers[sessionID] = gc
	log.Printf("Started game container %s for session %s", resp.ID, sessionID)
	return gc, nil
}

func StopGameContainer(sessionID string) error {
	if isKubernetesMode() {
		return stopGameK8s(sessionID)
	}

	gc, exists := activeContainers[sessionID]
	if !exists {
		return fmt.Errorf("container not found for session %s", sessionID)
	}

	cli, err := newDockerClient()
	if err != nil {
		return err
	}

	// Stop (v28 uses container.StopOptions)
	if err := cli.ContainerStop(context.Background(), gc.ID, container.StopOptions{}); err != nil {
		return err
	}
	// Remove (v28 uses container.RemoveOptions)
	if err := cli.ContainerRemove(context.Background(), gc.ID, container.RemoveOptions{RemoveVolumes: true, Force: true}); err != nil {
		log.Printf("Error removing container: %v", err)
	}
	stopVideoStreaming(sessionID)

	delete(activeContainers, sessionID)
	log.Printf("Stopped game container for session %s", sessionID)
	return nil
}

func getContainerConfig(sessionID, gameType string, port int) (*container.Config, *container.HostConfig) {
	switch gameType {
	case "minetest":
		return getMinetestConfig(sessionID, port)
	case "retroarch":
		return getRetroArchConfig(sessionID, port)
	case "freeciv":
		return getFreecivConfig(sessionID)
	default:
		return getDefaultConfig(sessionID, port)
	}
}

func getMinetestConfig(sessionID string, port int) (*container.Config, *container.HostConfig) {
	streamDir := filepath.Join("/tmp", "streams", sessionID)
	_ = os.MkdirAll(streamDir, 0755)

	config := &container.Config{
		Image: "ubuntu:22.04",
		Cmd: []string{
			"sh", "-c",
			"apt-get update && " +
				"DEBIAN_FRONTEND=noninteractive apt-get install -y " +
				"minetest xdotool xvfb ffmpeg fluxbox x11-utils x11-xserver-utils && " +
				"Xvfb :99 -screen 0 1280x720x24 & " +
				"sleep 1 && " +
				"export DISPLAY=:99 && " +
				"fluxbox >/tmp/fluxbox.log 2>&1 & " +
				"minetest >/tmp/minetest.log 2>&1 & " +
				"sleep 2 && " +
				"ffmpeg -hide_banner -loglevel error -f x11grab -video_size 1280x720 -framerate 30 " +
				"-i :99.0 -an -c:v libx264 -preset ultrafast -tune zerolatency -pix_fmt yuv420p " +
				"-g 60 -keyint_min 60 -sc_threshold 0 -f hls -hls_time 1 -hls_list_size 6 " +
				"-hls_flags delete_segments+append_list+independent_segments " +
				"-hls_segment_filename /tmp/streams/segment_%03d.ts /tmp/streams/playlist.m3u8",
		},
		Tty: true,
	}

	hostConfig := &container.HostConfig{
		Binds:      []string{fmt.Sprintf("%s:/tmp/streams", streamDir)},
		AutoRemove: true,
	}
	return config, hostConfig
}

func getRetroArchConfig(_ string, port int) (*container.Config, *container.HostConfig) {
	portHTTP, _ := nat.NewPort("tcp", "8080")

	config := &container.Config{
		Image: "inglebard/retroarch-web",
		ExposedPorts: nat.PortSet{
			portHTTP: struct{}{},
		},
		Tty: true,
	}

	hostConfig := &container.HostConfig{
		PortBindings: nat.PortMap{
			portHTTP: []nat.PortBinding{{HostPort: fmt.Sprintf("%d", port)}},
		},
		AutoRemove: true,
	}
	return config, hostConfig
}

func getFreecivConfig(sessionID string) (*container.Config, *container.HostConfig) {
	streamDir := filepath.Join("/tmp", "streams", sessionID)
	_ = os.MkdirAll(streamDir, 0755)

	config := &container.Config{
		Image: "freeciv-combined",
		Env:   []string{"DISPLAY=:99"},
		Cmd:   []string{"/home/freeciv/start.sh"},
		Tty:   true,
	}

	hostConfig := &container.HostConfig{
		Binds:      []string{fmt.Sprintf("%s:/tmp/streams", streamDir)},
		AutoRemove: true,
	}

	return config, hostConfig
}

func getDefaultConfig(_ string, port int) (*container.Config, *container.HostConfig) {
	config := &container.Config{
		Image: "ubuntu:22.04",
		Cmd:   []string{"sleep", "infinity"},
		Tty:   true,
	}
	hostConfig := &container.HostConfig{AutoRemove: true}
	return config, hostConfig
}

func getAvailablePort() int {
	return 30000 + len(activeContainers)
}

func setupVideoStreaming(container *GameContainer) {
	streamDir := filepath.Join("/tmp", "streams", container.SessionID)
	os.MkdirAll(streamDir, 0755)
	go startFFmpegStream(container, streamDir)
}

func GetContainer(sessionID string) (*GameContainer, error) {
	gc, exists := activeContainers[sessionID]
	if !exists {
		return nil, fmt.Errorf("container not found for session %s", sessionID)
	}
	return gc, nil
}

func ListActiveContainers() map[string]*GameContainer { return activeContainers }

func ensureImageAvailable(cli *client.Client, imageName string) error {
	if imageName == "" {
		return fmt.Errorf("container image is empty")
	}

	if _, err := cli.ImageInspect(context.Background(), imageName); err == nil {
		return nil
	}

	log.Printf("Pulling missing image: %s", imageName)
	reader, err := cli.ImagePull(context.Background(), imageName, image.PullOptions{})
	if err != nil {
		return fmt.Errorf("failed to pull image %s: %w", imageName, err)
	}
	defer reader.Close()
	_, _ = io.Copy(io.Discard, reader)
	return nil
}
