package main

import (
	"crypto/rand"
	"encoding/hex"
	"log"
	"math/big"
	"time"

	"github.com/gofiber/fiber/v3"
)

func SetupAPIRoutes(api fiber.Router) {
	// Create new game session
	api.Post("/sessions", func(c fiber.Ctx) error {
		var request struct {
			GameType string `json:"gameType"`
		}

		if err := c.Bind().Body(&request); err != nil {
			return c.Status(400).JSON(fiber.Map{"error": "Invalid request body"})
		}

		if request.GameType == "" {
			request.GameType = "minetest" // Default game
		}

		sessionID := generateSessionID()
		container, err := StartGameContainer(sessionID, request.GameType)
		if err != nil {
			log.Printf("Error starting container: %v", err)
			return c.Status(500).JSON(fiber.Map{"error": err.Error()})
		}

		return c.JSON(fiber.Map{
			"sessionID":   sessionID,
			"containerID": container.ID,
			"gameType":    container.GameType,
			"port":        container.Port,
			"status":      container.Status,
			"createdAt":   container.CreatedAt,
		})
	})

	// Get session info
	api.Get("/sessions/:id", func(c fiber.Ctx) error {
		sessionID := c.Params("id")
		container, err := GetContainer(sessionID)
		if err != nil {
			return c.Status(404).JSON(fiber.Map{"error": "Session not found"})
		}

		return c.JSON(fiber.Map{
			"sessionID":   container.SessionID,
			"containerID": container.ID,
			"gameType":    container.GameType,
			"port":        container.Port,
			"status":      container.Status,
			"createdAt":   container.CreatedAt,
		})
	})

	// List all active sessions
	api.Get("/sessions", func(c fiber.Ctx) error {
		containers := ListActiveContainers()
		sessions := make([]fiber.Map, 0, len(containers))

		for _, container := range containers {
			sessions = append(sessions, fiber.Map{
				"sessionID":   container.SessionID,
				"containerID": container.ID,
				"gameType":    container.GameType,
				"port":        container.Port,
				"status":      container.Status,
				"createdAt":   container.CreatedAt,
			})
		}

		return c.JSON(fiber.Map{
			"sessions": sessions,
			"count":    len(sessions),
		})
	})

	// Stop game session
	api.Delete("/sessions/:id", func(c fiber.Ctx) error {
		sessionID := c.Params("id")
		if err := StopGameContainer(sessionID); err != nil {
			return c.Status(500).JSON(fiber.Map{"error": err.Error()})
		}
		return c.JSON(fiber.Map{"message": "Session stopped successfully"})
	})

	// Get available games
	api.Get("/games", func(c fiber.Ctx) error {
		games := []fiber.Map{
			{
				"id":          "minetest",
				"name":        "Minetest",
				"description": "Open-source voxel game engine",
				"image":       "alpine:3.19",
				"port":        30000,
				"type":        "server",
			},
			{
				"id":          "retroarch",
				"name":        "RetroArch",
				"description": "Cross-platform emulator frontend",
				"image":       "inglebard/retroarch-web",
				"port":        8080,
				"type":        "web",
			},
		}

		return c.JSON(fiber.Map{
			"games": games,
			"count": len(games),
		})
	})

	// Health check
	api.Get("/health", func(c fiber.Ctx) error {
		return c.JSON(fiber.Map{
			"status":    "healthy",
			"timestamp": time.Now(),
			"version":   "1.0.0",
		})
	})
}

func generateSessionID() string {
	bytes := make([]byte, 8)
	rand.Read(bytes)
	return "session_" + hex.EncodeToString(bytes)
}

func RandString(n int) string {
	const letters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789"
	result := make([]byte, n)
	for i := range result {
		num, _ := rand.Int(rand.Reader, big.NewInt(int64(len(letters))))
		result[i] = letters[num.Int64()]
	}
	return string(result)
}
