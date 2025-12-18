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
    quint64 taille;
    QBuffer tampon;
    QString reponse=ui->lineEditReponse->text();

    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    out<<taille<<reponse;
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
    // quint64 taille=0;
    // QChar commande;
    // QPixmap image;
    // QString question;
    // int score;
    // int timer;
    // // Il y a au moins le champs taille d'arrive
    // if (socketJoueur.bytesAvailable() >= (qint64)sizeof(taille))
    // {
    //     // Lecture de la taille de la trame
    //     QDataStream in(&socketJoueur);
    //     in >> taille;
    //     qDebug()<<"nb octet attendus : "<<taille;
    //     // Le reste de la trame est disponible
    //     qDebug()<<"nb octet dispo : "<<socketJoueur.bytesAvailable();
    //     if (socketJoueur.bytesAvailable() >= (qint64)taille)
    //     {
    //         // Lecture de la commande
    //         in>>commande;
    //         qDebug()<<"commande : "<<commande;
    //         switch (commande.toLatin1()) {

    //         case 'Q':
    //             in >> image >> question >> score >> timer;
    //             //QPixmap imageTest(imagePath);
    //             if (image.isNull()) {
    //                 // En cas d'échec (fichier introuvable ou chemin incorrect)
    //                 ui->labelImage->setText("ERREUR DE TEST : Image non trouvée ou échec du chargement ");
    //                 ui->labelImage->setStyleSheet("color: red; font-weight: bold;");
    //             }
    //             else
    //             {

    //                 ui->labelImage->setPixmap(image);
    //                             // imageTest.scaled(
    //                             //     350, 350, // Taille d'affichage souhaitée
    //                             //     Qt::KeepAspectRatio,
    //                             //     Qt::SmoothTransformation
    //                             //     )
    //                         //    );
    //                 ui->labelImage->setAlignment(Qt::AlignCenter);
    //                 ui->labelImage->resize(image.size());
    //             }
    //             break;
    //         case 'V':

    //             break;
    //         }
    //     }
    // }

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
            qDebug()<<"reception image";
            if (commande=='Q')
            {
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
            }

        }
        else
        {
            qDebug() << "En attente de " << (tailleAttendue - taille) << " bytes supplémentaires";
        }

    }
}

void WidgetPopSauceClient::onQTcpSocket_errorOccured(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Erreur";
}


void WidgetPopSauceClient::on_pushButtonEnvoyer_clicked()
{
    // // --- 1. Définir le chemin de l'image de test ---
    // // Changez ceci par le chemin réel de votre image de test.
    // // Il est fortement recommandé d'utiliser le système de ressources (QRC) de Qt pour le chemin :
    // const QString imagePath = "/home/USERS/ELEVES/CIEL2024/hatomyan/Images/img/Fernando.jpg";
    // // Si vous n'utilisez pas QRC, utilisez un chemin de fichier absolu/relatif, ex: "C:/data/test.jpg"

    // // 2. Charger l'image dans un QPixmap
    // QPixmap imageTest(imagePath);

    // // 3. Vérifier si le chargement a réussi
    // if (imageTest.isNull()) {
    //     // En cas d'échec (fichier introuvable ou chemin incorrect)
    //     ui->labelImage->setText("ERREUR DE TEST : Image non trouvée ou échec du chargement à " + imagePath);
    //     ui->labelImage->setStyleSheet("color: red; font-weight: bold;");
    //     return;
    // }

    // // 4. Afficher le QPixmap dans le QLabel dédié à l'image
    // // Assurez-vous que le nom du QLabel dans votre UI est bien 'imageLabel' (ou adaptez-le, ex: 'labelImageQuestion')
    // ui->labelImage->setPixmap(
    //             imageTest.scaled(
    //                 350, 350, // Taille d'affichage souhaitée
    //                 Qt::KeepAspectRatio,
    //                 Qt::SmoothTransformation
    //                 )
    //             );
    // ui->labelImage->setAlignment(Qt::AlignCenter);

    // ui->labelQuestion->setText("Qui est-ce ?");
    // ui->labelQuestion->setAlignment(Qt::AlignCenter);

    envoyerDonnees();
}

