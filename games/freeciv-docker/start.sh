#!/bin/sh
set -e
export DISPLAY=:99
export PATH="/usr/games:$PATH"
RTP_HOST="${RTP_HOST:-host.docker.internal}"
RTP_PORT="${RTP_PORT:-5004}"
mkdir -p /tmp/saves

# start virtual display (lower res to reduce encoding cost)
Xvfb :99 -screen 0 960x540x24 >/tmp/xvfb.log 2>&1 &

# start minimal window manager
fluxbox >/tmp/fluxbox.log 2>&1 &

# start server
freeciv-server -p 5556 --saves /tmp/saves >/tmp/freeciv-server.log 2>&1 &

# wait server up
sleep 2

# start client connecting to local server
freeciv-gtk3.22 --server localhost --port 5556 --name player >/tmp/freeciv-client.log 2>&1 &

# start ffmpeg capture and publish RTP for WebRTC ingest on the host
ffmpeg -hide_banner -loglevel error \
  -f x11grab -video_size 960x540 -framerate 20 -i :99.0 \
  -use_wallclock_as_timestamps 1 -fflags nobuffer -flags low_delay \
  -probesize 32 -analyzeduration 0 -flush_packets 1 \
  -an -c:v libx264 -preset ultrafast -tune zerolatency -pix_fmt yuv420p \
  -threads 2 -x264-params "scenecut=0:rc-lookahead=0:sync-lookahead=0" \
  -profile:v baseline -level 3.1 \
  -g 20 -keyint_min 20 -sc_threshold 0 \
  -b:v 1200k -maxrate 1400k -bufsize 600k \
  -payload_type 96 -f rtp "rtp://${RTP_HOST}:${RTP_PORT}?pkt_size=1200"
