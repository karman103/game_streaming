# Base image
FROM ubuntu:24.04

# Install build tools and Freeciv dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    python3 python3-pip \
    ninja-build \
    meson \
    git \
    wget \
    pkg-config \
    libgtk-3-dev \
    libsdl2-dev \
    libsdl2-mixer-dev \        
    libcurl4-openssl-dev \
    libicu-dev \
    libsqlite3-dev \
    zlib1g-dev \
    libbz2-dev \
    liblzma-dev \
    libreadline-dev \
    && rm -rf /var/lib/apt/lists/*

# Clone Freeciv
WORKDIR /opt
RUN git clone https://github.com/freeciv/freeciv.git

# Build Freeciv
WORKDIR /opt/freeciv
RUN meson setup builddir --prefix=/usr \
    && ninja -C builddir \
    && ninja -C builddir install

# Expose server ports
EXPOSE 5556 5557

# Start Freeciv server by default
CMD ["freeciv-server", "--no-stdin"]
