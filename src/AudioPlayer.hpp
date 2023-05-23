#ifndef __AUDIO_PLAYER_HPP__
#define __AUDIO_PLAYER_HPP__

#include <iostream>

extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <ao/ao.h>
}

class AudioPlayer {
public:
    static AudioPlayer& getInstance();
    int playAudio(const char* filename);

    void abortPlayback();

private:
    AudioPlayer();
    ~AudioPlayer();
    AudioPlayer(const AudioPlayer&) = delete;
    AudioPlayer& operator=(const AudioPlayer&) = delete;
    void initialize(int desired_sample_rate, int desired_bits_per_sample, int desired_channels);
    void cleanup();

    AVFormatContext *formatContext;
    AVCodecContext *codecContext;
    AVCodecParameters *codecParameters;
    AVCodec *codec;
    ao_device *device;
    int audioStreamIndex;

    bool initialized;
    bool abortRequest;
};

#endif //__AUDIO_PLAYER_HPP__
