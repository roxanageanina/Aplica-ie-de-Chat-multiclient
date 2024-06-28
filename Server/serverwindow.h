#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include <QList>
#include "ClientHandler.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ServerWindow; }
QT_END_NAMESPACE

class ServerWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ServerWindow(QWidget *parent = nullptr);
    ~ServerWindow();

private slots:
    void newConnection();
    void readSocket(ClientHandler *handler, const QByteArray &data);
    void clientDisconnected(ClientHandler *handler);

private:
    Ui::ServerWindow *ui;
    QTcpServer *server;
    QList<ClientHandler*> clientHandlers;
    QHash<ClientHandler*, ClientHandler*> chatPairs;

    void notifyPartnerDisconnected(ClientHandler *handler);
    ClientHandler* findAvailablePartner(ClientHandler *requestingClient);
};

#endif // SERVERWINDOW_H
