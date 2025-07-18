package main

import (
    "log"
    "github.com/gofiber/fiber/v3"
    "github.com/karman103/mvp/backend/routes"
)

func main() {
    app := fiber.New()

    // API routes
    api := app.Group("/api")
    routes.SetupAPIRoutes(api)

    // WebSocket routes
    ws := app.Group("/ws")
    routes.SetupWebSocketRoutes(ws)

    log.Println("Backend running on :3000")
    log.Fatal(app.Listen(":3000"))
}
