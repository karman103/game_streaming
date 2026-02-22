import React, { useEffect, useRef, useState, useCallback } from 'react';
import Hls from 'hls.js';
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
  onStatusChange,
}) => {
  const videoRef = useRef<HTMLVideoElement>(null);
  const streamContentRef = useRef<HTMLDivElement>(null);
  const lastMouseMoveRef = useRef<number>(0);

  const [streamInfo, setStreamInfo] = useState<StreamInfo | null>(null);
  const [isConnected, setIsConnected] = useState(false);
  const [isLoading, setIsLoading] = useState(true);
  const [error, setError] = useState<string | null>(null);
  const [sessionExpired, setSessionExpired] = useState(false);
  const [gamepadConnected, setGamepadConnected] = useState(false);

  const sendInput = useCallback(async (payload: unknown) => {
    try {
      await fetch(`/api/sessions/${sessionId}/input`, {
        method: 'POST',
        headers: { 'Content-Type': 'application/json' },
        body: JSON.stringify(payload),
      });
    } catch {
      // Ignore transient input transport errors in UI.
    }
  }, [sessionId]);

  useEffect(() => {
    let cancelled = false;

    const fetchInfo = async () => {
      try {
        const response = await fetch(`/api/stream/${sessionId}/info`);
        if (!response.ok) {
          if (response.status === 404) {
            setSessionExpired(true);
            throw new Error('Session not found. Start a new game session.');
          }
          throw new Error('Failed to fetch stream info');
        }

        const info: StreamInfo = await response.json();
        if (cancelled) return;

        setStreamInfo(info);
        setIsConnected(info.status === 'ready' || info.status === 'running');
        setError(null);
        setSessionExpired(false);
        onStatusChange?.(info.status);
      } catch (err) {
        if (cancelled) return;
        const message = err instanceof Error ? err.message : 'Failed to connect to game stream';
        setError(message);
        setIsConnected(false);
        onError?.(message);
      } finally {
        if (!cancelled) {
          setIsLoading(false);
        }
      }
    };

    fetchInfo();
    const interval = window.setInterval(fetchInfo, 3000);

    return () => {
      cancelled = true;
      window.clearInterval(interval);
    };
  }, [sessionId, onError, onStatusChange]);

  useEffect(() => {
    const videoEl = videoRef.current;
    if (!videoEl || !streamInfo?.streamUrl || streamInfo.status !== 'ready') {
      return;
    }

    let hls: Hls | null = null;

    const sourceUrl = streamInfo.streamUrl;

    if (Hls.isSupported()) {
      hls = new Hls({
        lowLatencyMode: true,
        liveSyncDurationCount: 2,
        maxLiveSyncPlaybackRate: 1.5,
      });
      hls.loadSource(sourceUrl);
      hls.attachMedia(videoEl);

      hls.on(Hls.Events.MANIFEST_PARSED, () => {
        void videoEl.play().catch(() => {
          // Autoplay can be blocked by browser policies.
        });
      });

      hls.on(Hls.Events.ERROR, (_event, data) => {
        if (data.fatal) {
          switch (data.type) {
            case Hls.ErrorTypes.NETWORK_ERROR:
              hls?.startLoad();
              break;
            case Hls.ErrorTypes.MEDIA_ERROR:
              hls?.recoverMediaError();
              break;
            default:
              setError('Stream playback failed');
              onError?.('Stream playback failed');
              hls?.destroy();
              break;
          }
        }
      });
    } else if (videoEl.canPlayType('application/vnd.apple.mpegurl')) {
      videoEl.src = sourceUrl;
      void videoEl.play().catch(() => {
        // Autoplay can be blocked by browser policies.
      });
    } else {
      setError('HLS is not supported in this browser');
    }

    return () => {
      if (hls) {
        hls.destroy();
      }
    };
  }, [streamInfo, onError]);

  useEffect(() => {
    const handleKeyDown = (event: KeyboardEvent) => {
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

    return () => {
      window.removeEventListener('gamepadconnected', handleGamepadConnected);
      window.removeEventListener('gamepaddisconnected', handleGamepadDisconnected);
    };
  }, []);

  useEffect(() => {
    if (!gamepadConnected) {
      return;
    }

    const interval = window.setInterval(() => {
      const pads = navigator.getGamepads();
      for (const pad of pads) {
        if (!pad) continue;
        pad.buttons.forEach((button, index) => {
          if (button.pressed) {
            sendInput({ type: 'gamepadbutton', button: index, pressed: true });
          }
        });
        pad.axes.forEach((axis, index) => {
          if (Math.abs(axis) > 0.2) {
            sendInput({ type: 'gamepadaxis', axis: index, value: axis });
          }
        });
      }
    }, 50);

    return () => window.clearInterval(interval);
  }, [gamepadConnected, sendInput]);

  const handleMouseMove = useCallback((event: React.MouseEvent) => {
    const now = Date.now();
    if (now-lastMouseMoveRef.current < 30) {
      return;
    }
    lastMouseMoveRef.current = now;
    const rect = streamContentRef.current?.getBoundingClientRect();
    if (!rect) return;
    const x = Math.round((event.clientX - rect.left) * (1280 / rect.width));
    const y = Math.round((event.clientY - rect.top) * (720 / rect.height));
    sendInput({ type: 'mousemove', x, y });
  }, [sendInput]);

  const handleMouseDown = useCallback((event: React.MouseEvent) => {
    event.preventDefault();
    sendInput({ type: 'mousedown', button: event.button });
  }, [sendInput]);

  const handleMouseUp = useCallback((event: React.MouseEvent) => {
    event.preventDefault();
    sendInput({ type: 'mouseup', button: event.button });
  }, [sendInput]);

  const handleWheel = useCallback((event: React.WheelEvent) => {
    event.preventDefault();
    sendInput({ type: 'wheel', deltaX: event.deltaX, deltaY: event.deltaY });
  }, [sendInput]);

  if (error) {
    return (
      <div className="game-stream-error">
        <div className="error-icon">⚠️</div>
        <h3>Connection Error</h3>
        <p>{error}</p>
        {sessionExpired && (
          <button onClick={() => { window.location.href = '/'; }}>
            Back to Launcher
          </button>
        )}
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

      <div
        ref={streamContentRef}
        className="stream-content"
        onMouseMove={handleMouseMove}
        onMouseDown={handleMouseDown}
        onMouseUp={handleMouseUp}
        onWheel={handleWheel}
        tabIndex={0}
      >
        {streamInfo?.status !== 'ready' && (
          <div className="stream-waiting">Preparing video stream...</div>
        )}
        <video
          ref={videoRef}
          className="game-video"
          autoPlay
          muted
          controls
          playsInline
        />
      </div>

      <div className="stream-controls">
        <div className="control-hints">
          <div className="hint-group">
            <strong>Mode:</strong>
            <span>HLS playback enabled</span>
            <span>WebSocket streaming disabled</span>
          </div>
          <div className="hint-group">
            <strong>Status:</strong>
            <span>Session active</span>
            <span>Input forwarding enabled</span>
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
