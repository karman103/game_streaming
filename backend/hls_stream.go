package main

import (
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"sync"
)

var (
	streamMu        sync.Mutex
	streamProcesses = make(map[string]*exec.Cmd)
)

func startFFmpegStream(container *GameContainer, streamDir string) {
	_ = os.MkdirAll(streamDir, 0755)
	_ = os.Remove(filepath.Join(streamDir, "playlist.m3u8"))
	_ = os.Remove(filepath.Join(streamDir, "segment_000.ts"))

	playlistPath := filepath.Join(streamDir, "playlist.m3u8")
	segmentPattern := filepath.Join(streamDir, "segment_%03d.ts")

	args := []string{
		"-hide_banner",
		"-loglevel", "error",
		"-re",
		"-f", "lavfi",
		"-i", "testsrc2=size=1280x720:rate=30",
		"-an",
		"-c:v", "libx264",
		"-preset", "ultrafast",
		"-tune", "zerolatency",
		"-pix_fmt", "yuv420p",
		"-g", "60",
		"-keyint_min", "60",
		"-sc_threshold", "0",
		"-f", "hls",
		"-hls_time", "1",
		"-hls_list_size", "6",
		"-hls_flags", "delete_segments+append_list+independent_segments",
		"-hls_segment_filename", segmentPattern,
		playlistPath,
	}

	cmd := exec.Command("ffmpeg", args...)
	if err := cmd.Start(); err != nil {
		log.Printf("Failed to start ffmpeg for session %s: %v", container.SessionID, err)
		return
	}

	streamMu.Lock()
	streamProcesses[container.SessionID] = cmd
	streamMu.Unlock()

	log.Printf("Started ffmpeg HLS for session %s", container.SessionID)

	go func(sessionID string, process *exec.Cmd) {
		err := process.Wait()
		if err != nil {
			log.Printf("ffmpeg exited for session %s: %v", sessionID, err)
		}
		streamMu.Lock()
		delete(streamProcesses, sessionID)
		streamMu.Unlock()
	}(container.SessionID, cmd)
}

func stopVideoStreaming(sessionID string) {
	streamMu.Lock()
	cmd, ok := streamProcesses[sessionID]
	streamMu.Unlock()
	if !ok || cmd == nil || cmd.Process == nil {
		return
	}

	if err := cmd.Process.Kill(); err != nil {
		log.Printf("Failed to stop ffmpeg for session %s: %v", sessionID, err)
		return
	}
	log.Printf("Stopped ffmpeg HLS for session %s", sessionID)
}

func isStreamProcessRunning(sessionID string) bool {
	streamMu.Lock()
	defer streamMu.Unlock()
	cmd, ok := streamProcesses[sessionID]
	return ok && cmd != nil && cmd.Process != nil
}
