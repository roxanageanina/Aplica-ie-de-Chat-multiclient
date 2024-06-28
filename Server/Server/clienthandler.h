#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>

class ClientHandler : public QObject
{
    Q_OBJECT

public:
    explicit ClientHandler(qintptr socketDescriptor, QObject *parent = nullptr);
    ~ClientHandler();

    void send(const QByteArray &data);
    qintptr getSocketDescriptor() const;
    QString getSex() const;
    void setSex(const QString &sex);
    QString getCountry() const;
    void setCountry(const QString &country);

signals:
    void readyRead(ClientHandler *handler, const QByteArray &data);
    void disconnected(ClientHandler *handler);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *socket;
    qintptr socketDescriptor;
    QString sex;
    QString country;
};

#endif // CLIENTHANDLER_H
