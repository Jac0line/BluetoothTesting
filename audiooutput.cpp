#include "audiooutput.h"

#include <QDebug>

audioOutput::audioOutput()
{

}

audioOutput::~audioOutput()
{

}

bool audioOutput::init()
{
    QAudioDeviceInfo device = QAudioDeviceInfo::defaultOutputDevice();
    qDebug() << "device: "<< device.deviceName();
    qDebug() << "defaultOutputDevice: "<< device.defaultOutputDevice().deviceName();
    QList<QAudioDeviceInfo> availableDevices = device.availableDevices(QAudio::AudioOutput);
    qDebug() << "available devices: ";
    for (const QAudioDeviceInfo &deviceInfo : availableDevices)
        qDebug() << "Device name: " << deviceInfo.deviceName();

    QAudioFormat desired_audio_format = device.preferredFormat();
    qDebug() << "desire_audio_romat: " << desired_audio_format;

    m_audioFileStream = new AudioFileStream;
    if (!m_audioFileStream->init(desired_audio_format))
    {
        return false;
    }
    QAudioOutput* m_audioOutput = new QAudioOutput(desired_audio_format);
    qreal volume = 0.2;
    m_audioOutput->setVolume(volume);
    m_audioOutput->start(m_audioFileStream);
    return true;
}

void audioOutput::play()
{

//    m_audioFileStream->play("/home/jacoline/CantinaBand3.wav");
    m_audioFileStream->play("/home/jacoline/Music/01 Mr. Blue Sky.mp3");
}

void audioOutput::stop()
{
    m_audioFileStream->stop();
}
