package routes

import (
    "backend"
    "github.com/gofiber/fiber/v3"
)

func SetupAPIRoutes(api fiber.Router) {
    api.Post("/sessions", func(c *fiber.Ctx) error {
        sessionID := "game_" + RandString(8)
        containerID, err := backend.StartGameContainer(sessionID)
        if err != nil {
            return c.Status(500).JSON(fiber.Map{"error": err.Error()})
        }
        return c.JSON(fiber.Map{
            "sessionID":   sessionID,
            "containerID": containerID,
        })
    })

    api.Delete("/sessions/:id", func(c *fiber.Ctx) error {
        containerID := c.Params("id")
        err := backend.StopGameContainer(containerID)
        if err != nil {
            return c.Status(500).JSON(fiber.Map{"error": err.Error()})
        }
        return c.JSON(fiber.Map{"message": "Session stopped"})
    })
}
