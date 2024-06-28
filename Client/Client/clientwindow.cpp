#include "clientwindow.h"
#include "ui_clientwindow.h"
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QVBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QFileDialog>
#include <QImageReader>
#include <QImage>
#include <QPixmap>

ClientWindow::ClientWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::ClientWindow), socket(nullptr), lookingForChat(false)
{
    ui->setupUi(this);

    QVBoxLayout *layout = new QVBoxLayout;
    chatWindow = new QTextEdit(this);
    chatWindow->setReadOnly(true);
    messageInput = new QLineEdit(this);
    sendButton = new QPushButton("Send Message", this);
    startButton = new QPushButton("Start Chat", this);
    stopButton = new QPushButton("Stop Chat", this);
    closeButton = new QPushButton("Close Application", this);
    sendImageButton = new QPushButton("Send Image", this);
    sexComboBox = new QComboBox(this);
    countryComboBox = new QComboBox(this);

    sexComboBox->addItem("Select Sex");
    sexComboBox->addItem("Barbat");
    sexComboBox->addItem("Femeie");

    countryComboBox->addItem("Select Country");
    countryComboBox->addItem("USA");
    countryComboBox->addItem("UK");
    countryComboBox->addItem("Franta");
    countryComboBox->addItem("Germania");
    countryComboBox->addItem("Romania");

    layout->addWidget(chatWindow);
    layout->addWidget(messageInput);
    layout->addWidget(sendButton);
    layout->addWidget(sendImageButton);
    layout->addWidget(sexComboBox);
    layout->addWidget(countryComboBox);
    layout->addWidget(startButton);
    layout->addWidget(stopButton);
    layout->addWidget(closeButton);
    ui->centralwidget->setLayout(layout);

    setStyleSheet("QWidget { background-color: #E0FFFF; color: #000000; }"
                  "QPushButton { background-color: #40E0D0; border: 2px solid #008B8B; color: #000000; padding: 5px; border-radius: 10px; }"
                  "QLineEdit { background-color: #FFFFFF; border: 2px solid #008B8B; padding: 5px; border-radius: 10px; }"
                  "QTextEdit { background-color: #FFFFFF; border: 2px solid #008B8B; padding: 5px; border-radius: 10px; }"
                  "QComboBox { background-color: #FFFFFF; border: 2px solid #008B8B; padding: 5px; border-radius: 10px; }"
                  "QStatusBar { background-color: #E0FFFF; color: #000000; }");

    QPalette centralWidgetPalette;
    centralWidget()->setAutoFillBackground(true);
    centralWidget()->setPalette(centralWidgetPalette);

    connect(startButton, &QPushButton::clicked, this, &ClientWindow::startChat);
    connect(stopButton, &QPushButton::clicked, this, &ClientWindow::stopChat);
    connect(sendButton, &QPushButton::clicked, this, &ClientWindow::sendMessage);
    connect(sendImageButton, &QPushButton::clicked, this, &ClientWindow::sendImage);
    connect(messageInput, &QLineEdit::returnPressed, this, &ClientWindow::sendMessage);
    connect(closeButton, &QPushButton::clicked, this, &ClientWindow::closeApplication);

    setChatControlsEnabled(false);

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
    if (!validateInputs())
    {
        return;
    }

    if (socket->state() == QAbstractSocket::UnconnectedState)
    {
        socket->connectToHost("127.0.0.1", 1234);
    }
    lookingForChat = true;

    QString sex = sexComboBox->currentText();
    QString country = countryComboBox->currentText();
    QString info = sex + "|" + country;
    socket->write(info.toUtf8());

    setChatControlsEnabled(true);
}

void ClientWindow::stopChat()
{
    if (!validateInputs())
    {
        return;
    }

    if (socket->state() == QAbstractSocket::ConnectedState)
    {
        socket->disconnectFromHost();
    }
    lookingForChat = false;

    setChatControlsEnabled(false);
}

void ClientWindow::sendMessage()
{
    if (!validateInputs())
    {
        return;
    }

    if (socket->state() == QAbstractSocket::ConnectedState && !messageInput->text().isEmpty())
    {
        QByteArray data = messageInput->text().toUtf8();
        appendMessage("<b>Me:</b> " + messageInput->text(), true);
        socket->write(data);
        messageInput->clear();
    }
}

