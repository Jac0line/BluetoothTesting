#ifndef AUDIOFILESTREAM_H
#define AUDIOFILESTREAM_H

#endif // AUDIOFILESTREAM_H

#pragma once

#include <QIODevice>
#include <QBuffer>
#include <QAudioDecoder>
#include <QAudioFormat>
#include <QFile>

/**
 * @brief The AudioFileStream class Decode audio files like MP3 and push decoded audio data to QOutputDevice, based on: https://github.com/Znurre/QtMixer
 */
class AudioFileStream : public QIODevice
{
    Q_OBJECT

public:
    /**
     * @brief AudioFileStream Constructs AudioFileStream with input, ouput and state
     */
    AudioFileStream();

    ~AudioFileStream();

    /**
     * @brief init Initialize buffers and set audio format for decoding
     * @param format The audioformat that needs decoding
     * @return Succeeded initialising 
     */
    bool init(const QAudioFormat& format);

    /**
     * @brief The State enum State of the AudioFileStream
     */
    enum State { Playing, Stopped };
    
    /**
     * @brief play Opens the file and starts the decoder
     * @param filePath The path to the audio file that needs to be decoded
     */
    void play(const QString &filePath);
    
    /**
     * @brief stop Stops and clears decoder and changes state
     */
    void stop();

    /**
     * @brief atEnd Check if file decoding is finished 
     * @return True is file is fully read and decoded
     */
    bool atEnd() const override;

protected:
    qint64 readData(char* data, qint64 maxlen) override;
    qint64 writeData(const char* data, qint64 len) override;

private:
    QFile m_file;
    QBuffer m_input;
    QBuffer m_output;
    QByteArray m_data;
    QAudioDecoder m_decoder;
    QAudioFormat m_format;
    State m_state;

    bool isInited;
    bool isDecodingFinished;

    void clear();

private slots:
    void bufferReady();
    void finished();

signals:
    void stateChanged(AudioFileStream::State state);
    void newData(const QByteArray& data);
};
