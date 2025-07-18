package backend

import (
    "context"
    "encoding/json"
    "log"

    "github.com/docker/docker/client"
    "github.com/docker/docker/api/types"
    "github.com/gofiber/websocket/v2"
)

type InputEvent struct {
    Type string `json:"type"` // "keydown", "keyup", "mousemove"
    Key  string `json:"key,omitempty"`
    X    int    `json:"x,omitempty"`
    Y    int    `json:"y,omitempty"`
}

// InputHandler forwards frontend inputs to Docker container
func InputHandler(c *websocket.Conn) {
    sessionID := c.Params("id")
    log.Printf("Client connected for input: %s", sessionID)

    cli, err := client.NewClientWithOpts(client.FromEnv)
    if err != nil {
        log.Printf("Docker client error: %v", err)
        return
    }

    for {
        _, msg, err := c.ReadMessage()
        if err != nil {
            log.Printf("WebSocket read error: %v", err)
            break
        }

        var input InputEvent
        if err := json.Unmarshal(msg, &input); err != nil {
            log.Printf("Invalid input JSON: %v", err)
            continue
        }

        log.Printf("Received input: %+v", input)

        // Example: simulate keypress in container using xdotool
        // You must have xdotool installed in your game Docker image
        cmd := []string{"xdotool", "key", input.Key}
        execResp, err := cli.ContainerExecCreate(context.Background(), sessionID, types.ExecConfig{
            Cmd:          cmd,
            AttachStdout: true,
            AttachStderr: true,
        })
        if err != nil {
            log.Printf("Docker exec create error: %v", err)
            continue
        }

        err = cli.ContainerExecStart(context.Background(), execResp.ID, types.ExecStartCheck{})
        if err != nil {
            log.Printf("Docker exec start error: %v", err)
        }
    }
}
