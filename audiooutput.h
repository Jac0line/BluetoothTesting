#ifndef AUDIOOUTPUT_H
#define AUDIOOUTPUT_H

#include <QFile>
#include <QDialog>
#include <QtMultimedia/QAudioOutput>
#include <QBluetoothSocket>
#include "AudioFileStream.h"


class audioOutput : public QObject
{
public:
    audioOutput();
    ~audioOutput();
    bool init();
    void play();
    void stop();
    void outputAudioFile(QBluetoothSocket *bluetoothSocket);

//public slots:

//private slots:

private:
    AudioFileStream *m_audioFileStream;
    QFile sourceFile;
    QAudioOutput *audio;

};

#endif // AUDIOOUTPUT_H
