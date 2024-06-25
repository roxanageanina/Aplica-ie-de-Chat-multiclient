#ifndef SERVERWINDOW_H
#define SERVERWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QList>
#include <QMap>
#include "clienthandler.h"

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
    void discardSocket(ClientHandler *handler);
    void clientDisconnected(ClientHandler *handler);

private:
    Ui::ServerWindow *ui;
    QTcpServer *server;
    QList<ClientHandler*> clientHandlers;
    QMap<ClientHandler*, ClientHandler*> chatPairs;
    static int userCounter;

    ClientHandler* findAvailablePartner(ClientHandler *requestingClient);
    void notifyPartnerDisconnected(ClientHandler *handler);
};

#endif // SERVERWINDOW_H
