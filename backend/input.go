package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"strings"

	"github.com/docker/docker/api/types/container"
	"github.com/docker/docker/client"
	"github.com/gofiber/websocket/v2"
)

type InputEvent struct {
	Type     string `json:"type"` // "keydown", "keyup", "mousemove", "mousedown", "mouseup", "wheel"
	Key      string `json:"key,omitempty"`
	Code     string `json:"code,omitempty"`
	X        int    `json:"x,omitempty"`
	Y        int    `json:"y,omitempty"`
	Button   int    `json:"button,omitempty"`
	DeltaX   int    `json:"deltaX,omitempty"`
	DeltaY   int    `json:"deltaY,omitempty"`
	CtrlKey  bool   `json:"ctrlKey,omitempty"`
	ShiftKey bool   `json:"shiftKey,omitempty"`
	AltKey   bool   `json:"altKey,omitempty"`
	MetaKey  bool   `json:"metaKey,omitempty"`
}

type GamepadEvent struct {
	Type    string  `json:"type"` // "gamepadconnected", "gamepaddisconnected", "gamepadbutton", "gamepadaxis"
	Index   int     `json:"index,omitempty"`
	Button  int     `json:"button,omitempty"`
	Value   float64 `json:"value,omitempty"`
	Pressed bool    `json:"pressed,omitempty"`
	Axis    int     `json:"axis,omitempty"`
}

// InputHandler forwards frontend inputs to Docker container
func InputHandler(c *websocket.Conn) {
	sessionID := c.Params("id")
	log.Printf("Client connected for input: %s", sessionID)

	containerInfo, err := GetContainer(sessionID)
	if err != nil {
		log.Printf("Container not found for session %s: %v", sessionID, err)
		c.WriteMessage(websocket.TextMessage, []byte(`{"error": "Container not found"}`))
		return
	}

	cli, err := client.NewClientWithOpts(client.FromEnv)
	if err != nil {
		log.Printf("Docker client error: %v", err)
		return
	}

	defer c.Close()

	for {
		_, msg, err := c.ReadMessage()
		if err != nil {
			log.Printf("WebSocket read error: %v", err)
			break
		}

		// Try to parse as InputEvent first
		var input InputEvent
		if err := json.Unmarshal(msg, &input); err == nil {
			handleInputEvent(cli, containerInfo.ID, input)
			continue
		}

		// Try to parse as GamepadEvent
		var gamepad GamepadEvent
		if err := json.Unmarshal(msg, &gamepad); err == nil {
			handleGamepadEvent(cli, containerInfo.ID, gamepad)
			continue
		}

		log.Printf("Invalid input JSON: %v", err)
	}
}

func handleInputEvent(cli *client.Client, containerID string, input InputEvent) {
	log.Printf("Received input: %+v", input)

	var cmd []string
	var err error

	switch input.Type {
	case "keydown", "keyup":
		cmd, err = buildKeyboardCommand(input)
	case "mousemove":
		cmd, err = buildMouseMoveCommand(input)
	case "mousedown", "mouseup":
		cmd, err = buildMouseClickCommand(input)
	case "wheel":
		cmd, err = buildMouseWheelCommand(input)
	default:
		log.Printf("Unknown input type: %s", input.Type)
		return
	}

	if err != nil {
		log.Printf("Error building command: %v", err)
		return
	}

	executeCommand(cli, containerID, cmd)
}

func handleGamepadEvent(cli *client.Client, containerID string, gamepad GamepadEvent) {
	log.Printf("Received gamepad input: %+v", gamepad)

	var cmd []string
	var err error

	switch gamepad.Type {
	case "gamepadbutton":
		cmd, err = buildGamepadButtonCommand(gamepad)
	case "gamepadaxis":
		cmd, err = buildGamepadAxisCommand(gamepad)
	default:
		log.Printf("Unknown gamepad type: %s", gamepad.Type)
		return
	}

	if err != nil {
		log.Printf("Error building gamepad command: %v", err)
		return
	}

	executeCommand(cli, containerID, cmd)
}