void ClientWindow::sendImage()
{
    QString filePath = QFileDialog::getOpenFileName(this, "Selecteaza o imagine", "", "Images (*.png *.jpg *.jpeg)");

    if (!filePath.isEmpty())
    {
        QFile file(filePath);
        if (file.open(QIODevice::ReadOnly))
        {
            QByteArray imageData = file.readAll();
            QByteArray data = "IMAGE:" + imageData.toBase64();
            socket->write(data);
            file.close();
        }
    }
}

void ClientWindow::readSocket()
{
    QByteArray data = socket->readAll();
    QString message = QString::fromUtf8(data);
    qDebug()<< message;

    if (message.startsWith("IMAGE:"))
    {
        QByteArray imageData = QByteArray::fromBase64(message.mid(6).toUtf8());
        handleImageReceived(imageData);
    }
    else if (isDisconnectMessage(message))
    {
        handleDisconnectMessage(message);
    }
    else
    {
        appendMessage(message, false);
    }
}

void ClientWindow::handleImageReceived(const QByteArray &data)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Imagine primita", "Ati primit o imagine. Doriti sa o vizualizati?",
                                  QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes)
    {
        QPixmap pixmap;
        pixmap.loadFromData(data);
        QLabel *imageLabel = new QLabel;
        imageLabel->setPixmap(pixmap);
        QVBoxLayout *layout = new QVBoxLayout;
        layout->addWidget(imageLabel);
        QWidget *imageWindow = new QWidget;
        imageWindow->setLayout(layout);
        imageWindow->setWindowTitle("Received Image");
        imageWindow->setStyleSheet("QWidget { background-color: #E0FFFF; }");
        imageWindow->show();
    }
}

bool ClientWindow::isDisconnectMessage(const QString &message)
{
    return message.endsWith("|DISCONNECT");
}

void ClientWindow::handleDisconnectMessage(const QString &message)
{
    stopChat();
    showNotification("Chat Notification", "Your partner has disconnected.");
}

void ClientWindow::discardSocket()
{
    appendMessage("Unul dintre parteneri a finalizat conversatia. Daca doriti sa incepeti o noua convesatie, apasati butonul 'Start Chat'.", false);
    if (lookingForChat)
    {
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
    chatWindow->append(message);
}

void ClientWindow::showNotification(const QString &title, const QString &message)
{
    trayIcon->showMessage(title, message, QSystemTrayIcon::Information, 3000);
}

bool ClientWindow::validateInputs()
{
    bool sexSelected = sexComboBox->currentIndex() != 0;
    bool countrySelected = countryComboBox->currentIndex() != 0;

    if (!sexSelected || !countrySelected)
    {
        QMessageBox::warning(this, "Eroare", "Inainte de a incepe o conversatie, va rugam sa va selectati sex-ul si tara de unde proveniti.");
        return false;
    }
    return true;
}

void ClientWindow::setChatControlsEnabled(bool enabled)
{
    sendButton->setEnabled(enabled);
    sendImageButton->setEnabled(enabled);
    stopButton->setEnabled(enabled);
}

void ClientWindow::closeApplication()
{
    QDialog dialog(this);
    dialog.setWindowTitle("Rating aplicatie");
    dialog.setStyleSheet("QWidget { background-color: #E0FFFF; color: #000000; }");

    QVBoxLayout layout(&dialog);
    QLabel label("Va rugam sa oferiti o nota aplicatiei (1-10):");
    QComboBox comboBox;
    for (int i = 1; i <= 10; i++)
    {
        comboBox.addItem(QString::number(i));
    }
    QPushButton submitButton("Trimite");

    layout.addWidget(&label);
    layout.addWidget(&comboBox);
    layout.addWidget(&submitButton);

    connect(&submitButton, &QPushButton::clicked, &dialog, &QDialog::accept);

    if (dialog.exec() == QDialog::Accepted)
    {
        QString rating = comboBox.currentText();
        QFile file("ratings.txt");
        if (file.open(QIODevice::Append | QIODevice::Text))
        {
            QTextStream out(&file);
            out << rating << "\n";
            file.close();
        }
        QMessageBox::information(this, "Multumesc", "Va multumesc pentru rating-ul oferit!");
        qApp->quit();
    }
}
