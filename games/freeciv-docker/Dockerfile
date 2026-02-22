FROM debian:stable-slim

# Install Freeciv client+server and streaming deps
RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get install -y \
    freeciv-client-gtk3 freeciv-server \
    xvfb fluxbox ffmpeg xdotool \
    fonts-dejavu-core ca-certificates && \
    rm -rf /var/lib/apt/lists/*

# Create unprivileged user
RUN useradd -m freeciv
USER freeciv
WORKDIR /home/freeciv

# Startup script: launch server, client to connect localhost, capture via ffmpeg to /tmp/streams
COPY --chown=freeciv:freeciv start.sh /home/freeciv/start.sh
RUN chmod +x /home/freeciv/start.sh

EXPOSE 5556 5557
CMD ["/home/freeciv/start.sh"]
