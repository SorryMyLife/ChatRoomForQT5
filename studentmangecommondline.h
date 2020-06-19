#ifndef STUDENTMANGECOMMONDLINE_H
#define STUDENTMANGECOMMONDLINE_H

#include "studentmangeinfo.h"
#include "studentmangeutils.h"
#include <QDebug>
#include <QHostAddress>
#include <QMap>
#include <QPlainTextEdit>
#include <QProcess>
#include <QString>
#include <QTcpSocket>
class StudentMangeCommondLine : public StudentMangeUtils {

protected:
  class UserInfo {
  public:
    bool isStartCMD = false;
    QString ip, port;

  public:
    bool getIsStartCMD() const;
    void setIsStartCMD(bool value);
    QString getIp() const;
    void setIp(const QString &value);
    QString getPort() const;
    void setPort(const QString &value);
    UserInfo();
    UserInfo(bool isStartCMD, QString ip, QString port);
    UserInfo(QString ip, QString port);
  };

private:
  QMap<int, StudentMangeInfo> tcpSocketMap;
  QList<UserInfo *> userInfoList;
  int startCmd = 0;

public:
  QString help();
  void checkCommond(QString command, QPlainTextEdit *t);
  void checkCommond(QString command, QTcpSocket *t);
  QString MyCommand(QString command);

private:
  bool isFind(QTcpSocket *t);

public:
  StudentMangeCommondLine();
  QMap<int, StudentMangeInfo> getTcpSocketMap() const;
  void setTcpSocketMap(const QMap<int, StudentMangeInfo> &value);
};

#endif // STUDENTMANGECOMMONDLINE_H
