#ifndef WIDGETPOPSAUCESERVEUR_H
#define WIDGETPOPSAUCESERVEUR_H

#include <QWidget>
#include <QTcpServer>
#include <QTcpSocket>
#include <QBuffer>
#include <QPixmap>
#include <QSqlDatabase>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlQuery>
#include <QSettings>
#include <QFileInfo>
#include <QRandomGenerator>
#include <QTimer>
#include "client.h"

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetPopSauceServeur; }
QT_END_NAMESPACE

class WidgetPopSauceServeur : public QWidget
{
    Q_OBJECT

public:
    WidgetPopSauceServeur(QWidget *parent = nullptr);
    ~WidgetPopSauceServeur();
private slots:
    void on_pushButtonLancer_clicked();
    void onQTcpServer_newConnection();
    void onQTcpSocket_disconnected();
    void onQTcpSocket_readyRead();
    void onQTcpSocket_errorOccured();
    void onQTcpSocket_connected();
    void onTimer_timeout();
private:
    Ui::WidgetPopSauceServeur *ui;
    QTcpServer sockEcoute;
    QList <Client *> listeClients;
    QString bReponse,question,indice,alt1,alt2,reponseNorm;
    QTimer *timer;
    void envoyerQuestion(QTcpSocket *client);
    void envoyerVerification(QTcpSocket *client, QString reponse);
    void envoyerFin(QTcpSocket *client);
    void bddQestion();
    void normaliser(QString reponse);
    int getIndexClient(QTcpSocket *client);
};
#endif // WIDGETPOPSAUCESERVEUR_H
