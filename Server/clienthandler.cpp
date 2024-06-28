#include "ClientHandler.h"
#include <QDebug>

ClientHandler::ClientHandler(qintptr socketDescriptor, QObject *parent)
    : QObject(parent), socketDescriptor(socketDescriptor), socket(new QTcpSocket(this))
{
    socket->setSocketDescriptor(socketDescriptor);

    connect(socket, &QTcpSocket::readyRead, this, &ClientHandler::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, this, &ClientHandler::onDisconnected);
}

ClientHandler::~ClientHandler()
{
    socket->disconnectFromHost();
    socket->deleteLater();
}

void ClientHandler::send(const QByteArray &data)
{
    socket->write(data);
}

qintptr ClientHandler::getSocketDescriptor() const
{
    return socketDescriptor;
}

QString ClientHandler::getSex() const
{
    return sex;
}

void ClientHandler::setSex(const QString &sex)
{
    this->sex = sex;
}

QString ClientHandler::getCountry() const
{
    return country;
}

void ClientHandler::setCountry(const QString &country)
{
    this->country = country;
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
