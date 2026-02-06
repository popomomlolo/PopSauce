#include "widgetpopsauceserveur.h"
#include "ui_widgetpopsauceserveur.h"
#include <algorithm>

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

    srand(time(nullptr));

    // AJOUT : Configuration du timer
    timer = new QTimer(this);
    timer->setInterval(20000);  // 20 secondes = 20000 millisecondes
    timer->setSingleShot(true);

    connect(timer, &QTimer::timeout, this, &WidgetPopSauceServeur::onTimer_timeout);

    timerProchaine = new QTimer(this);
    timerProchaine->setInterval(5000);  // 5 secondes
    timerProchaine->setSingleShot(true);
    connect(timerProchaine, &QTimer::timeout, this, &WidgetPopSauceServeur::onTimerProchaine_timeout);


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

    // Charger la première question au démarrage du serveur
    bddQestion();
}

void WidgetPopSauceServeur::onQTcpServer_newConnection()
{

    QTcpSocket *client=sockEcoute.nextPendingConnection();

    connect(client,&QTcpSocket::connected,this,&WidgetPopSauceServeur::onQTcpSocket_connected);
    connect(client,&QTcpSocket::disconnected,this,&WidgetPopSauceServeur::onQTcpSocket_disconnected);
    connect(client,&QTcpSocket::readyRead,this,&WidgetPopSauceServeur::onQTcpSocket_readyRead);
    connect(client,&QTcpSocket::errorOccurred,this,&WidgetPopSauceServeur::onQTcpSocket_errorOccured);


    Client *nouveauClient=new Client();

    nouveauClient->setSockClient(client);
    nouveauClient->setScores(0);
    nouveauClient->setNom("Joueur " + QString::number(listeClients.size() + 1));


    listeClients.append(nouveauClient);


    qDebug() << "Nouvelle connexion de" << client->peerAddress().toString();

    // Envoyer la question actuelle au nouveau client (ne pas charger une nouvelle question)
    if (!question.isEmpty())
    {
        envoyerQuestion(client);
        // Démarrer le timer seulement s'il n'est pas déjà actif
        if (!timer->isActive())
        {
            timer->start();
        }
        // Envoyer les scores actuels au nouveau client
        envoyerScores();
    }
}

void WidgetPopSauceServeur::onQTcpSocket_disconnected()
{
    QTcpSocket *client=qobject_cast<QTcpSocket *>(sender());
    ui->textEdit->append("disconnected client " +client->peerAddress().toString());
    int index= getIndexClient(client);
    if (index!=-1)
    {
        listeClients.removeAt(index);
    }
}

void WidgetPopSauceServeur::onQTcpSocket_readyRead()
{
    QTcpSocket *client=qobject_cast<QTcpSocket *>(sender());

    quint64 taille=0;
    QString reponse;
    QChar commande;
    qDebug()<<"reception";
    if (client->bytesAvailable() >= (qint64)sizeof(taille))
    {
        // Création du flux de lecture sur la socket
        QDataStream in(client);
        // Lecture de la taille de la trame (en octets)
        in >> taille;
        qDebug()<<"taille " <<taille;


        // Vérification que le reste de la trame est complètement arrivé
        if (client->bytesAvailable() >= (qint64)taille)
        {

            qDebug()<<"trame complete";

            in>>commande;
            qDebug()<<"commande : "<<commande;

            switch (commande.toLatin1()) {
            case 'R':
                in>>reponse;
                qDebug() << "reponse utilisateur "<<reponse;
                ui->textEdit->append("reponse utilisateur "+reponse);
                envoyerVerification(client,reponse);
                break;
            case 'I':{
                QString pseudo,mail,mdp,verifMdp;
                qDebug() << "Inscription client "<<pseudo<<mail<<mdp<<verifMdp;
                break;}
            case 'C':{
                QString pseudo,mdp;
                qDebug() << "Connexion client "<<pseudo<<mdp;
                break;}
            default:
                break;
            }
        }
    }
}

void WidgetPopSauceServeur::onQTcpSocket_errorOccured()
{
    QTcpSocket *client=qobject_cast<QTcpSocket*>(sender());
    qDebug() << "Nouvelle connexion de" << client->peerAddress().toString();
}

void WidgetPopSauceServeur::onQTcpSocket_connected()
{
    ui->textEdit->append("connected client");
}

void WidgetPopSauceServeur::onTimer_timeout()
{
    qDebug() << "Timer expiré ! 20 secondes écoulées";
    ui->textEdit->append("Temps écoulé pour la question !");

    // Envoyer les scores à tous les clients
    envoyerScores();

    // Envoyer la fin à tous les clients connectés
    for (int i = 0; i < listeClients.size(); i++)
    {
        envoyerFin(listeClients.at(i)->getSockClient());
    }

    // Arrêter le timer
    timer->stop();

    // AJOUT : Démarrer le timer de 5s avant la prochaine question
    ui->textEdit->append("Prochaine question dans 5 secondes...");
    timerProchaine->start();
}


void WidgetPopSauceServeur::onTimerProchaine_timeout()
{
    qDebug() << "5 secondes écoulées, envoi de la prochaine question";
    ui->textEdit->append("Envoi de la prochaine question !");

    // Arrêter le timer
    timerProchaine->stop();

    // Préparer et envoyer la prochaine question
    envoyerProchaineQuestion();
}


