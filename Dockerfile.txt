FROM ubuntu:jammy AS config

RUN apt-get update \
    && apt-get -y install alsa-utils ffmpeg libao4 \
    && rm -rf /var/lib/apt/lists/*

RUN echo 'default_driver=alsa\nquiet' > /etc/libao.conf
COPY ./asound.conf /etc/asound.conf

FROM config AS dev

RUN apt-get update \
    && apt-get -y install build-essential cmake git gdb pkg-config \
                libao-dev libavformat-dev libavcodec-dev libavutil-dev  \
    && rm -rf /var/lib/apt/lists/*

FROM dev AS build

ARG BUILD_TYPE=Release

COPY . /root/workspace/lineout

RUN cmake -S /root/workspace/lineout -B /root/workspace/lineout-build \
        -DCMAKE_INSTALL_PREFIX=/deploy \
        -DCMAKE_BUILD_TYPE=${BUILD_TYPE} \
    && cmake --build /root/workspace/lineout-build --target install -- -l

FROM config as deploy

COPY --from=build /deploy/ /usr/
