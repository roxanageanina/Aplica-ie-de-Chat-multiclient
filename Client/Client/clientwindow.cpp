#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QMessageBox>
#include <QDebug>
#include <QDateTime>
#include <QPalette>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClientWindow), socket(nullptr), username("anonymous"), lookingForChat(false)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout;
    chatWindow = new QTextEdit(this);
    chatWindow->setReadOnly(true);
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send Message", this);
    startButton = new QPushButton("Start Chat", this);
    stopButton = new QPushButton("Stop Chat", this);

    QPalette pal = palette();
    pal.setColor(QPalette::Base, QColor(180, 230, 180));
    pal.setColor(QPalette::Text, Qt::black);
    setPalette(pal);

    QPalette buttonPalette;
    buttonPalette.setColor(QPalette::Button, QColor(0, 128, 0));
    buttonPalette.setColor(QPalette::ButtonText, Qt::white);
    sendButton->setPalette(buttonPalette);
    startButton->setPalette(buttonPalette);
    stopButton->setPalette(buttonPalette);

    sendButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    startButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
    stopButton->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);

    layout->addWidget(chatWindow);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    QPalette centralWidgetPalette;
    centralWidget->setAutoFillBackground(true);
    centralWidget->setPalette(centralWidgetPalette);

    connect(startButton, &QPushButton::clicked, this, &ClientWindow::startChat);
    connect(stopButton, &QPushButton::clicked, this, &ClientWindow::stopChat);
    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ClientWindow::sendMessage);

    socket = new QTcpSocket(this);
    connect(socket, &QTcpSocket::readyRead, this, &ClientWindow::readSocket);
    connect(socket, &QTcpSocket::disconnected, this, &ClientWindow::discardSocket);
}

ClientWindow::~ClientWindow()
{
    delete ui;
}

void ClientWindow::startChat()
{
    if (socket->state() == QAbstractSocket::UnconnectedState) {
        socket->connectToHost("127.0.0.1", 1234);
    }
    lookingForChat = true;
}

void ClientWindow::stopChat()
{
    if (socket->state() == QAbstractSocket::ConnectedState) {
        socket->disconnectFromHost();
    }
    lookingForChat = false;
}

void ClientWindow::sendMessage()
{
    if (socket->state() == QAbstractSocket::ConnectedState && !messageInput->text().isEmpty()) {
        QByteArray data = messageInput->text().toUtf8();
        appendMessage(messageInput->text(), true);
        socket->write(data);
        messageInput->clear();
    }
}

void ClientWindow::readSocket()
{
    QByteArray data = socket->readAll();
    appendMessage(QString::fromUtf8(data), false);
}

void ClientWindow::discardSocket()
{
    appendMessage("Disconnected from server.", false);
    if (lookingForChat) {
        startButton->setEnabled(true);
    }
}

void ClientWindow::resetConnection()
{
    socket->abort();
    socket->connectToHost("127.0.0.1", 1234);
}

void ClientWindow::appendMessage(const QString &message, bool isOwnMessage)
{
    QString formattedMessage = isOwnMessage ? "Me: " + message : message;
    chatWindow->append(formattedMessage);
}
