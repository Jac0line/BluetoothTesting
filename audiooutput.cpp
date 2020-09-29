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

void audioOutput::outputAudioFile(QBluetoothSocket *bluetoothSocket)
{
    QFile inputFile;
    QAudioOutput* audio;
    inputFile.setFileName("/home/jacoline/btscanner/CantinaBand3.wav");
    inputFile.open(QIODevice::ReadOnly);

    QAudioFormat format;
    format.setSampleRate(8000);
    format.setChannelCount(1);
    format.setSampleSize(8);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::UnSignedInt);

    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
    if (!info.isFormatSupported(format)) {
        qWarning()<<"wav audio format not supported by backend, cannot play audio.";
        return;
    }

    audio = new QAudioOutput(format, this);

    connect(audio,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
     audio->start(&inputFile);



////    sourceFile.setFileName("/tmp/test.raw");
////    sourceFile.open(QIODevice::ReadOnly);

////    QAudioFormat format;
////    // Set up the format, eg.
////    format.setSampleRate(8000);
////    format.setChannelCount(1);
////    format.setSampleSize(8);
////    format.setCodec("audio/pcm");
////    format.setByteOrder(QAudioFormat::LittleEndian);
////    format.setSampleType(QAudioFormat::UnSignedInt);

////    QAudioDeviceInfo info(QAudioDeviceInfo::defaultOutputDevice());
////    if (!info.isFormatSupported(format)) {
////        qWarning() << "Raw audio format not supported by backend, cannot play audio.";
////        return;
////    }

////    audio = new QAudioOutput(format, nullptr);
////    connect(audio, SIGNAL(stateChanged(QAudio::State)), this, SLOT(handleStateChanged(QAudio::State)));
////    audio->start(&sourceFile);

//    qDebug() << "opening file.mp3";
//    const QString filePath = "/home/jacoline/btscanner/CantinaBand3.wav";
//    QFile file;
//    file.setFileName(filePath);

//    //QFile file("file.mp3");
//    if (!file.exists()) {
//        qDebug() << "cannot find file";
//    }
//    QString errMsg;
//    QFileDevice::FileError err = QFileDevice::NoError;
//    if (!file.open(QIODevice::ReadOnly)) {
//        errMsg = file.errorString();
//        err = file.error();
//    }

////    bool succes;
////    succes = file.open(QIODevice::ReadOnly);
////    qDebug() << "succes: " << succes;

////    if (!file.open(QIODevice::ReadOnly)){
////        qDebug() << "cannot open file";
////        return;
////    }

//    qDebug() << "reading to qByteArray";
////    QFile fileTextStream(&file);
//            //const QByteArray content = file.readAll();

////    QFile file("file.dat");
////    file.open(QIODevice::ReadOnly);
////    QDataStream in(&file);


////    char * fileData;
////    while(sourceFile.bytesAvailable())
////    {
////        sourceFile.read(fileData,);
////    }
////    QByteArray buf;
////    QDataStream s(&buf, QIODevice::ReadWrite);

//    qDebug() << "writing content";

//    char * fileData;
//    qint64 maxSize = 1024;//sizeof(qint64);//64;//sizeof (fileData);
//    qDebug() << "maxSize: " << maxSize;
//    while(file.bytesAvailable())
//    {
//        file.read(fileData,maxSize);
//        qDebug() << "bluetoothSocket->state(): "<< bluetoothSocket->state();
////        qDebug() << "fileData" << fileData;
//        bluetoothSocket->write(fileData);
//        qDebug() << "writing...";
//    }
//    qDebug() << "File writen" ;
}
