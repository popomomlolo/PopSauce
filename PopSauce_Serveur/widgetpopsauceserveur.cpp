#include "widgetpopsauceserveur.h"
#include "ui_widgetpopsauceserveur.h"

WidgetPopSauceServeur::WidgetPopSauceServeur(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetPopSauceServeur)
{
    ui->setupUi(this);
    connect(&sockEcoute, &QTcpServer::newConnection,
            this, &WidgetPopSauceServeur::onQTcpServer_newConnection);
}

WidgetPopSauceServeur::~WidgetPopSauceServeur()
{
    delete ui;
}

void WidgetPopSauceServeur::on_pushButtonLancer_clicked()
{
    sockEcoute.listen(QHostAddress::Any, ui->spinBoxPort->value());
    qDebug() << "Serveur lancé";
}

void WidgetPopSauceServeur::onQTcpServer_newConnection()
{
    QTcpSocket *client = sockEcoute.nextPendingConnection();
    connect(client, &QTcpSocket::readyRead,
            this, &WidgetPopSauceServeur::onQTcpSocket_readyRead);
    connect(client, &QTcpSocket::disconnected,
            this, &WidgetPopSauceServeur::onQTcpSocket_disconnected);
    listeDesClients.append(client);
    qDebug() << "Nouvelle connexion de" << client->peerAddress().toString();

}

void WidgetPopSauceServeur::onQTcpSocket_disconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Déconnexion de" << client->peerAddress().toString();
    listeDesClients.removeOne(client);
}

void WidgetPopSauceServeur::onQTcpSocket_readyRead()
{

}

