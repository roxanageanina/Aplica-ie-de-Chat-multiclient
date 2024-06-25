#include "serverwindow.h"
#include "ui_serverwindow.h"

int ServerWindow::userCounter = 0;

ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ServerWindow), server(new QTcpServer(this))
{
    ui->setupUi(this);
    connect(server, &QTcpServer::newConnection, this, &ServerWindow::newConnection);
    server->listen(QHostAddress::Any, 1234);
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::newConnection()
{
    while (server->hasPendingConnections()) {
        QTcpSocket *socket = server->nextPendingConnection();
        QString username = "anonymous" + QString::number(++userCounter);
        ClientHandler *handler = new ClientHandler(socket->socketDescriptor(), username, this);
        clientHandlers.append(handler);
        connect(handler, &ClientHandler::readyRead, this, &ServerWindow::readSocket);
        connect(handler, &ClientHandler::disconnected, this, &ServerWindow::clientDisconnected);

        ui->textEdit->append("Client with socket: " + QString::number(socket->socketDescriptor()) + " connected as " + username + ".");
        ui->comboBox_clientList->addItem(QString::number(socket->socketDescriptor()));

        handler->send("Buna, " + username.toUtf8() + "! Se cauta un utilizator cu care poti conversa...");

        ClientHandler *partner = findAvailablePartner(handler);
        if (partner) {
            chatPairs[handler] = partner;
            chatPairs[partner] = handler;

            handler->send("Partener gasit: " + partner->getUsername().toUtf8());
            partner->send("Partener gasit: " + handler->getUsername().toUtf8());
        } else {
            handler->send("Nu am gasit niciun partener disponibil, va rugam sa asteptati...");
        }
    }
}

void ServerWindow::readSocket(ClientHandler *handler, const QByteArray &data)
{
    if (chatPairs.contains(handler)) {
        ClientHandler *partner = chatPairs[handler];
        QString message = handler->getUsername() + ": " + QString::fromUtf8(data);
        partner->send(message.toUtf8());
    }
}

void ServerWindow::discardSocket(ClientHandler *handler)
{
    clientHandlers.removeAll(handler);
    chatPairs.remove(handler);
    handler->deleteLater();

    ui->textEdit->append("Client disconnected.");
}

void ServerWindow::clientDisconnected(ClientHandler *handler)
{
    notifyPartnerDisconnected(handler);

    clientHandlers.removeAll(handler);
    chatPairs.remove(handler);
    handler->deleteLater();

    ui->textEdit->append("Client disconnected.");
}

void ServerWindow::notifyPartnerDisconnected(ClientHandler *handler)
{
    if (chatPairs.contains(handler)) {
        ClientHandler *partner = chatPairs[handler];
        chatPairs.remove(partner);
        chatPairs.remove(handler);

        if (partner) {
            partner->send("Partenerul tau s-a deconectat. Apasa 'Start Chat' pentru a cauta alt partener.");
        }
    }
}

ClientHandler* ServerWindow::findAvailablePartner(ClientHandler *requestingClient)
{
    for (ClientHandler *handler : qAsConst(clientHandlers)) {
        if (handler != requestingClient && !chatPairs.contains(handler)) {
            return handler;
        }
    }
    return nullptr;
}
