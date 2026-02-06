#include "widgetpopsauceclient.h"
#include "ui_widgetpopsauceclient.h"

WidgetPopSauceClient::WidgetPopSauceClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetPopSauceClient)
{
    ui->setupUi(this);

    monTimer = new QTimer(this);
    connect(monTimer,&QTimer::timeout,this,&WidgetPopSauceClient::updateCompteur);

    connect(&socketJoueur,&QTcpSocket::connected,this,&WidgetPopSauceClient::onQTcpSocket_connected);
    connect(&socketJoueur,&QTcpSocket::disconnected,this,&WidgetPopSauceClient::onQTcpSocket_disconnected);
    connect(&socketJoueur,&QTcpSocket::readyRead,this,&WidgetPopSauceClient::onQTcpSocket_readyRead);
    connect(&socketJoueur,&QTcpSocket::errorOccurred,this,&WidgetPopSauceClient::onQTcpSocket_errorOccured);
}

WidgetPopSauceClient::~WidgetPopSauceClient()
{
    delete ui;
}

void WidgetPopSauceClient::envoyerDonnees()
{
    quint64 taille;
    QBuffer tampon;
    QString reponse=ui->lineEditReponse->text();
    QChar commande = 'R';

    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    out<<taille<<commande<<reponse;
    taille=(static_cast<quint64>(tampon.size()))-sizeof(taille);
    tampon.seek(0);
    out<<taille;
    qDebug() <<"envoyerDonnees"<< reponse;
    socketJoueur.write(tampon.buffer());

    ui->labelAfficherReponseEcrit->setText(reponse);
}


void WidgetPopSauceClient::on_pushButtonConnexion_clicked()
{
    if (ui->pushButtonConnexion->text() == "Connexion")
    {
        socketJoueur.connectToHost(ui->lineEditIP->text(),ui->spinBoxPort->value());
        //ui->groupBoxConnexion->setEnabled(false);
        ui->groupBoxJeu->setEnabled(true);
        ui->pushButtonConnexion->setText("Deconnexion");
    }else{
        // ui->groupBoxConnexion->setEnabled(true);
        ui->groupBoxJeu->setEnabled(false);
        ui->pushButtonConnexion->setText("Connexion");
        socketJoueur.close();
    }
}

void WidgetPopSauceClient::onQTcpSocket_connected()
{
    qDebug() << "Connecté";
}

void WidgetPopSauceClient::onQTcpSocket_disconnected()
{
    qDebug() << "Déconnecté";
}

void WidgetPopSauceClient::onQTcpSocket_readyRead()
{
    quint64 taille; // La taille minimale pour prendre en compte les datas
    QChar commande;
    QPixmap img ;
    QString question;
    int score;


    qDebug() << this << "Ready Read : " << QString::number(socketJoueur.bytesAvailable()) << " bytes Availables";
    if (!enCoursDeLecture)
    {
        if (socketJoueur.bytesAvailable() >= (qint64)sizeof(taille))
        {
            QDataStream in(&socketJoueur);
            in >> taille;
            tailleAttendue=taille;
            enCoursDeLecture=true;
            qDebug()<<"Taille attendue : "<<tailleAttendue;
        }
    }
    if (enCoursDeLecture)
    {
        qDebug()<<"des donnees sont disponibles ";

        taille=socketJoueur.bytesAvailable();
        qDebug()<<"octets disponibles : "<<taille;
        if (taille >= (quint64)tailleAttendue)
        {
            QDataStream in(&socketJoueur);

            in>>commande;
            qDebug()<<"commande"<<commande;
            qDebug()<<"reception image";
            tailleAttendue = 0;
            enCoursDeLecture = false;
            switch (commande.toLatin1()) {
            case 'Q':
                int dureeTimer;
                in >> img >> question >> score >> dureeTimer;

                // Affichage de l'image
                ui->labelImage->setPixmap(img);
                ui->labelImage->setScaledContents(true);

                // Affichage de la question
                ui->labelQuestion->setText(question);
                ui->labelQuestion->setAlignment(Qt::AlignCenter);

                // Démarrer le compte à rebours
                tempsRestant = dureeTimer / 1000; // Convertir millisecondes en secondes
                ui->labelTemps->setText(QString::number(tempsRestant) + " s");

                monTimer->start(1000); // Déclencher toutes les secondes

                break;
            case 'V':
                ui->labelVraiFaux->setText("Bonne réponse");
                qDebug()<<"case V";
                // ui->labelScore->setText(QString::number(score));
                break;
            case 'F':
                ui->labelVraiFaux->setText("Mauvaise réponse");
                qDebug()<<"case F";
                break;
            case 'E':
            {
                QString reponse;
                in >> reponse;
                //ui->labelVraiFaux->setText(reponse);
                qDebug()<<"case E";
                break;
            }
            default:
                break;
            }
        }
    }
}

void WidgetPopSauceClient::onQTcpSocket_errorOccured(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Erreur";
}


void WidgetPopSauceClient::on_pushButtonEnvoyer_clicked()
{
    envoyerDonnees();
}

void WidgetPopSauceClient::updateCompteur()
{
    tempsRestant--;

    if (tempsRestant <= 0)
    {
        monTimer->stop();
        ui->labelTemps->setText("0 s");
        qDebug() << "Temps écoulé !";
    }
    else
    {
        ui->labelTemps->setText(QString::number(tempsRestant)+ "s");
    }
}

void WidgetPopSauceClient::on_pushButtonInscription_clicked()
{
    // Stub implementation for inscription button
}

void WidgetPopSauceClient::on_pushButtonConnexionCompte_clicked()
{
    // Stub implementation for connexion compte button
}

void WidgetPopSauceClient::on_pushButtoninscrire_clicked()
{
    // Stub implementation for inscrire button
}
