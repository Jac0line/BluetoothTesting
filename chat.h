#include "ui_chat.h"

#include <QtWidgets/qdialog.h>

#include <QtBluetooth/qbluetoothhostinfo.h>

QT_USE_NAMESPACE

class ChatServer;
class ChatClient;

class Chat : public QDialog
{
    Q_OBJECT

public:
    explicit Chat(QWidget *parent = nullptr);
    ~Chat();

signals:
    void sendMessage(const QString &message);

private slots:
    void connectClicked();
    void clientDisconnected();

private:
    int currentAdapterIndex = 0;
    Ui_Chat *ui;

    ChatServer *server;
    QList<ChatClient *> clients;
    QList<QBluetoothHostInfo> localAdapters;

    QString localName;
};
