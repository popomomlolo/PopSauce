# PopSauce
                    COMANDE SQL

                    
Select la question + img;

SELECT question, indice FROM "question" WHERE id_quest = :id;


Select la reponse seuleument

SELECT reponse FROM "question" WHERE id_quest = :id;


====================================================================================================================

  Fonction pour recuperer une Question et son indice avec le meme ID


#include <QRandomGenerator>
//srand(time(nullptr)); DANS LE CONSTRUCTEUR
    void WidgetPopSauceServeur::recupererQuestionIndice()
    {
    int nbRandom = rand() % 2 + 1; //Nombre random entre 1 et 2 (seuleument changer le premier nb pas le deuxieme)
    
    QSqlQuery requetePrepare;
    requetePrepare.prepare("SELECT texte_question, indice, reponse FROM "question" WHERE id_quest = :id;");

    //requetePrepare.bindValue(":id",nbRandom);
    if (!requetePrepare.exec()){
        qDebug()<<"pb requete "<<requetePrepare.lastError();
    }

    QString question, indice;
    while(requetePrepare.next())
    {
        question=requetePrepare.value("texte_question").toString();
        indice=requetePrepare.value("indice").toString();
        reponse=requetePrepare.value("reponse").toString();
        
        qDebug()<<question << indice << reponse;
    }
    }
===================================================================

    void Widget::normaliser()
    {
    QString reponseNorm = "AzeRtyUé2éÊAa esCap         Tabuhgu";

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

    qDebug() << reponseNorm;}

  ============================================================
  
    {
      int nbQuestion;
      QSqlQuery requete("SELECT COUNT(id_quest) FROM question;" );
        if (requete.exec()){
            while(requete.next())
            {  
                  nbQuestion=requete.value("COUNT(id_quest)").toInt();

                qDebug() <<"Nombre de question"<< nbQuestion;
            }
        }}




readyread
in>>commande;
            switch (commande.toLatin1()) {
            case 'R':
                in>>reponse;
                qDebug() << "reponse utilisateur "<<reponse;
                ui->textEdit->append("reponse utilisateur "+reponse);
                envoyerVérification(client,reponse);
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
            




    QChar commande='R';

    tampon.open(QIODevice::WriteOnly);
    QDataStream out(&tampon);
    out<<taille<<commande<<reponse;



====================================================================================

            void WidgetPopSauceServeur::inscrireClient(QString pseudo, QString bemail, QString bmdp, QString verifMdp)
            {
            if(mdp == verifMdp){

            QSqlQuery requetePrepare;
    
    
            requetePrepare.prepare("INSERT INTO utilisateurs (`nom_utilisateur`, `mot_de_passe`, `email`, `score`) VALUES (:bpseudo, :bmdp, :bemail, '0');");
            requetePrepare.bindValue(":bpseudo",pseudo);
            requetePrepare.bindValue(":bmdp",bmdp);
            requetePrepare.bindValue(":bemail",bemail);
    
            if (!requetePrepare.exec()){
                qDebug()<<"pb requete "<<requetePrepare.lastError();
            }
    }
    }
