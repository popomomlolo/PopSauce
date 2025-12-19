#ifndef WIDGETPOPSAUCECLIENT_H
#define WIDGETPOPSAUCECLIENT_H

#include <QWidget>
#include <QTcpSocket>
#include <QBuffer>
#include <QTimer>

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
    void updateCompteur();
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
    QTimer *monTimer;
    int tempsRestant;
};
#endif // WIDGETPOPSAUCECLIENT_H
