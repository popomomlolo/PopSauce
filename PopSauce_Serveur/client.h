#ifndef CLIENT_H
#define CLIENT_H

#include <QTcpSocket>

class Client
{
public:
    Client();
    ~Client();
    int getScores() const;
    void setScores(int newScores);
    QString getNom() const;
    void setNom(const QString &newNom);
    QString getEmail() const;
    void setEmail(const QString &newEmail);
    QTcpSocket *getSockClient() const;
    void setSockClient(QTcpSocket *newSockClient);

private:
    int scores;
    QString nom;
    QString email;
    QTcpSocket *sockClient;
};

#endif // CLIENT_H
