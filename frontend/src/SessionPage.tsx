import React from 'react';
import { useParams } from 'react-router-dom';

const SessionPage: React.FC = () => {
  const { sessionId } = useParams<{ sessionId: string }>();

  return (
    <div className="h-screen bg-black flex items-center justify-center">
      <iframe
        src={`https://your-stream-server.com/session/${sessionId}`}
        className="w-full h-full border-none"
        allow="fullscreen; autoplay; clipboard-write"
      />
    </div>
  );
};

export default SessionPage;
