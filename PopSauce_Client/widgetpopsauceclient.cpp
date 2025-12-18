#include "widgetpopsauceclient.h"
#include "ui_widgetpopsauceclient.h"

WidgetPopSauceClient::WidgetPopSauceClient(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::WidgetPopSauceClient)
{
    ui->setupUi(this);

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
    quint64 taille=0;
    QBuffer tampon;
    QString reponse=ui->lineEditReponse->text();
    QChar commande='R';

    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
<<<<<<< HEAD
=======
    commande='R';
>>>>>>> 203b2ab2787fc2ef2f0621d9b51144b6ab0487ee
    out<<taille<<commande<<reponse;
    taille=(static_cast<quint64>(tampon.size()))-sizeof(taille);
    tampon.seek(0);
    out<<taille;
    qDebug() <<"envoyerDonnees"<< commande << reponse;
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
    int timer;

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
            switch (commande.toLatin1()) {
            case 'Q':
                // lecture image
                in >> img >> question >> score >> timer;
                qDebug()<<"image reçu";

                //affichage image dans le QLabel ui->labelImg
                ui->labelImage->setPixmap(img);
                //ui->labelImg->resize(img.size());
                ui->labelImage->setScaledContents(true);
                tailleAttendue=0;
                enCoursDeLecture=false;

                ui->labelQuestion->setText(question);
                ui->labelQuestion->setAlignment(Qt::AlignCenter);
                break;
            case 'V':
                ui->labelVraiFaux->setText("Bonne réponse");
                qDebug()<<"case V";
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

void WidgetPopSauceClient::on_pushButtonInscription_clicked()
{
    pseudo = ui->lineEditInscrUser->text();
    mail = ui->lineEditMail->text();
    mdp = ui->lineEditInscrMdp->text();
    verifMdp = ui->lineEditInscrMdpVerif->text();

    envoyerInscription(pseudo, mail, mdp, verifMdp);

    qDebug() << pseudo << mail << mdp << verifMdp;
}

void WidgetPopSauceClient::envoyerInscription(QString pseudo, QString mail, QString mdp, QString mdp2)
{
    quint64 taille;
    QBuffer tampon;

    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    commande='I';
    out<<taille<<commande<<pseudo<<mail<<mdp<<mdp2;
    taille=(static_cast<quint64>(tampon.size()))-sizeof(taille);
    tampon.seek(0);
    out<<taille;
    qDebug() <<"Donnez envoyee"<<commande<<pseudo<<mail<<mdp<<mdp2;
    socketJoueur.write(tampon.buffer());


}


void WidgetPopSauceClient::on_pushButtonConnexionCompte_clicked()
{
    pseudo = ui->lineEditUser->text();
    mdp = ui->lineEditMdp->text();

    envoyerConnexionCompte(pseudo, mdp);

    qDebug()<< "Donne envoye" << commande << pseudo << mdp;
}

void WidgetPopSauceClient::envoyerConnexionCompte(QString pseudo, QString mdp)
{
    quint64 taille;
    QBuffer tampon;

    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    commande='C';
    out<<taille<<commande<<pseudo<<mdp;
    taille=(static_cast<quint64>(tampon.size()))-sizeof(taille);
    tampon.seek(0);
    out<<taille;
    qDebug() <<"Donnez envoyee"<<commande<<pseudo<<mdp;
    socketJoueur.write(tampon.buffer());
}
