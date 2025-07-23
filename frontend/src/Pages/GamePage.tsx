import React, { useEffect, useRef } from "react";
import { useParams } from "react-router-dom";
import ImageStream from "../components/ImageStream";
import GameStream from "../components/Stream";
import MarioGame from "../components/ProxyForward";
const GamePage: React.FC = () => {
  const { gameId } = useParams();
  const canvasRef = useRef<HTMLCanvasElement>(null);

  useEffect(() => {
    if (!gameId) return;

    const ws = new WebSocket(`ws://localhost:3000/ws/stream/${gameId}`);
    const ctx = canvasRef.current?.getContext("2d");
    const img = new Image();

    ws.binaryType = "arraybuffer"; // Expect binary data

    ws.onmessage = (event) => {
      const blob = new Blob([event.data], { type: "image/png" });
      const url = URL.createObjectURL(blob);

      img.onload = () => {
        ctx?.drawImage(img, 0, 0, canvasRef.current!.width, canvasRef.current!.height);
        URL.revokeObjectURL(url); // Clean up
      };

      img.src = url;
    };

    ws.onerror = (err) => {
      console.error("WebSocket error:", err);
    };

    ws.onclose = () => {
      console.log("WebSocket closed");
    };
    return () => {
      ws.close();
    };
  }, [gameId]);

  return (
    <div>
      <h1>Game Session: {gameId}</h1>
      {/* <ImageStream /> */}
      {/* <GameStream/> */}
      <MarioGame />
      {/* <canvas
        ref={canvasRef}
        width={640}
        height={480}
        style={{ border: "1px solid black" }}
      ></canvas> */}
    </div>
  );
};

export default GamePage;
