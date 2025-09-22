package main

import (
	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/websocket/v2"
)

func SetupWebSocketRoutes(ws fiber.Router) {
	// Video streaming WebSocket
	ws.Get("/stream/:id", websocket.New(StreamHandler))

	// Input handling WebSocket
	ws.Get("/input/:id", websocket.New(InputHandler))

	// Game status WebSocket
	ws.Get("/status/:id", websocket.New(StatusHandler))
}
