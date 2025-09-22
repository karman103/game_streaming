package main

import (
	"encoding/json"
	"fmt"
	"os"
	"os/exec"
	"strings"
	"time"
)

type k8sService struct {
	Spec struct {
		Ports []struct {
			NodePort int `json:"nodePort"`
		} `json:"ports"`
	} `json:"spec"`
}

func isKubernetesMode() bool {
	return strings.EqualFold(os.Getenv("ORCHESTRATOR"), "kubernetes")
}

func startGameK8s(sessionID, gameType string) (*GameContainer, error) {
	namespace := getenvDefault("K8S_NAMESPACE", "game-stream")

	// Ensure namespace exists (ignore error)
	_ = exec.Command("kubectl", "create", "namespace", namespace).Run()

	image, cmd, ports := k8sGameTemplate(gameType)
	labels := fmt.Sprintf("app=%s,component=game,session=%s", sessionID, sessionID)

	// Create pod
	runArgs := []string{
		"run", sessionID,
		"--image", image,
		"-n", namespace,
		"--labels", labels,
		"--restart", "Never",
	}
	if len(cmd) > 0 {
		runArgs = append(runArgs, "--command", "--")
		runArgs = append(runArgs, cmd...)
	}
	if out, err := exec.Command("kubectl", runArgs...).CombinedOutput(); err != nil {
		return nil, fmt.Errorf("kubectl run failed: %v: %s", err, string(out))
	}

	// Expose service (NodePort) for first TCP/UDP port if any
	var nodePort int
	if len(ports) > 0 {
		port := ports[0]
		proto := "tcp"
		if strings.Contains(port, "/") {
			parts := strings.SplitN(port, "/", 2)
			port = parts[0]
			proto = strings.ToLower(parts[1])
		}

		// Create service
		args := []string{
			"expose", "pod", sessionID,
			"--name", sessionID + "-svc",
			"-n", namespace,
			"--type", "NodePort",
			"--port", port,
			"--target-port", port,
		}
		if proto == "udp" {
			args = append(args, "--protocol", "UDP")
		}
		if out, err := exec.Command("kubectl", args...).CombinedOutput(); err != nil {
			return nil, fmt.Errorf("kubectl expose failed: %v: %s", err, string(out))
		}

		// Give k8s a moment to allocate a nodePort
		time.Sleep(1 * time.Second)

		// Read nodePort
		getArgs := []string{"get", "svc", sessionID + "-svc", "-n", namespace, "-o", "json"}
		out, err := exec.Command("kubectl", getArgs...).CombinedOutput()
		if err != nil {
			return nil, fmt.Errorf("kubectl get svc failed: %v: %s", err, string(out))
		}
		var svc k8sService
		if err := json.Unmarshal(out, &svc); err == nil && len(svc.Spec.Ports) > 0 {
			nodePort = svc.Spec.Ports[0].NodePort
		}
	}

	gc := &GameContainer{
		ID:        sessionID, // use sessionID as k8s identity
		SessionID: sessionID,
		GameType:  gameType,
		Status:    "running",
		Port:      nodePort,
		CreatedAt: time.Now(),
	}
	activeContainers[sessionID] = gc
	go setupVideoStreaming(gc)
	return gc, nil
}

func stopGameK8s(sessionID string) error {
	namespace := getenvDefault("K8S_NAMESPACE", "game-stream")
	_ = exec.Command("kubectl", "delete", "svc", sessionID+"-svc", "-n", namespace, "--ignore-not-found").Run()
	if out, err := exec.Command("kubectl", "delete", "pod", sessionID, "-n", namespace, "--ignore-not-found").CombinedOutput(); err != nil {
		return fmt.Errorf("kubectl delete pod failed: %v: %s", err, string(out))
	}
	delete(activeContainers, sessionID)
	return nil
}

func k8sGameTemplate(gameType string) (image string, cmd []string, ports []string) {
	switch gameType {
	case "minetest":
		return "alpine:3.19", []string{"sh", "-c", "apk add --no-cache minetest-server && minetestserver --worldname docker_world --port 30000"}, []string{"30000/udp", "30000/tcp"}
	case "retroarch":
		return "inglebard/retroarch-web", nil, []string{"8080/tcp"}
	default:
		return "ubuntu:22.04", []string{"sleep", "infinity"}, nil
	}
}

func getenvDefault(k, def string) string {
	v := os.Getenv(k)
	if v == "" {
		return def
	}
	return v
}
