#include "serverwindow.h"
#include "ui_serverwindow.h"


ServerWindow::ServerWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::ServerWindow)
    , server(new QTcpServer(this))
{
    ui->setupUi(this);
    connect(server, &QTcpServer::newConnection, this, &ServerWindow::newConnection);

    if (server->listen(QHostAddress::Any, 1234))
    {
        ui->statusbar->showMessage("Server started");
    } else
    {
        ui->statusbar->showMessage("Server failed to start");
    }

    setStyleSheet("QWidget { background-color: #E0FFFF; color: #000000; }"
                  "QPushButton { background-color: #40E0D0; border: 2px solid #008B8B; color: #000000; padding: 5px; border-radius: 10px; }"
                  "QLineEdit { background-color: #FFFFFF; border: 2px solid #008B8B; padding: 5px; border-radius: 10px; }"
                  "QTextEdit { background-color: #FFFFFF; border: 2px solid #008B8B; padding: 5px; border-radius: 10px; }"
                  "QComboBox { background-color: #FFFFFF; border: 2px solid #008B8B; padding: 5px; border-radius: 10px; }"
                  "QStatusBar { background-color: #E0FFFF; color: #000000; }");

    ui->textEdit->setStyleSheet("background-color: #FFFFFF; border: 2px solid #008B8B; padding: 5px; border-radius: 10px;");
}

ServerWindow::~ServerWindow()
{
    delete ui;
}

void ServerWindow::newConnection()
{
    while (server->hasPendingConnections())
    {
        QTcpSocket *socket = server->nextPendingConnection();
        ClientHandler *handler = new ClientHandler(socket->socketDescriptor(), this);
        clientHandlers.append(handler);
        connect(handler, &ClientHandler::readyRead, this, &ServerWindow::readSocket);
        connect(handler, &ClientHandler::disconnected, this, &ServerWindow::clientDisconnected);

        ui->textEdit->append("Client with socket: " + QString::number(socket->socketDescriptor()) + " connected.");
        ui->comboBox_clientList->addItem(QString::number(socket->socketDescriptor()));
    }
}

void ServerWindow::readSocket(ClientHandler *handler, const QByteArray &data)
{
    QString message = QString::fromUtf8(data);
    QStringList parts = message.split("|");

    if (message.startsWith("IMAGE:"))
    {
        if (chatPairs.contains(handler))
        {
            ClientHandler *partner = chatPairs[handler];
            partner->send(data);
            ui->textEdit->append("Client with socket " + QString::number(handler->getSocketDescriptor()) + " sent an image to client with socket " + QString::number(partner->getSocketDescriptor()));
        }
    }
    else if (parts.size() == 2)
    {
        handler->setSex(parts[0]);
        handler->setCountry(parts[1]);

        ClientHandler *partner = findAvailablePartner(handler);
        if (partner)
        {
            chatPairs[handler] = partner;
            chatPairs[partner] = handler;

            handler->send("Buna! Se cauta un utilizator cu care poti conversa...");


            handler->send("Partener gasit: " + partner->getSex().toUtf8() + ", " + partner->getCountry().toUtf8());
            partner->send("Partener gasit: " + handler->getSex().toUtf8() + ", " + handler->getCountry().toUtf8());
        }
        else
        {
            handler->send("Buna! Se cauta un utilizator cu care poti conversa...");
            handler->send("Nu am gasit niciun partener disponibil, va rugam sa asteptati...");
        }
    }
    else if (chatPairs.contains(handler))
    {
        ClientHandler *partner = chatPairs[handler];
        QString message = "<b>Stranger:</b> " + QString::fromUtf8(data);
        partner->send(message.toUtf8());

        ui->textEdit->append("Client with socket " + QString::number(handler->getSocketDescriptor()) + " sent a message to client with socket " + QString::number(partner->getSocketDescriptor()) + ": '" + QString::fromUtf8(data) + "'");

    }
}

void ServerWindow::clientDisconnected(ClientHandler *handler)
{
    notifyPartnerDisconnected(handler);

    clientHandlers.removeAll(handler);
    chatPairs.remove(handler);

    QString socketDescriptor = QString::number(handler->getSocketDescriptor());
    int index = ui->comboBox_clientList->findText(socketDescriptor);

    if (index != -1)
    {
        ui->comboBox_clientList->removeItem(index);
    }

    handler->deleteLater();

    ui->textEdit->append("Client with socket " + QString::number(handler->getSocketDescriptor()) + " disconnected.");
}

void ServerWindow::notifyPartnerDisconnected(ClientHandler *handler)
{
    if (chatPairs.contains(handler))
    {
        ClientHandler *partner = chatPairs[handler];
        chatPairs.remove(partner);
        chatPairs.remove(handler);

        if (partner)
        {
            partner->send("Partenerul s-a deconectat.|DISCONNECT");

            clientHandlers.removeAll(partner);
            partner->deleteLater();
        }
    }
}

ClientHandler* ServerWindow::findAvailablePartner(ClientHandler *requestingClient)
{
    for (ClientHandler *handler : qAsConst(clientHandlers))
    {
        if (handler != requestingClient && !chatPairs.contains(handler))
        {
            return handler;
        }
    }
    return nullptr;
}
