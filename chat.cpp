#include "chat.h"
#include "remoteselector.h"
#include "chatserver.h"
#include "chatclient.h"

#include <QtCore/qdebug.h>

#include <QtBluetooth/qbluetoothdeviceinfo.h>
#include <QtBluetooth/qbluetoothlocaldevice.h>
#include <QtBluetooth/qbluetoothuuid.h>


static const QBluetoothUuid::ServiceClassUuid uuid = QBluetoothUuid::AudioSink;

//static const QLatin1String serviceUuid("e8e10f95-1a70-4b27-9ccf-02010264e9c8");

Chat::Chat(QWidget *parent)
    : QDialog(parent), ui(new Ui_Chat)
{
    ui->setupUi(this);

    connect(ui->quitButton, &QPushButton::clicked, this, &Chat::accept);
    connect(ui->connectButton, &QPushButton::clicked, this, &Chat::connectClicked);

    localName = QBluetoothLocalDevice().name();
}

Chat::~Chat()
{
    qDeleteAll(clients);
    delete server;
}

void Chat::clientDisconnected()
{
    ChatClient *client = qobject_cast<ChatClient *>(sender());
    if (client) {
        clients.removeOne(client);
        client->deleteLater();
    }
}

void Chat::connectClicked()
{
    ui->connectButton->setEnabled(false);

    // scan for services
    const QBluetoothAddress adapter = localAdapters.isEmpty() ?
                                           QBluetoothAddress() :
                                           localAdapters.at(currentAdapterIndex).address();
//    const QBluetoothAddress adapter = *new QBluetoothAddress(*new QString("08:BE:AC:11:F1:87"));

    RemoteSelector remoteSelector(adapter);
    remoteSelector.startDiscovery(uuid);
    if (remoteSelector.exec() == QDialog::Accepted) {
        QBluetoothServiceInfo service = remoteSelector.service();
        service.setServiceUuid(uuid);

        qDebug() << "Connecting to service 2" << service.serviceName()
                 << "on" << service.device().name();

        // Create client
        qDebug() << "Going to create client";
        ChatClient *client = new ChatClient(this);
qDebug() << "Connecting...";

        connect(client, &ChatClient::disconnected,
                this, QOverload<>::of(&Chat::clientDisconnected));
        connect(this, &Chat::sendMessage, client, &ChatClient::sendMessage);
qDebug() << "Start client";
        client->startClient(service, adapter);

        clients.append(client);
    }
    ui->connectButton->setEnabled(true);
}
