package main

import (
	"encoding/json"
	"log"
	"time"

	"github.com/gofiber/websocket/v2"
)

type StatusMessage struct {
	Type      string      `json:"type"`
	SessionID string      `json:"sessionId"`
	Data      interface{} `json:"data"`
	Timestamp time.Time   `json:"timestamp"`
}

// StatusHandler provides real-time status updates for game sessions
func StatusHandler(c *websocket.Conn) {
	sessionID := c.Params("id")
	log.Printf("Client connected for status updates: %s", sessionID)

	container, err := GetContainer(sessionID)
	if err != nil {
		log.Printf("Container not found for session %s: %v", sessionID, err)
		c.WriteMessage(websocket.TextMessage, []byte(`{"error": "Container not found"}`))
		return
	}

	defer c.Close()

	// Send initial status
	sendStatusUpdate(c, sessionID, "connected", map[string]interface{}{
		"containerID": container.ID,
		"gameType":    container.GameType,
		"port":        container.Port,
		"status":      container.Status,
	})

	// Status update ticker
	ticker := time.NewTicker(5 * time.Second)
	defer ticker.Stop()

	for {
		select {
		case <-ticker.C:
			// Get updated container status
			updatedContainer, err := GetContainer(sessionID)
			if err != nil {
				sendStatusUpdate(c, sessionID, "error", map[string]interface{}{
					"error": "Container not found",
				})
				return
			}

			// Send status update
			sendStatusUpdate(c, sessionID, "status", map[string]interface{}{
				"containerID": updatedContainer.ID,
				"gameType":    updatedContainer.GameType,
				"port":        updatedContainer.Port,
				"status":      updatedContainer.Status,
				"uptime":      time.Since(updatedContainer.CreatedAt).String(),
			})

		case <-time.After(30 * time.Second):
			// Send heartbeat
			sendStatusUpdate(c, sessionID, "heartbeat", map[string]interface{}{
				"timestamp": time.Now(),
			})
		}
	}
}

func sendStatusUpdate(c *websocket.Conn, sessionID, msgType string, data interface{}) {
	message := StatusMessage{
		Type:      msgType,
		SessionID: sessionID,
		Data:      data,
		Timestamp: time.Now(),
	}

	jsonData, err := json.Marshal(message)
	if err != nil {
		log.Printf("Error marshaling status message: %v", err)
		return
	}

	err = c.WriteMessage(websocket.TextMessage, jsonData)
	if err != nil {
		log.Printf("Error sending status message: %v", err)
	}
}
