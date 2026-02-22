#!/bin/sh
set -e
STREAM_DIR=/tmp/streams
mkdir -p "$STREAM_DIR"
export DISPLAY=:99
export PATH="/usr/games:$PATH"
mkdir -p /tmp/saves

# start virtual display
Xvfb :99 -screen 0 1280x720x24 >/tmp/xvfb.log 2>&1 &

# start minimal window manager
fluxbox >/tmp/fluxbox.log 2>&1 &

# start server
freeciv-server -p 5556 --saves /tmp/saves >/tmp/freeciv-server.log 2>&1 &

# wait server up
sleep 2

# start client connecting to local server
freeciv-gtk3.22 --server localhost --port 5556 --name player >/tmp/freeciv-client.log 2>&1 &

# start ffmpeg capture to HLS
ffmpeg -hide_banner -loglevel error \
  -f x11grab -video_size 1280x720 -framerate 30 -i :99.0 \
  -an -c:v libx264 -preset ultrafast -tune zerolatency -pix_fmt yuv420p \
  -g 60 -keyint_min 60 -sc_threshold 0 \
  -f hls -hls_time 1 -hls_list_size 0 -hls_flags append_list+independent_segments \
  -hls_segment_filename "$STREAM_DIR/segment_%03d.ts" "$STREAM_DIR/playlist.m3u8"
