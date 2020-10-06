#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QFile>
#include <QDialog>
#include <QtMultimedia/QAudioOutput>
#include <QBluetoothSocket>
#include "AudioFileStream.h"

QT_USE_NAMESPACE

/**
 * @brief The audioOutput class Create QAudioOutput and interact with AudioFileStream
 */
class audioOutput : public QObject
{
    Q_OBJECT

public:
    /**
     * @brief audioOutput Construct audioOuput
     */
    audioOutput();

    ~audioOutput();

//    void handleStateChanged(QAudio::State state);

    /**
     * @brief init initialize AudioFileStream with the QAudioOutput and the desired audio format
     * @return Return true if initialization was successfull
     */
    bool init();

    /**
     * @brief play Start to play the AudioFileStream
     */
    void play();

    /**
     * @brief stop Stop playing the AudioFileStream
     */
    void stop();

//    void playFromFile();
//    QAudioFormat audioFormat();

private:
    AudioFileStream *m_audioFileStream;
    QFile sourceFile;
    QAudioOutput *audio;


};

#endif // AUDIOOUTPUT_H
