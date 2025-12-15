# PopSauce
                    COMANDE SQL

                    
Select la question + img;

SELECT question, indice FROM "question" WHERE id_quest = :id;


Select la reponse seuleument

SELECT reponse FROM "question" WHERE id_quest = :id;


====================================================================================================================

  Fonction pour recuperer une Question et son indice avec le meme ID


    void WidgetPopSauceServeur::recupererQuestionIndice()
    {
    QSqlQuery requetePrepare;
    requetePrepare.prepare("SELECT question, indice FROM "question" WHERE id_quest = :id;");

    //requetePrepare.bindValue(":id",nbRandom);
    if (!requetePrepare.exec()){
        qDebug()<<"pb requete "<<requetePrepare.lastError();
    }

    QString question, indice;
    while(requetePrepare.next())
    {
        question=requetePrepare.value("question").toString();
        indice=requetePrepare.value("indice").toString();
        qDebug()<<question << indice;
    }
    }
