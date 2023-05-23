#include "AudioPlayer.hpp"

extern "C" {
    #include <libswresample/swresample.h>
}

AudioPlayer& AudioPlayer::getInstance() {
    static AudioPlayer instance;
    return instance;
}

AudioPlayer::AudioPlayer() : initialized(false), abortRequest(false) {
}

AudioPlayer::~AudioPlayer() {
    cleanup();
}

void AudioPlayer::initialize(int desired_sample_rate, int desired_bits_per_sample, int desired_channels) {
    ao_initialize();
    int driver_id = ao_default_driver_id();
    ao_sample_format sample_format;
    sample_format.bits = desired_bits_per_sample;
    sample_format.channels = desired_channels;
    sample_format.rate = desired_sample_rate;
    sample_format.byte_format = AO_FMT_NATIVE;
    sample_format.matrix = 0;
    device = ao_open_live(driver_id, &sample_format, NULL);
    initialized = true;
}

void AudioPlayer::cleanup() {
    if (initialized) {
        ao_close(device);
        ao_shutdown();
        avcodec_close(codecContext);
        avformat_close_input(&formatContext);
    }
}

int AudioPlayer::playAudio(const char* filename) {
    int desired_sample_rate = 44100;  // Set your desired sample rate here
    int desired_bits_per_sample = 16;  // Set your desired bits per sample here
    int desired_channels = 2;  // Set your desired number of channels here

    this->abortRequest = false;

    if (!initialized) {
        initialize(desired_sample_rate, desired_bits_per_sample, desired_channels);
    }
    
    if (avformat_open_input(&formatContext, filename, NULL, NULL) < 0) {
        std::cerr << "avformat_open_input failed" << std::endl;
        return 1;
    }

    if (avformat_find_stream_info(formatContext, NULL) < 0) {
        avformat_close_input(&formatContext);
        std::cerr << "avformat_find_stream_info failed" << std::endl;
        return 2;
    }

    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioStreamIndex = i;
            break;
        }
    }

    codecParameters = formatContext->streams[audioStreamIndex]->codecpar;
    codec = avcodec_find_decoder(codecParameters->codec_id);
    codecContext = avcodec_alloc_context3(codec);
    avcodec_parameters_to_context(codecContext, codecParameters);

    if (avcodec_open2(codecContext, codec, NULL) < 0) {
        avformat_close_input(&formatContext);
        return 3;
    }

    SwrContext* swr = swr_alloc_set_opts(NULL,
                                        av_get_default_channel_layout(desired_channels),
                                        AV_SAMPLE_FMT_S16,
                                        desired_sample_rate,
                                        av_get_default_channel_layout(codecContext->channels),
                                        codecContext->sample_fmt,
                                        codecContext->sample_rate,
                                        0,
                                        NULL);

    if(!swr || swr_init(swr) < 0)
    {
        avcodec_close(codecContext);
        avformat_close_input(&formatContext);
        std::cerr << "swr_init failed" << std::endl;
        return 3;
    }

    AVPacket packet;
    AVFrame *frame = av_frame_alloc();

    while (av_read_frame(formatContext, &packet) >= 0) {
        if (abortRequest) {
            break;
        }
        
        if (packet.stream_index == audioStreamIndex) {
            if (avcodec_send_packet(codecContext, &packet) == 0) {
                while (avcodec_receive_frame(codecContext, frame) == 0) {
                    uint8_t* buffer = NULL;
                    int num_samples = av_rescale_rnd(swr_get_delay(swr, codecContext->sample_rate) + frame->nb_samples, desired_sample_rate, desired_sample_rate, AV_ROUND_UP);
                    av_samples_alloc(&buffer, NULL, desired_channels, num_samples, AV_SAMPLE_FMT_S16, 0);
                    num_samples = swr_convert(swr, &buffer, num_samples, (const uint8_t**)frame->data, frame->nb_samples);
                    int buffer_size = av_samples_get_buffer_size(NULL, desired_channels, num_samples, AV_SAMPLE_FMT_S16, 1);
                    ao_play(device, (char*)buffer, buffer_size);
                    if(buffer != NULL)
                        av_freep(&buffer);
                }
            }
        }
        av_packet_unref(&packet);
    }

    av_frame_free(&frame);

    if(swr != NULL)
        swr_free(&swr);

    avcodec_close(codecContext);
    avformat_close_input(&formatContext);

    return 0;
}

void AudioPlayer::abortPlayback() {
    abortRequest = true;
}
