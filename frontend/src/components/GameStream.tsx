import React, { useEffect, useRef, useState, useCallback } from 'react';
import './GameStream.css';

interface GameStreamProps {
  sessionId: string;
  onError?: (error: string) => void;
  onStatusChange?: (status: string) => void;
}

interface StreamInfo {
  sessionId: string;
  streamUrl: string;
  status: string;
  quality: string;
  resolution: string;
  fps: number;
  bitrate: number;
}

const GameStream: React.FC<GameStreamProps> = ({ 
  sessionId, 
  onError, 
  onStatusChange 
}) => {
  const videoRef = useRef<HTMLVideoElement>(null);
  const canvasRef = useRef<HTMLCanvasElement>(null);
  const wsRef = useRef<WebSocket | null>(null);
  const inputWsRef = useRef<WebSocket | null>(null);
  const statusWsRef = useRef<WebSocket | null>(null);
  
  const [streamInfo, setStreamInfo] = useState<StreamInfo | null>(null);
  const [isConnected, setIsConnected] = useState(false);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [gamepadConnected, setGamepadConnected] = useState(false);

  // Initialize WebSocket connections
  useEffect(() => {
    const protocol = window.location.protocol === 'https:' ? 'wss:' : 'ws:';
    const baseUrl = `${protocol}//${window.location.host}`;

    // Stream WebSocket
    const streamWs = new WebSocket(`${baseUrl}/ws/stream/${sessionId}`);
    wsRef.current = streamWs;

    // Input WebSocket
    const inputWs = new WebSocket(`${baseUrl}/ws/input/${sessionId}`);
    inputWsRef.current = inputWs;

    // Status WebSocket
    const statusWs = new WebSocket(`${baseUrl}/ws/status/${sessionId}`);
    statusWsRef.current = statusWs;

    // Stream WebSocket handlers
    streamWs.onopen = () => {
      console.log('Stream WebSocket connected');
      setIsConnected(true);
      setIsLoading(false);
    };

    streamWs.onmessage = (event) => {
      if (event.data instanceof Blob) {
        // Handle binary video data
        const url = URL.createObjectURL(event.data);
        if (videoRef.current) {
          videoRef.current.src = url;
        }
      } else {
        // Handle stream info
        try {
          const info: StreamInfo = JSON.parse(event.data);
          setStreamInfo(info);
        } catch (e) {
          console.error('Error parsing stream info:', e);
        }
      }
    };

    streamWs.onerror = (error) => {
      console.error('Stream WebSocket error:', error);
      setError('Failed to connect to game stream');
      onError?.('Failed to connect to game stream');
    };

    // Input WebSocket handlers
    inputWs.onopen = () => {
      console.log('Input WebSocket connected');
    };

    inputWs.onerror = (error) => {
      console.error('Input WebSocket error:', error);
    };

    // Status WebSocket handlers
    statusWs.onopen = () => {
      console.log('Status WebSocket connected');
    };

    statusWs.onmessage = (event) => {
      try {
        const status = JSON.parse(event.data);
        onStatusChange?.(status.type);
      } catch (e) {
        console.error('Error parsing status:', e);
      }
    };

    statusWs.onerror = (error) => {
      console.error('Status WebSocket error:', error);
    };

    // Cleanup
    return () => {
      streamWs.close();
      inputWs.close();
      statusWs.close();
    };
  }, [sessionId, onError, onStatusChange]);

  // Input handling
  const sendInput = useCallback((input: any) => {
    if (inputWsRef.current?.readyState === WebSocket.OPEN) {
      inputWsRef.current.send(JSON.stringify(input));
    }
  }, []);

  // Keyboard event handlers
  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
      event.preventDefault();
      sendInput({
        type: 'keydown',
        key: event.key,
        code: event.code,
        ctrlKey: event.ctrlKey,
        shiftKey: event.shiftKey,
        altKey: event.altKey,
        metaKey: event.metaKey,
      });
    };

    const handleKeyUp = (event: KeyboardEvent) => {
      event.preventDefault();
      sendInput({
        type: 'keyup',
        key: event.key,
        code: event.code,
        ctrlKey: event.ctrlKey,
        shiftKey: event.shiftKey,
        altKey: event.altKey,
        metaKey: event.metaKey,
      });
    };

    window.addEventListener('keydown', handleKeyDown);
    window.addEventListener('keyup', handleKeyUp);

    return () => {
      window.removeEventListener('keydown', handleKeyDown);
      window.removeEventListener('keyup', handleKeyUp);
    };
  }, [sendInput]);

  // Mouse event handlers
  const handleMouseMove = useCallback((event: React.MouseEvent) => {
    const rect = canvasRef.current?.getBoundingClientRect();
    if (rect) {
      sendInput({
        type: 'mousemove',
        x: Math.round((event.clientX - rect.left) * (1280 / rect.width)),
        y: Math.round((event.clientY - rect.top) * (720 / rect.height)),
      });
    }
  }, [sendInput]);

  const handleMouseDown = useCallback((event: React.MouseEvent) => {
    event.preventDefault();
    sendInput({
      type: 'mousedown',
      button: event.button,
      x: event.clientX,
      y: event.clientY,
    });
  }, [sendInput]);

  const handleMouseUp = useCallback((event: React.MouseEvent) => {
    event.preventDefault();
    sendInput({
      type: 'mouseup',
      button: event.button,
      x: event.clientX,
      y: event.clientY,
    });
  }, [sendInput]);

  const handleWheel = useCallback((event: React.WheelEvent) => {
    event.preventDefault();
    sendInput({
      type: 'wheel',
      deltaX: event.deltaX,
      deltaY: event.deltaY,
    });
  }, [sendInput]);

  // Gamepad support
  useEffect(() => {
    const handleGamepadConnected = (event: GamepadEvent) => {
      console.log('Gamepad connected:', event.gamepad.id);
      setGamepadConnected(true);
    };

    const handleGamepadDisconnected = (event: GamepadEvent) => {
      console.log('Gamepad disconnected:', event.gamepad.id);
      setGamepadConnected(false);
    };

    window.addEventListener('gamepadconnected', handleGamepadConnected);
    window.addEventListener('gamepaddisconnected', handleGamepadDisconnected);

    // Gamepad polling
    const pollGamepads = () => {
      const gamepads = navigator.getGamepads();
      for (const gamepad of gamepads) {
        if (gamepad) {
          // Check buttons
          gamepad.buttons.forEach((button, index) => {
            if (button.pressed) {
              sendInput({
                type: 'gamepadbutton',
                button: index,
                pressed: true,
              });
            }
          });

          // Check axes
          gamepad.axes.forEach((axis, index) => {
            if (Math.abs(axis) > 0.1) {
              sendInput({
                type: 'gamepadaxis',
                axis: index,
                value: axis,
              });
            }
          });
        }
      }
    };

    const gamepadInterval = setInterval(pollGamepads, 16); // ~60 FPS

    return () => {
      window.removeEventListener('gamepadconnected', handleGamepadConnected);
      window.removeEventListener('gamepaddisconnected', handleGamepadDisconnected);
      clearInterval(gamepadInterval);
    };
  }, [sendInput]);

  if (error) {
    return (
      <div className="game-stream-error">
        <div className="error-icon">⚠️</div>
        <h3>Connection Error</h3>
        <p>{error}</p>
        <button onClick={() => window.location.reload()}>
          Retry Connection
        </button>
      </div>
    );
  }

  if (isLoading) {
    return (
      <div className="game-stream-loading">
        <div className="loading-spinner"></div>
        <p>Connecting to game stream...</p>
      </div>
    );
  }

  return (
    <div className="game-stream-container">
      <div className="stream-header">
        <div className="stream-info">
          {streamInfo && (
            <>
              <span className="quality-badge">{streamInfo.quality}</span>
              <span className="fps-info">{streamInfo.fps} FPS</span>
              <span className="resolution-info">{streamInfo.resolution}</span>
            </>
          )}
        </div>
        <div className="connection-status">
          <div className={`status-indicator ${isConnected ? 'connected' : 'disconnected'}`}></div>
          <span>{isConnected ? 'Connected' : 'Disconnected'}</span>
          {gamepadConnected && <span className="gamepad-indicator">🎮</span>}
        </div>
      </div>

      <div className="stream-content">
        <canvas
          ref={canvasRef}
          className="game-canvas"
          width={1280}
          height={720}
          onMouseMove={handleMouseMove}
          onMouseDown={handleMouseDown}
          onMouseUp={handleMouseUp}
          onWheel={handleWheel}
          tabIndex={0}
        />
        <video
          ref={videoRef}
          className="game-video"
          autoPlay
          muted
          playsInline
          style={{ display: 'none' }}
        />
      </div>

      <div className="stream-controls">
        <div className="control-hints">
          <div className="hint-group">
            <strong>Keyboard:</strong>
            <span>WASD - Move</span>
            <span>Space - Jump</span>
            <span>Shift - Sneak</span>
          </div>
          <div className="hint-group">
            <strong>Mouse:</strong>
            <span>Click - Interact</span>
            <span>Scroll - Zoom</span>
          </div>
          {gamepadConnected && (
            <div className="hint-group">
              <strong>Gamepad:</strong>
              <span>Connected</span>
            </div>
          )}
        </div>
      </div>
    </div>
  );
};

export default GameStream;
