package main

import (
	"github.com/gofiber/fiber/v3"
)

func SetupWebSocketRoutes(ws fiber.Router) {
	// WebSocket handlers are temporarily disabled due to framework version mismatch.
	// The API and HLS endpoints still run for local development.
	ws.Get("/stream/:id", func(c fiber.Ctx) error {
		return c.Status(fiber.StatusNotImplemented).JSON(fiber.Map{
			"error": "websocket streaming route is not enabled in current build",
		})
	})
	ws.Get("/input/:id", func(c fiber.Ctx) error {
		return c.Status(fiber.StatusNotImplemented).JSON(fiber.Map{
			"error": "websocket input route is not enabled in current build",
		})
	})
	ws.Get("/status/:id", func(c fiber.Ctx) error {
		return c.Status(fiber.StatusNotImplemented).JSON(fiber.Map{
			"error": "websocket status route is not enabled in current build",
		})
	})
}
