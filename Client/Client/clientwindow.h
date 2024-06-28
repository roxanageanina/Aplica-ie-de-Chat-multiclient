#ifndef CLIENTWINDOW_H
#define CLIENTWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QTextEdit>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QSystemTrayIcon>
#include <QFileDialog>
#include <QImageReader>

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
    void closeApplication();
    void sendImage();
    void handleImageReceived(const QByteArray &data);

private:
    Ui::ClientWindow *ui;
    QTcpSocket *socket;
    QTextEdit *chatWindow;
    QLineEdit *messageInput;
    QPushButton *sendButton;
    QPushButton *startButton;
    QPushButton *stopButton;
    QPushButton *closeButton;
    QPushButton *sendImageButton;
    QComboBox *sexComboBox;
    QComboBox *countryComboBox;
    QSystemTrayIcon *trayIcon;
    bool lookingForChat;

    void resetConnection();
    void appendMessage(const QString &message, bool isOwnMessage);
    void showNotification(const QString &title, const QString &message);
    bool validateInputs();
    void setChatControlsEnabled(bool enabled);
    bool isDisconnectMessage(const QString &message);
    void handleDisconnectMessage(const QString &message);
};

#endif // CLIENTWINDOW_H
