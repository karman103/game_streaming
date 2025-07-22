import React, { useEffect, useRef } from "react";
import { useNavigate } from "react-router-dom";

const GameStream: React.FC = () => {
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const img = useRef(new Image());
  const wsRef = useRef<WebSocket | null>(null); // Track WebSocket
  const navigate = useNavigate();

  useEffect(() => {
    const ws = new WebSocket("ws://localhost:3000/ws/stream");
    ws.binaryType = "arraybuffer";
    wsRef.current = ws;

    const ctx = canvasRef.current?.getContext("2d");

    ws.onopen = () => console.log("WebSocket connected");
    ws.onclose = () => console.log("WebSocket disconnected");
    ws.onerror = (err) => console.error("WebSocket error:", err);

    ws.onmessage = (event) => {
      const blob = new Blob([event.data], { type: "image/jpeg" });
      const url = URL.createObjectURL(blob);

      img.current.src = url;

      img.current.onload = () => {
        ctx?.clearRect(0, 0, canvasRef.current!.width, canvasRef.current!.height);
        ctx?.drawImage(img.current, 0, 0, canvasRef.current!.width, canvasRef.current!.height);
        URL.revokeObjectURL(url); // Cleanup
      };
    };

    return () => {
      ws.close(); // Clean up WebSocket on unmount
    };
  }, []);

  const handleStopStream = () => {
    console.log("Stopping stream...");
    wsRef.current?.close(); // Close WebSocket
    navigate("/");          // Go back to home page
  };

  return (
    <div>
      <h1>🎮 Live Game Stream</h1>
      <canvas ref={canvasRef} width={1280} height={720}></canvas>
      <div style={{ marginTop: "1rem" }}>
        <button
          onClick={handleStopStream}
          style={{
            padding: "10px 20px",
            backgroundColor: "red",
            color: "white",
            border: "none",
            borderRadius: "5px",
            cursor: "pointer",
          }}
        >
          🛑 Stop Stream
        </button>
      </div>
    </div>
  );
};

export default GameStream;
