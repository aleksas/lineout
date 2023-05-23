# Lineout Audio Player

Lineout Audio Player is a simple C++ application that plays audio files from the command line. It leverages the FFmpeg library for reading audio files and the libao library for audio output.

The software is built to run on Linux and can be easily containerized for deployment using Docker. The Docker image created from the provided Dockerfile includes all necessary dependencies for running the audio player.

## Table of Contents

- [Getting Started](#getting-started)
- [Building and Running](#building-and-running)
- [Running with Docker](#running-with-docker)
- [Running with Docker Compose](#running-with-docker-compose)
- [License](#license)

## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

- C++20 compiler (e.g., g++ or clang++)
- cmake (version 3.1 or higher)
- libao (audio output library)
- FFmpeg (libavformat, libavcodec, libavutil)

You can install these dependencies on Ubuntu with:

```bash
sudo apt-get install build-essential cmake libao-dev libavformat-dev libavcodec-dev libavutil-dev
```

## Building and Running

The project uses a CMake build system. After you've installed the prerequisites, you can build and run the program with:

```bash
mkdir build
cd build
cmake ..
make
./lineout-player <audiofile>
```

Replace `<audiofile>` with the path to the audio file you wish to play.

## Running with Docker

You can also run the application using Docker. First, build the Docker image:

```bash
docker build -t lineout:latest .
```

Then, run the application:

```bash
docker run -it --rm lineout lineout-player <audiofile>
```

Please note that running audio applications inside Docker might require specific device permissions and configurations, due to Docker's isolation from host audio devices.

## Running with Docker Compose

The repository includes a `docker-compose.yml` file for running the application with Docker Compose. To use it, first ensure you have Docker Compose installed. Then, run:

```bash
docker-compose up
```

This will build the Docker image (if not built) and run the service. By default, the service will play the file `/usr/share/sounds/alsa/Front_Center.wav`.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Disclaimer

This code was generated with the help of Chat GPT 4
