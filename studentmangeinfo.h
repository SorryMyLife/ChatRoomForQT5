#ifndef STUDENTMANGEINFO_H
#define STUDENTMANGEINFO_H

#include <QString>
#include <QTcpSocket>
/*
用于存放客户端信息
nickName : 用户昵称
id : 用户唯一标识码(类似QQ号)
port : 用户当前登录端口
ip : 用户当前登录IP
icon : 用户头像
tcpSocket : 用户当前tcpSocket
*/
class StudentMangeInfo {

public:
  QString nickName, id, port, ip, icon;
  QTcpSocket *tcpSocket;

public:
  StudentMangeInfo();
  StudentMangeInfo(QString nickName, QString id, QString port, QString ip,
                   QString icon);
  StudentMangeInfo(QString nickName, QString id, QString port, QString ip);

  StudentMangeInfo(QString nickName, QString id, QString port, QString ip,
                   QString icon, QTcpSocket *tcpSocket);

  QString getIcon() const;
  void setIcon(const QString &value);
  QString getIp() const;
  void setIp(const QString &value);
  QString getPort() const;
  void setPort(const QString &value);
  QString getId() const;
  void setId(const QString &value);
  QString getNickName() const;
  void setNickName(const QString &value);
  QTcpSocket *getTcpSocket() const;
  void setTcpSocket(QTcpSocket *value);
};

#endif // STUDENTMANGEINFO_H
