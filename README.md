![docker workflow](https://github.com/aleksas/lineout/actions/workflows/docker-image.yml/badge.svg)

# Lineout Player

Lineout Player is an audio player for the command line, built with C++ and utilizing FFmpeg and libao for audio decoding and output. It is meant to be run in an Odroid C4 environment, specifically accessing the `/dev/snd/controlC1` sound device. 

Please note that the device path and cgroup rules may differ based on your specific hardware and system configuration.

## Prerequisites

Ensure that you have the following installed:

- Docker
- Docker Compose

## Build and Run

### Building the Docker Image

1. Clone this repository to your local machine.

2. Build the Docker image by running:

    ```bash
    docker-compose build
    ```

### Running the Lineout Player

1. Ensure your Odroid C4 device is connected and the `/dev/snd/controlC1` sound device is accessible.

2. Run the player using Docker Compose:

    ```bash
    docker-compose up
    ```

3. You should hear the Front_Center.wav sound file played through your Odroid device's speakers.

## Code Files

The application code is found in the following files:

- `main.cpp`: The entry point for the application.
- `AudioPlayer.hpp`: Header file for the AudioPlayer class, which handles audio playback.
- `AudioPlayer.cpp`: Implementation of the AudioPlayer class.

The project uses a `CMakeLists.txt` file for building the application.

## Configuration Files

The application also uses several configuration files:

- `Dockerfile`: Used for creating the Docker image.
- `asound.conf`: ALSA sound configuration file.
- `docker-compose.yml`: Docker Compose file for orchestrating the application.

## Important Note

The Lineout Player is set up for the Odroid C4 device, specifically accessing the `/dev/snd/controlC1` sound device. If you are using a different device, you may need to adjust the `asound.conf` and `docker-compose.yml` files.

For example, the `docker-compose.yml` includes the following device_cgroup_rules, which may need to be adjusted:

```yaml
device_cgroup_rules:
  - 'c 116:* rw'
```

## Contributions

Please submit issues and pull requests for anything you might want to contribute or discuss.

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details.

## Disclaimer

This code was generated with the help of Chat GPT 4
