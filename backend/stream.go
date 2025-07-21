// package backend

import (
    "io"
    "log"
    "net"
    "github.com/gofiber/websocket/v2"
)

// StreamHandler proxies video stream to WebSocket clients
func StreamHandler(c *websocket.Conn) {
    sessionID := c.Params("id")
    log.Printf("Client connected for stream: %s", sessionID)

    // Connect to FFmpeg stream inside the container
    // Assuming container streams to TCP localhost:9000
    conn, err := net.Dial("tcp", "localhost:9000")
    if err != nil {
        log.Printf("Error connecting to stream: %v", err)
        _ = c.WriteMessage(websocket.TextMessage, []byte("Stream unavailable"))
        return
    }
    defer conn.Close()

    buf := make([]byte, 1024*8)
    for {
        n, err := conn.Read(buf)
        if err != nil {
            if err != io.EOF {
                log.Printf("Stream read error: %v", err)
            }
            break
        }
        // Send raw frame data to frontend WebSocket
        if err := c.WriteMessage(websocket.BinaryMessage, buf[:n]); err != nil {
            log.Printf("WebSocket write error: %v", err)
            break
        }
    }
}
