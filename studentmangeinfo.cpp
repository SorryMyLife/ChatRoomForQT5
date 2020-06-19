#include "studentmangeinfo.h"

QString StudentMangeInfo::getIcon() const { return icon; }

void StudentMangeInfo::setIcon(const QString &value) { icon = value; }

QString StudentMangeInfo::getIp() const { return ip; }

void StudentMangeInfo::setIp(const QString &value) { ip = value; }

QString StudentMangeInfo::getPort() const { return port; }

void StudentMangeInfo::setPort(const QString &value) { port = value; }

QString StudentMangeInfo::getId() const { return id; }

void StudentMangeInfo::setId(const QString &value) { id = value; }

QString StudentMangeInfo::getNickName() const { return nickName; }

void StudentMangeInfo::setNickName(const QString &value) { nickName = value; }

QTcpSocket *StudentMangeInfo::getTcpSocket() const { return tcpSocket; }

void StudentMangeInfo::setTcpSocket(QTcpSocket *value) { tcpSocket = value; }

StudentMangeInfo::StudentMangeInfo() {}

StudentMangeInfo::StudentMangeInfo(QString nickName, QString id, QString port,
                                   QString ip, QString icon) {

  this->nickName = nickName;
  this->id = id;
  this->ip = ip;
  this->port = port;
  this->icon = icon;
}

StudentMangeInfo::StudentMangeInfo(QString nickName, QString id, QString port,
                                   QString ip) {
  this->nickName = nickName;
  this->id = id;
  this->ip = ip;
  this->port = port;
}

StudentMangeInfo::StudentMangeInfo(QString nickName, QString id, QString port,
                                   QString ip, QString icon,
                                   QTcpSocket *tcpSocket) {
  this->nickName = nickName;
  this->id = id;
  this->ip = ip;
  this->port = port;
  this->icon = icon;
  this->tcpSocket = tcpSocket;
}
