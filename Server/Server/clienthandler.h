 #ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QTcpSocket>

class ClientHandler : public QObject
{
    Q_OBJECT

public:
    explicit ClientHandler(qintptr socketDescriptor, const QString &username, QObject *parent = nullptr);
    ~ClientHandler();

    QString getUsername() const;
    void send(const QByteArray &data);

signals:
    void readyRead(ClientHandler *handler, const QByteArray &data);
    void disconnected(ClientHandler *handler);

private slots:
    void onReadyRead();
    void onDisconnected();

private:
    QTcpSocket *socket;
    QString username;
};

#endif // CLIENTHANDLER_H
