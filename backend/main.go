package main

import (
    "fmt"
    "log"
    "time"
    "io/ioutil"

    "github.com/gofiber/fiber/v2"
    "github.com/gofiber/websocket/v2"
)

func main() {
    app := fiber.New()

    // Serve WebSocket endpoint
    app.Get("/ws", websocket.New(func(c *websocket.Conn) {
        fmt.Println("Client connected")
        defer func() {
            fmt.Println("Client disconnected")
            c.Close()
        }()

        for {
            // Read message from client
            mt, msg, err := c.ReadMessage()
            if err != nil {
                fmt.Println("read error:", err)
                break
            }
            fmt.Printf("Received: %s\n", msg)

            // Send message back to client
            response := fmt.Sprintf("Server time: %s", time.Now().Format(time.RFC3339))
            err = c.WriteMessage(mt, []byte(response))
            if err != nil {
                fmt.Println("write error:", err)
                break
            }
        }
    }))

    app.Get("/ws/image", websocket.New(func(c *websocket.Conn) {
        fmt.Println("Client connected")
        defer c.Close()

        for {
            // Load image file (e.g., PNG or JPEG)
            imgBytes, err := ioutil.ReadFile("test.png")
            if err != nil {
                log.Println("Failed to read image:", err)
                break
            }

            // Send as binary message
            if err := c.WriteMessage(websocket.BinaryMessage, imgBytes); err != nil {
                log.Println("Failed to send image:", err)
                break
            }

            time.Sleep(100 * time.Millisecond) // simulate 10 FPS
        }
    }))

    log.Fatal(app.Listen(":3000"))
}
