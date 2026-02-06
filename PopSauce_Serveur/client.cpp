#include "client.h"


Client::Client() : scores(0), sockClient(nullptr)
{

}

Client::~Client()
{

}

int Client::getScores() const
{
    return scores;
}

void Client::setScores(int newScores)
{
    scores = newScores;
}

QString Client::getNom() const
{
    return nom;
}

void Client::setNom(const QString &newNom)
{
    nom = newNom;
}

QString Client::getEmail() const
{
    return email;
}

void Client::setEmail(const QString &newEmail)
{
    email = newEmail;
}

QTcpSocket *Client::getSockClient() const
{
    return sockClient;
}

void Client::setSockClient(QTcpSocket *newSockClient)
{
    sockClient = newSockClient;
}
