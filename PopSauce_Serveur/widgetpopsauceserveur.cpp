#include "widgetpopsauceserveur.h"
#include "ui_widgetpopsauceserveur.h"

WidgetPopSauceServeur::WidgetPopSauceServeur(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetPopSauceServeur)
{
    ui->setupUi(this);

    QPixmap img("/home/USERS/ELEVES/CIEL2024/alaffiac/CIEL_2/challenge_noel/images_jpeg/xavier_sous_frozen.jpeg");
    ui->labelImage->setPixmap(img);

    QString ip,login,mdp,base;
    QString conf="conf.ini";

    QFileInfo testFichier(conf);

    if (testFichier.exists() && testFichier.isFile())
    {
        QSettings paramsSocket(conf,QSettings::IniFormat);

        ip=paramsSocket.value("configBDD/ip","178.58.18.8").toString();
        login=paramsSocket.value("configBDD/login","ciel").toString();
        mdp=paramsSocket.value("configBDD/mdp","ciel").toString();
        base=paramsSocket.value("configBDD/base","popSauc").toString();

        qDebug()<<ip<<login<<mdp<<base;
    }
    else
    {
        qDebug()<<"fichier ini non valide";
    }
    qDebug()<<ip<<login<<mdp<<base;

    QSqlDatabase bdd;
    bdd = QSqlDatabase::addDatabase("QMYSQL");

    bdd.setHostName(ip);
    bdd.setDatabaseName(base);
    bdd.setUserName(login);
    bdd.setPassword(mdp);

    bool ok = bdd.open();
    if (!ok)
    {
        QMessageBox::warning(this,"Erreur de connexion à la bdd ",bdd.lastError().text());
    }
    else
    {
        qDebug()<<"Ouverture de la BDD ok";
    }
    
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
    envoyerQuestion(client);

}

void WidgetPopSauceServeur::onQTcpSocket_disconnected()
{
    QTcpSocket *client = qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Déconnexion de" << client->peerAddress().toString();
    listeDesClients.removeOne(client);
}

void WidgetPopSauceServeur::onQTcpSocket_readyRead()
{
    QTcpSocket *client=qobject_cast<QTcpSocket *>(sender());
    QDataStream in(client);
    quint16 taille=0;

    if (client->bytesAvailable() >= (qint64)sizeof(taille))
    {
        // Création du flux de lecture sur la socket
        QDataStream in(client);
        // Lecture de la taille de la trame (en octets)
        in >> taille;

        // Vérification que le reste de la trame est complètement arrivé
        if (client->bytesAvailable() >= (qint64)taille)
        {
            QChar commande;
            in>>commande;
            QPixmap reponse;
            in>>reponse;
            //envoyerQuestion(client);
        }
    }
}

void WidgetPopSauceServeur::envoyerQuestion(QTcpSocket *client)
{
    quint64 taille = 0;
    QBuffer tampon;
    QChar commande('Q');
    QPixmap img("/home/USERS/ELEVES/CIEL2024/alaffiac/CIEL_2/challenge_noel/images_jpeg/xavier_sous_frozen.jpeg");
    ui->labelImage->setPixmap(img);
    QString question("Qui est cet homme ?");
    int score(0);
    int tempsMilisecondes=0;
    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);

    // Construction de la trame
    out << taille << commande << img << question << score << tempsMilisecondes;

    // Calcul et mise à jour de la taille
    taille = static_cast<quint64>(tampon.size()) - sizeof(taille);
    tampon.seek(0);
    out << taille;
 qDebug() << taille << commande << img << question << score << tempsMilisecondes;
    // Envoi
    client->write(tampon.buffer());
}



