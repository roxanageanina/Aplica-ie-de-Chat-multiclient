#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QVBoxLayout>

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWindow; }
QT_END_NAMESPACE

class ClientWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit ClientWindow(QWidget *parent = nullptr);
    ~ClientWindow();

private slots:
    void startChat();
    void stopChat();
    void sendMessage();
    void readSocket();
    void discardSocket();
    void resetConnection();
    void appendMessage(const QString &message, bool isOwnMessage);

private:
    Ui::ClientWindow *ui;
    QTcpSocket *socket;
    QTextEdit *chatWindow;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *startButton;
    QPushButton *stopButton;
    QString username;
    bool lookingForChat;
};

#endif // CLIENTWINDOW_H
