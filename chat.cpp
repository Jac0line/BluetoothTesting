#include "chat.h"
#include "remoteselector.h"
#include "audiooutput.h"

#include <QtCore/qdebug.h>
#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QTime>

Chat::Chat(QWidget *parent)
    : QDialog(parent), ui(new Ui_Chat)
{
    ui->setupUi(this);

    connect(ui->quitButton, &QPushButton::clicked, this, &Chat::quitClicked);
    connect(ui->connectButton, &QPushButton::clicked, this, &Chat::connectClicked);

    localName = QBluetoothLocalDevice().name();
}

Chat::~Chat()
{
    qDeleteAll(clients);
}

void Chat::clientDisconnected()
{
    Chat *client = qobject_cast<Chat *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}

void Chat::quitClicked()
{
    socket->disconnectFromService();
    socket->close();
}

void Chat::connectClicked()
{
    ui->connectButton->setEnabled(false);

    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(currentAdapterIndex).address();
//    const QBluetoothAddress adapter = *new QBluetoothAddress(*new QString("08:BE:AC:11:F1:87"));

    RemoteSelector remoteSelector(adapter);
    remoteSelector.startDiscovery(uuid);
    if (remoteSelector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remoteSelector.service();
        service.setServiceUuid(uuid);

        qDebug() << "Connecting to service" << service.serviceName()
                 << "on" << service.device().name();

        // Create client
        qDebug() << "Going to create client";
        Chat *client = new Chat(this);
        qDebug() << "Connecting...";

        connect(client, &Chat::disconnected,
                this, QOverload<>::of(&Chat::clientDisconnected));
        qDebug() << "Start client";
        client->startClient(service, adapter);

        clients.append(client);
    }
    ui->connectButton->setEnabled(true);
}

void Chat::startClient(const QBluetoothServiceInfo &remoteService, const QBluetoothAddress localDevice)
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

    connect(socket, &QBluetoothSocket::readyRead, this, &Chat::readSocket);
    connect(socket, &QBluetoothSocket::connected, this, QOverload<>::of(&Chat::connected));
    connect(socket, &QBluetoothSocket::disconnected, this, &Chat::disconnected);
    connect(socket, QOverload<QBluetoothSocket::SocketError>::of(&QBluetoothSocket::error),
            this, &Chat::onSocketErrorOccurred);

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

void Chat::stopClient()
{
    delete socket;
    socket = nullptr;
}

void Chat::readSocket()
{
    if (!socket)
        return;

    while (socket->canReadLine()) {
        QByteArray line = socket->readLine();
        emit messageReceived(socket->peerName(),
                             QString::fromUtf8(line.constData(), line.length()));
    }
}

void Chat::onSocketErrorOccurred(QBluetoothSocket::SocketError error)
{
    if (error == QBluetoothSocket::NoSocketError)
        return;

    QMetaEnum metaEnum = QMetaEnum::fromType<QBluetoothSocket::SocketError>();
    QString errorString = socket->peerName() + QLatin1Char(' ')
            + metaEnum.valueToKey(error) + QLatin1String(" occurred");

    emit socketErrorOccurred(errorString);
}

void Chat::connected()
{
    qDebug() << "Connected";
    emit connected(socket->peerName());
}

