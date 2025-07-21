import React, { useEffect, useRef, useState } from "react";

const ImageStream: React.FC = () => {
  const [imageSrc, setImageSrc] = useState<string>("");

  useEffect(() => {
    const ws = new WebSocket("ws://localhost:3000/ws/image");
    ws.binaryType = "arraybuffer"; // expect binary data

    ws.onmessage = (event) => {
      const blob = new Blob([event.data], { type: "image/png" });
      const url = URL.createObjectURL(blob);
      setImageSrc(url);

      // Optional: cleanup URLs to avoid memory leaks
      setTimeout(() => URL.revokeObjectURL(url), 100);
    };

    ws.onopen = () => console.log("WebSocket connected");
    ws.onclose = () => console.log("WebSocket disconnected");
    ws.onerror = (err) => console.error("WebSocket error:", err);

    return () => ws.close();
  }, []);

  return (
    <div>
      <h1>Live Image Stream</h1>
      {imageSrc && <img src={imageSrc} alt="Streaming" />}
    </div>
  );
};

export default ImageStream;
