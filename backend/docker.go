package main

import (
	"context"
	"fmt"
	"log"
	"os"
	"path/filepath"
	"time"

	"github.com/docker/docker/api/types/container"
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

	cli, err := client.NewClientWithOpts(client.FromEnv)
	if err != nil {
		return nil, err
	}

	// Get available port
	port := getAvailablePort()

	// Configure container based on game type
	config, hostConfig := getContainerConfig(gameType, port)

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
	go setupVideoStreaming(gc)
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

	cli, err := client.NewClientWithOpts(client.FromEnv)
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

	delete(activeContainers, sessionID)
	log.Printf("Stopped game container for session %s", sessionID)
	return nil
}

func getContainerConfig(gameType string, port int) (*container.Config, *container.HostConfig) {
	switch gameType {
	case "minetest":
		return getMinetestConfig(port)
	case "retroarch":
		return getRetroArchConfig(port)
	default:
		return getDefaultConfig(port)
	}
}

func getMinetestConfig(port int) (*container.Config, *container.HostConfig) {
	// Expose both UDP/TCP 30000
	portUDP, _ := nat.NewPort("udp", "30000")
	portTCP, _ := nat.NewPort("tcp", "30000")

	config := &container.Config{
		Image: "alpine:3.19",
		Cmd: []string{
			"sh", "-c",
			"apk add --no-cache minetest-server && minetestserver --worldname docker_world --port 30000",
		},
		ExposedPorts: nat.PortSet{
			portUDP: struct{}{},
			portTCP: struct{}{},
		},
		Tty: true,
	}

	hostConfig := &container.HostConfig{
		PortBindings: nat.PortMap{
			portUDP: []nat.PortBinding{{HostPort: fmt.Sprintf("%d", port)}},
			portTCP: []nat.PortBinding{{HostPort: fmt.Sprintf("%d", port)}},
		},
		AutoRemove: true,
	}
	return config, hostConfig
}

func getRetroArchConfig(port int) (*container.Config, *container.HostConfig) {
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

func getDefaultConfig(port int) (*container.Config, *container.HostConfig) {
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

func startFFmpegStream(container *GameContainer, streamDir string) {
	streamFile := filepath.Join(streamDir, "stream.m3u8")
	content := `#EXTM3U
#EXT-X-VERSION:3
#EXT-X-TARGETDURATION:10
#EXT-X-MEDIA-SEQUENCE:0
#EXTINF:10.0,
segment0.ts
#EXT-X-ENDLIST`
	os.WriteFile(streamFile, []byte(content), 0644)
	log.Printf("Created stream file for session %s", container.SessionID)
}

func GetContainer(sessionID string) (*GameContainer, error) {
	gc, exists := activeContainers[sessionID]
	if !exists {
		return nil, fmt.Errorf("container not found for session %s", sessionID)
	}
	return gc, nil
}

func ListActiveContainers() map[string]*GameContainer { return activeContainers }