void WidgetPopSauceServeur::envoyerQuestion(QTcpSocket *client)
{
    quint64 taille = 0;
    QBuffer tampon;
    QChar commande('Q');
    QPixmap img("/home/USERS/ELEVES/CIEL2024/alaffiac/CIEL_2/challenge_noel/images_jpeg/"+indice);
    ui->labelImage->setPixmap(img);
    int score(0);
    int tempsMilisecondes = 20000;  // Indiquer 20 secondes au client

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

void WidgetPopSauceServeur::envoyerVerification(QTcpSocket *client,QString reponse)
{
    quint64 taille = 0;
    QBuffer tampon;
    QChar commande('F');

    // Normaliser la réponse de l'utilisateur et les réponses de la base de données
    QString reponseNorm = normaliserTexte(reponse);
    QString bReponseNorm = normaliserTexte(bReponse);
    QString alt1Norm = normaliserTexte(alt1);
    QString alt2Norm = normaliserTexte(alt2);

    bool bonneReponse = false;

    if (!reponseNorm.isEmpty()) {
        if (reponseNorm == bReponseNorm || reponseNorm == alt1Norm || reponseNorm == alt2Norm)
        {
            commande = 'V';
            bonneReponse = true;
            ui->textEdit->append("Bonne réponse !");

            // Ajouter 1 au score du client
            int index = getIndexClient(client);
            if (index != -1)
            {
                int scoreActuel = listeClients.at(index)->getScores();
                listeClients.at(index)->setScores(scoreActuel + 1);
            }
        }
        else
        {
            ui->textEdit->append("Mauvaise réponse !");
        }
    }

    // Construire et envoyer la trame V ou F au client
    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    out << taille << commande;
    taille = static_cast<quint64>(tampon.size()) - sizeof(taille);
    tampon.seek(0);
    out << taille;
    client->write(tampon.buffer());

    // Si bonne réponse : arrêter le timer, envoyer fin à tous, envoyer les scores à tous, passer directement à la prochaine question
    if (bonneReponse)
    {
        timer->stop();

        // Envoyer le classement (scores) à tous les clients
        envoyerScores();

        // Envoyer fin à tous les clients
        for (int i = 0; i < listeClients.size(); i++)
        {
            envoyerFin(listeClients.at(i)->getSockClient());
        }

        // Passer directement à la prochaine question (pas de délai 5s)
        envoyerProchaineQuestion();
    }
    // Si mauvaise réponse : on ne fait rien d'autre, le timer de 20s continue
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
    int nbQuestion;
  /*  QSqlQuery requete("SELECT COUNT(id_quest) FROM question;" );
    if (requete.exec()){
        while(requete.next())
        {
            nbQuestion=requete.value("COUNT(id_quest)").toInt();

            qDebug() <<"Nombre de question"<< nbQuestion;
        }
    }
    int nbRandom = rand() % nbQuestion + 1; //Nombre random entre 1 et 2 (seuleument changer le premier nb pas le deuxieme)*/
    QSqlQuery requetePrepare("SELECT texte_question, indice, reponse,option_a ,option_b FROM question order by rand() limit 1;");;

   // requetePrepare.prepare("SELECT texte_question, indice, reponse,option_a ,option_b FROM question WHERE id_quest = :id;");
    //requetePrepare.bindValue(":id",nbRandom);

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

QString WidgetPopSauceServeur::normaliserTexte(const QString &reponse)
{
    QString reponseNorm = reponse;

    // 1. minuscules
    reponseNorm = reponseNorm.toLower();

    static const QString accents  = "àâäáãåçèéêëìíîïñòóôöõùúûüýÿ";
    static const QString sansAcc   = "aaaaaaceeeeiiiinooooouuuuyy";

    for (int i = 0; i < accents.size(); ++i){
        reponseNorm.replace(accents[i], sansAcc[i]);
    }

    // 3. suppression des espaces
    reponseNorm.remove(' ');
    reponseNorm.remove('\t');
    reponseNorm.remove('\n');

    qDebug() << reponseNorm;

    return reponseNorm;
}

void WidgetPopSauceServeur::envoyerProchaineQuestion()
{
    // Récupérer une nouvelle question de la base de données
    bddQestion();

    // Envoyer la nouvelle question à tous les clients connectés
    for (int i = 0; i < listeClients.size(); i++)
    {
        envoyerQuestion(listeClients.at(i)->getSockClient());
    }

    // Démarrer le timer après avoir envoyé la question à tous les clients
    timer->start();
}

int WidgetPopSauceServeur::getIndexClient(QTcpSocket *client)
{
    // Initialisation des variables pour la recherche
    int i = 0;
    int index = -1;
    int tailleListe = listeClients.size();
    // Parcours de la liste des clients pour trouver celui correspondant à la socket
    // La condition i < tailleListe DOIT être en premier pour éviter un dépassement
    while (i < tailleListe && listeClients.at(i)->getSockClient() != client)
    {
        i++;
    }

    // Si le client a été trouvé, on retourne son index
    if (i < tailleListe)
    {
        index = i;
    }

    // Retourne l'index du client ou -1 si non trouvé
    return index;

}

void WidgetPopSauceServeur::envoyerScores()
{
    // Construire la trame de scores
    quint64 taille = 0;
    QBuffer tampon;
    QChar commande('S');
    int nbJoueurs = listeClients.size();

    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);

    out << taille << commande << nbJoueurs;

    // Trier les clients par score décroissant pour le classement
    QList<Client*> classement = listeClients;
    std::sort(classement.begin(), classement.end(), [](Client *a, Client *b) {
        return a->getScores() > b->getScores();
    });

    for (int i = 0; i < classement.size(); i++)
    {
        out << classement.at(i)->getNom() << classement.at(i)->getScores();
    }

    taille = static_cast<quint64>(tampon.size()) - sizeof(taille);
    tampon.seek(0);
    out << taille;

    // Envoyer à tous les clients
    for (int i = 0; i < listeClients.size(); i++)
    {
        listeClients.at(i)->getSockClient()->write(tampon.buffer());
    }
}