func buildKeyboardCommand(input InputEvent) ([]string, error) {
	keyMap := map[string]string{
		"ArrowUp": "Up", "ArrowDown": "Down", "ArrowLeft": "Left", "ArrowRight": "Right",
		"Enter": "Return", "Space": "space", "Escape": "Escape", "Tab": "Tab",
		"Backspace": "BackSpace", "Delete": "Delete", "Home": "Home", "End": "End",
		"PageUp": "Prior", "PageDown": "Next", "F1": "F1", "F2": "F2", "F3": "F3",
		"F4": "F4", "F5": "F5", "F6": "F6", "F7": "F7", "F8": "F8", "F9": "F9",
		"F10": "F10", "F11": "F11", "F12": "F12",
	}
	key := input.Key
	if mapped, ok := keyMap[key]; ok {
		key = mapped
	}
	mods := []string{}
	if input.CtrlKey {
		mods = append(mods, "ctrl")
	}
	if input.ShiftKey {
		mods = append(mods, "shift")
	}
	if input.AltKey {
		mods = append(mods, "alt")
	}
	if input.MetaKey {
		mods = append(mods, "super")
	}
	if len(mods) > 0 {
		return []string{"xdotool", "key", strings.Join(append(mods, key), "+")}, nil
	}
	return []string{"xdotool", "key", key}, nil
}

func buildMouseMoveCommand(input InputEvent) ([]string, error) {
	return []string{"xdotool", "mousemove", fmt.Sprintf("%d", input.X), fmt.Sprintf("%d", input.Y)}, nil
}

func buildMouseClickCommand(input InputEvent) ([]string, error) {
	button := "1"
	switch input.Button {
	case 1:
		button = "2"
	case 2:
		button = "3"
	}
	action := "click"
	if input.Type == "mousedown" {
		action = "mousedown"
	} else if input.Type == "mouseup" {
		action = "mouseup"
	}
	return []string{"xdotool", action, button}, nil
}

func buildMouseWheelCommand(input InputEvent) ([]string, error) {
	if input.DeltaY > 0 {
		return []string{"xdotool", "click", "4"}, nil
	}
	if input.DeltaY < 0 {
		return []string{"xdotool", "click", "5"}, nil
	}
	return nil, fmt.Errorf("no wheel movement detected")
}

func buildGamepadButtonCommand(gamepad GamepadEvent) ([]string, error) {
	buttonMap := map[int]string{0: "space", 1: "Escape", 2: "Return", 3: "Tab", 4: "F1", 5: "F2", 6: "F3", 7: "F4"}
	key, ok := buttonMap[gamepad.Button]
	if !ok {
		return nil, fmt.Errorf("unknown gamepad button: %d", gamepad.Button)
	}
	if gamepad.Pressed {
		return []string{"xdotool", "keydown", key}, nil
	}
	return []string{"xdotool", "keyup", key}, nil
}

func buildGamepadAxisCommand(gamepad GamepadEvent) ([]string, error) {
	if gamepad.Axis == 0 {
		if gamepad.Value > 0.5 {
			return []string{"xdotool", "keydown", "Right"}, nil
		}
		if gamepad.Value < -0.5 {
			return []string{"xdotool", "keydown", "Left"}, nil
		}
		return []string{"xdotool", "keyup", "Right", "Left"}, nil
	}
	if gamepad.Axis == 1 {
		if gamepad.Value > 0.5 {
			return []string{"xdotool", "keydown", "Down"}, nil
		}
		if gamepad.Value < -0.5 {
			return []string{"xdotool", "keydown", "Up"}, nil
		}
		return []string{"xdotool", "keyup", "Up", "Down"}, nil
	}
	return nil, fmt.Errorf("unknown gamepad axis: %d", gamepad.Axis)
}

func executeCommand(cli *client.Client, containerID string, cmd []string) {
	if len(cmd) == 0 {
		return
	}
	execResp, err := cli.ContainerExecCreate(context.Background(), containerID, container.ExecOptions{
		Cmd:          cmd,
		AttachStdout: true,
		AttachStderr: true,
	})
	if err != nil {
		log.Printf("Docker exec create error: %v", err)
		return
	}
	if err := cli.ContainerExecStart(context.Background(), execResp.ID, container.ExecStartOptions{}); err != nil {
		log.Printf("Docker exec start error: %v", err)
	}
}
