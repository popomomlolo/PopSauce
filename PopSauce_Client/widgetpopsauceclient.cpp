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

}

void WidgetPopSauceClient::onQTcpSocket_errorOccured(QAbstractSocket::SocketError socketError)
{
    qDebug() << "Erreur";
}


void WidgetPopSauceClient::on_pushButtonEnvoyer_clicked()
{
    // --- 1. Définir le chemin de l'image de test ---
    // Changez ceci par le chemin réel de votre image de test.
    // Il est fortement recommandé d'utiliser le système de ressources (QRC) de Qt pour le chemin :
    const QString imagePath = "/home/USERS/ELEVES/CIEL2024/hatomyan/Images/img/Fernando.jpg";
    // Si vous n'utilisez pas QRC, utilisez un chemin de fichier absolu/relatif, ex: "C:/data/test.jpg"

    // 2. Charger l'image dans un QPixmap
    QPixmap imageTest(imagePath);

    // 3. Vérifier si le chargement a réussi
    if (imageTest.isNull()) {
        // En cas d'échec (fichier introuvable ou chemin incorrect)
        ui->labelImage->setText("ERREUR DE TEST : Image non trouvée ou échec du chargement à " + imagePath);
        ui->labelImage->setStyleSheet("color: red; font-weight: bold;");
        return;
    }

    // 4. Afficher le QPixmap dans le QLabel dédié à l'image
    // Assurez-vous que le nom du QLabel dans votre UI est bien 'imageLabel' (ou adaptez-le, ex: 'labelImageQuestion')
    ui->labelImage->setPixmap(
                imageTest.scaled(
                    350, 350, // Taille d'affichage souhaitée
                    Qt::KeepAspectRatio,
                    Qt::SmoothTransformation
                    )
                );
    ui->labelImage->setAlignment(Qt::AlignCenter);

    ui->labelQuestion->setText("Qui est-ce ?");
    ui->labelQuestion->setAlignment(Qt::AlignCenter);
}

