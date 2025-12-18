#ifndef WIDGETPOPSAUCECLIENT_H
#define WIDGETPOPSAUCECLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QBuffer>

QT_BEGIN_NAMESPACE
namespace Ui { class WidgetPopSauceClient; }
QT_END_NAMESPACE

class WidgetPopSauceClient : public QWidget
{
    Q_OBJECT

public:
    WidgetPopSauceClient(QWidget *parent = nullptr);
    ~WidgetPopSauceClient();
    void envoyerDonnees();
    void envoyerInscription(QString pseudo, QString mail, QString mdp, QString mdp2);
    void envoyerConnexionCompte(QString pseudo, QString mdp);
    void envoyerInscriptionCompte(QString pseudo, QString mdp);
private slots:
    void on_pushButtonConnexion_clicked();
    void onQTcpSocket_connected();
    void onQTcpSocket_disconnected();
    void onQTcpSocket_readyRead();
    void onQTcpSocket_errorOccured(QAbstractSocket::SocketError socketError);
    void on_pushButtonEnvoyer_clicked();
    void on_pushButtonInscription_clicked();
    void on_pushButtonConnexionCompte_clicked();
    void on_pushButtoninscrire_clicked();
private:
    Ui::WidgetPopSauceClient *ui;
    QTcpSocket socketJoueur;
    bool enCoursDeLecture=false;
    quint64 tailleAttendue=0;

    QString pseudo, mail, mdp, verifMdp;
};
#endif // WIDGETPOPSAUCECLIENT_H
