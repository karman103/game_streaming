#!/bin/sh
set -e
STREAM_DIR=/tmp/streams
mkdir -p "$STREAM_DIR"
export DISPLAY=:99
export PATH="/usr/games:$PATH"
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

# start ffmpeg capture to HLS
ffmpeg -hide_banner -loglevel error \
  -f x11grab -video_size 960x540 -framerate 15 -i :99.0 \
  -use_wallclock_as_timestamps 1 -fflags nobuffer -flags low_delay \
  -probesize 32 -analyzeduration 0 -flush_packets 1 \
  -an -c:v libx264 -preset ultrafast -tune zerolatency -pix_fmt yuv420p \
  -threads 2 -x264-params "scenecut=0:rc-lookahead=0:sync-lookahead=0" \
  -g 2 -keyint_min 2 -sc_threshold 0 \
  -b:v 900k -maxrate 1000k -bufsize 300k \
  -f hls -hls_time 0.1 -hls_list_size 60 -hls_flags append_list+independent_segments+program_date_time \
  -hls_segment_filename "$STREAM_DIR/segment_%03d.ts" "$STREAM_DIR/playlist.m3u8"
