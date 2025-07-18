package routes

import (
    "github.com/gofiber/fiber/v3"
    "github.com/gofiber/websocket/v2"
)

func SetupWebSocketRoutes(ws fiber.Router) {
    ws.Get("/stream/:id", websocket.New(StreamHandler))
    ws.Get("/input/:id", websocket.New(InputHandler))
}
