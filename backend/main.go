package main

import (
	"log"
	"os"

	"github.com/gofiber/fiber/v3"
	"github.com/gofiber/fiber/v3/middleware/cors"
	"github.com/gofiber/fiber/v3/middleware/logger"
	"github.com/gofiber/fiber/v3/middleware/recover"
)

func main() {
	// Create Fiber app
	app := fiber.New(fiber.Config{
		ErrorHandler: func(c fiber.Ctx, err error) error {
			code := fiber.StatusInternalServerError
			if e, ok := err.(*fiber.Error); ok {
				code = e.Code
			}
			return c.Status(code).JSON(fiber.Map{
				"error": err.Error(),
			})
		},
	})

	// Middleware
	app.Use(recover.New())
	app.Use(logger.New())
	app.Use(cors.New(cors.Config{
		AllowOrigins: []string{"*"},
		AllowMethods: []string{"GET", "POST", "PUT", "DELETE", "OPTIONS"},
		AllowHeaders: []string{"Origin", "Content-Type", "Accept", "Authorization"},
	}))

	// API routes
	api := app.Group("/api")
	SetupAPIRoutes(api)

	// WebSocket routes
	ws := app.Group("/ws")
	SetupWebSocketRoutes(ws)

	// Streaming routes
	SetupStreamingRoutes(app)

	// Health check
	app.Get("/health", func(c fiber.Ctx) error {
		return c.JSON(fiber.Map{
			"status":  "healthy",
			"service": "game-streaming-backend",
			"version": "1.0.0",
		})
	})

	// Start server
	port := os.Getenv("PORT")
	if port == "" {
		port = "3000"
	}

	log.Printf("🚀 Game Streaming Backend starting on port %s", port)
	log.Printf("📡 WebSocket endpoints available at /ws/")
	log.Printf("🎮 API endpoints available at /api/")
	log.Printf("📺 Streaming endpoints available at /api/stream/")

	log.Fatal(app.Listen(":" + port))
}
