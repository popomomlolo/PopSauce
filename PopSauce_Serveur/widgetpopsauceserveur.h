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
private:
    Ui::WidgetPopSauceServeur *ui;
    QTcpServer sockEcoute;
    QList<QTcpSocket*> listeDesClients;
    void envoyerMessage(QTcpSocket *emetteur, QPixmap image);
};
#endif // WIDGETPOPSAUCESERVEUR_H
