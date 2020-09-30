#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QFile>
#include <QDialog>
#include <QtMultimedia/QAudioOutput>
#include <QBluetoothSocket>
#include "AudioFileStream.h"

QT_USE_NAMESPACE

class audioOutput : public QObject
{
    Q_OBJECT

public:
    audioOutput();
    ~audioOutput();
    void handleStateChanged(QAudio::State state);

    bool init();
    void play();
    void stop();
    void playFromFile();
    QAudioFormat audioFormat();

private:
    AudioFileStream *m_audioFileStream;
    QFile sourceFile;
    QAudioOutput *audio;


};

#endif // AUDIOOUTPUT_H
