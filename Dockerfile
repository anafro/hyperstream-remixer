FROM silkeh/clang:21 AS build
RUN apt-get update && apt-get -y install \
    g++ \
    libc++-16-dev \
    libc++abi-16-dev \
    git \
    curl \
    zip \
    cmake \
    pkg-config \
    make \
    ninja-build \
    libxinerama-dev \
    libxcursor-dev \
    xorg-dev \
    libglu1-mesa-dev \
    autoconf \
    autoconf-archive \
    automake \
    libtool

WORKDIR /app
COPY tools/ tools/
RUN "./tools/vcpkg/bootstrap-vcpkg.sh"
COPY vcpkg.json vcpkg-configuration.json ./
RUN ./tools/vcpkg/vcpkg install

COPY ./CMakeLists.txt .
COPY src src
ENV CC=clang
ENV CXX=clang++
RUN cmake -B build -S . \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_TOOLCHAIN_FILE=tools/vcpkg/scripts/buildsystems/vcpkg.cmake \
    -DVCPKG_TARGET_TRIPLET=x64-linux && \
    cmake --build build

FROM debian:bookworm-20260202-slim AS run
WORKDIR /app
COPY --from=build /app/build/HyperstreamRemixer .
RUN chmod +x ./HyperstreamRemixer
ENV SDL_AUDIODRIVER=dummy
ENTRYPOINT [ "./HyperstreamRemixer" ]
