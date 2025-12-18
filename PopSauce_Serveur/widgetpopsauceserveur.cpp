#include "widgetpopsauceserveur.h"
#include "ui_widgetpopsauceserveur.h"

WidgetPopSauceServeur::WidgetPopSauceServeur(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetPopSauceServeur)
{
    ui->setupUi(this);
    srand(time(nullptr));

    //QPixmap img("/home/USERS/ELEVES/CIEL2024/alaffiac/CIEL_2/challenge_noel/images_jpeg/xavier_sous_frozen.jpeg");
    //ui->labelImage->setPixmap(img);

    QString ip,login,mdp,base;
    QString conf="conf.ini";

    QFileInfo testFichier(conf);

    if (testFichier.exists() && testFichier.isFile())
    {
        QSettings paramsSocket(conf,QSettings::IniFormat);

        ip=paramsSocket.value("configBDD/ip","178.58.18.8").toString();
        login=paramsSocket.value("configBDD/login","ciel").toString();
        mdp=paramsSocket.value("configBDD/mdp","ciel").toString();
        base=paramsSocket.value("configBDD/base","popSauce").toString();

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
    bddQestion();
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
    quint64 taille=0;
    QString reponse;
    if (client->bytesAvailable() >= (qint64)sizeof(taille))
    {
        // Création du flux de lecture sur la socket
        QDataStream in(client);
        // Lecture de la taille de la trame (en octets)
        in >> taille;

        // Vérification que le reste de la trame est complètement arrivé
        if (client->bytesAvailable() >= (qint64)taille)
        {
            
            in>>reponse;
            qDebug() << "reponse utilisateur "<<reponse;
            ui->textEdit->append("reponse utilisateur "+reponse);
            envoyerVérification(client,reponse);
        }
    }

}

void WidgetPopSauceServeur::envoyerQuestion(QTcpSocket *client)
{
    quint64 taille = 0;
    QBuffer tampon;
    QChar commande('Q');
    QPixmap img("/home/USERS/ELEVES/CIEL2024/alaffiac/CIEL_2/challenge_noel/images_jpeg/"+indice);
    ui->labelImage->setPixmap(img);
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
    qDebug() <<"envoyerQuestion"<< taille << commande << img << question << score << tempsMilisecondes;
    // Envoi
    client->write(tampon.buffer());
}

void WidgetPopSauceServeur::envoyerVérification(QTcpSocket *client,QString reponse)
{
    quint64 taille = 0;
    QBuffer tampon;
    QChar commande('Q');
    //QString bonneReponse=bddReponse();
    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    if (reponse==bReponse || reponse==alt1 || reponse==alt2)
    {
        commande='V';
    }
    else
    {
        commande='F';
    }
    // Construction de la trame
    out << taille << commande;

    // Calcul et mise à jour de la taille
    taille = static_cast<quint64>(tampon.size()) - sizeof(taille);
    tampon.seek(0);
    out << taille;
    qDebug() <<"envoyerVérification"<< taille << commande;
    // Envoi
    client->write(tampon.buffer());
}

void WidgetPopSauceServeur::envoyerFin(QTcpSocket *client)
{
    quint64 taille = 0;
    QBuffer tampon;
    QChar commande('E');
    //QString bonneReponse=bddReponse();
    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    // Construction de la trame
    out << taille << commande<< bReponse;

    // Calcul et mise à jour de la taille
    taille = static_cast<quint64>(tampon.size()) - sizeof(taille);
    tampon.seek(0);
    out << taille;
    qDebug() <<"envoyerFin"<< taille << commande << bReponse;
    // Envoi
    client->write(tampon.buffer());
}


void WidgetPopSauceServeur::bddQestion()
{
    int nbRandom = rand() % 2 + 1; //Nombre random entre 1 et 2 (seuleument changer le premier nb pas le deuxieme)
    QSqlQuery requetePrepare;

    requetePrepare.prepare("SELECT texte_question, indice, reponse,option_a ,option_b FROM question WHERE id_quest = :id;");
    requetePrepare.bindValue(":id",nbRandom);

    if (!requetePrepare.exec()){
        qDebug()<<"pb requete "<<requetePrepare.lastError();
    }

    while(requetePrepare.next())
    {
        question=requetePrepare.value("texte_question").toString();
        indice=requetePrepare.value("indice").toString();
        bReponse=requetePrepare.value("reponse").toString();
        alt1=requetePrepare.value("option_a").toString();
        alt2=requetePrepare.value("option_b").toString();
        qDebug()<<question << indice << bReponse;
    }

}


