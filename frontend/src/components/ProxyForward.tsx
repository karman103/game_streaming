import React from "react";

const MarioGame: React.FC = () => {
  return (
    <div style={{ width: "100%", height: "100vh", overflow: "hidden" }}>
      <iframe
        src="http://localhost:3000/stream/mario"
        title="Super Mario"
        style={{
          width: "100%",
          height: "100%",
          border: "none",
        }}
      />
    </div>
  );
};

export default MarioGame;
