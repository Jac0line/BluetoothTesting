#include "chatclient.h"
#include "audiooutput.h"

#include <QtCore/qmetaobject.h>
#include <QtBluetooth/qbluetoothservicediscoveryagent.h>
#include <QTime>
#include <QCoreApplication>

ChatClient::ChatClient(QObject *parent)
    :   QObject(parent)
{
}

ChatClient::~ChatClient()
{
    stopClient();
}

void ChatClient::startClient(const QBluetoothServiceInfo &remoteService, const QBluetoothAddress localDevice)
{
    if (socket)
        return;

    // Connect to service
    socket = new QBluetoothSocket(QBluetoothServiceInfo::RfcommProtocol);

    qDebug() << "Create socket";
    qDebug() << remoteService.serviceUuid();
    qDebug() << remoteService.serviceName();
    qDebug() << remoteService.socketProtocol();
    qDebug() << remoteService.serviceProvider();
    qDebug() << remoteService.serviceAvailability();
    qDebug() << remoteService.device().name();
    qDebug() << remoteService.device().address();
    qDebug() << localDevice;

    socket->connectToService(remoteService,QIODevice::ReadOnly);
    qDebug() << "ConnectToService done";

    QBluetoothSocket::SocketError error = socket->error();
    qDebug() << "SocketError: " << error;
    QString stringError = socket->errorString();
    qDebug() << "SocketErrorString: " << error;
    qDebug() << "openMode: " << socket->openMode();

    connect(socket, &QBluetoothSocket::readyRead, this, &ChatClient::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&ChatClient::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &ChatClient::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &ChatClient::onSocketErrorOccurred);

    //wait 5 sec to give time to connect socket
    QTime dieTime = QTime::currentTime().addMSecs(5000);
    while (QTime::currentTime() < dieTime) {
        QCoreApplication::processEvents( QEventLoop::AllEvents, 100);
    }

    qDebug() << "socketState: " << socket->state();

    if(socket->state() == QBluetoothSocket::ConnectedState || socket->state() == QBluetoothSocket::ConnectingState)
    {
        audioOutput audioOutputInst;
        audioOutputInst.init();
        audioOutputInst.play();
    }
    else{
        qDebug() << "Bluetooth socket not connected or connecting state";
    }
}

