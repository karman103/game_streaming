package main

import (
    "fmt"
    "log"
    "time"
    "io/ioutil"
    "os/exec"
    "io"
    "github.com/gofiber/fiber/v2"
    "github.com/gofiber/websocket/v2"
    "github.com/gofiber/fiber/v2/middleware/proxy"
    "strings"
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

    // app.Get("/ws/stream", websocket.New(func(c *websocket.Conn) {
    //     fmt.Println("Client connected")
    //     defer c.Close()

    //     ticker := time.NewTicker(time.Second / 30) // 30 FPS
    //     defer ticker.Stop()

    //     for range ticker.C {
    //         imgBytes, err := ioutil.ReadFile("test.png") // replace with game frame
    //         if err != nil {
    //             log.Println("Error reading frame:", err)
    //             break
    //         }

    //         if err := c.WriteMessage(websocket.BinaryMessage, imgBytes); err != nil {
    //             log.Println("Error sending frame:", err)
    //             break
    //         }
    //     }
    // }))

     app.Get("/ws/stream", websocket.New(func(c *websocket.Conn) {
        log.Println("Client connected")
        defer c.Close()

        cmd := exec.Command("ffmpeg", "-f", "x11grab", "-video_size", "1280x720",
            "-framerate", "30", "-i", ":0.0", "-update", "1", "-q:v", "5",
            "-f", "image2pipe", "-vcodec", "mjpeg", "-")

        stdout, err := cmd.StdoutPipe()
        if err != nil {
            log.Fatal("Error creating stdout pipe:", err)
        }

        if err := cmd.Start(); err != nil {
            log.Fatal("Error starting FFmpeg:", err)
        }

        buffer := make([]byte, 1024*64) // 64KB buffer

        for {
            n, err := stdout.Read(buffer)
            if err != nil {
                if err == io.EOF {
                    break
                }
                log.Println("Read error:", err)
                break
            }

            if err := c.WriteMessage(websocket.BinaryMessage, buffer[:n]); err != nil {
                log.Println("WebSocket write error:", err)
                break
            }

            time.Sleep(time.Second / 30) // throttle to 30 FPS
        }

        cmd.Wait()
        log.Println("FFmpeg stopped, client disconnected")
    }))
app.Use("/stream/mario", func(c *fiber.Ctx) error {
        if c.Path() == "/stream/mario" || c.Path() == "/stream/mario/" {
            handler := proxy.Forward("http://localhost:8080")
            if err := handler(c); err != nil {
                return err
            }

            // Grab response body
            body := c.Response().Body()
            // Inject <base href='/stream/mario/'>
            updated := strings.Replace(
                string(body),
                "<head>",
                "<head><base href='/stream/mario/'>",
                1,
            )
            return c.SendString(updated)
        }
        return c.Next()
    })
    app.Use(func(c *fiber.Ctx) error {
    c.Set("Access-Control-Allow-Origin", "*")
    c.Set("X-Frame-Options", "ALLOWALL")
    c.Set("Content-Security-Policy", "frame-ancestors *")
    return c.Next()
})

    // Proxy all asset requests under /stream/mario
    app.All("/stream/mario/*", func(c *fiber.Ctx) error {
        targetPath := c.OriginalURL()[len("/stream/mario"):]
        if targetPath == "" {
            targetPath = "/" // root fallback
        }
        targetURL := "http://localhost:8080" + targetPath
        log.Println("Proxying to:", targetURL)
        return proxy.Do(c, targetURL)
    })



    log.Println("Server running at http://localhost:3000")
    log.Fatal(app.Listen(":3000"))
}
