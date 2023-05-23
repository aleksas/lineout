#include <iostream>
#include "AudioPlayer.hpp"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        std::cout << "Please provide files to play." << std::endl;
        return 1;
    }

    auto& audioPlayer = AudioPlayer::getInstance();
    for (int i = 1; i < argc; i++) {
        audioPlayer.playAudio(argv[i]);
    }

    return 0;
}
