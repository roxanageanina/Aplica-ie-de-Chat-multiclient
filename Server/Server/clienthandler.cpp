#include "clienthandler.h"
#include <QDebug>

ClientHandler::ClientHandler(qintptr socketDescriptor, const QString &username, QObject *parent)
    : QObject(parent), username(username)
{
    socket = new QTcpSocket(this);
    if (!socket->setSocketDescriptor(socketDescriptor)) {
        qDebug() << "Failed to set socket descriptor";
        return;
    }

    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
}

ClientHandler::~ClientHandler()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}

QString ClientHandler::getUsername() const
{
    return username;
}

void ClientHandler::send(const QByteArray &data)
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->write(data);
    }
}

void ClientHandler::onReadyRead()
{
    QByteArray data = socket->readAll();
    emit readyRead(this, data);
}

void ClientHandler::onDisconnected()
{
    emit disconnected(this);
}
