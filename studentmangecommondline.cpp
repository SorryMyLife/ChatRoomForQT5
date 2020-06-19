#include "studentmangecommondline.h"

//获取服务端返回的TcpSocketMap
QMap<int, StudentMangeInfo> StudentMangeCommondLine::getTcpSocketMap() const {
  return tcpSocketMap;
}

void StudentMangeCommondLine::setTcpSocketMap(
    const QMap<int, StudentMangeInfo> &value) {
  tcpSocketMap = value;
}
//返回命令行帮助信息给客户端或者服务端
QString StudentMangeCommondLine::help() {
  return "add [IP:PORT] : add your new friends\r\n"
         "del [IP:PORT] or del [IP] : delete your ip friend or delete you all "
         "ip friend \r\n"
         "find [IP] or [PORT] or [IP:PORT] : fuzzy match or exact match \r\n"
         "list : return all friends\r\n"
         "sys [command] : run server command\r\n";
}

//检查命令是否存在问题
void StudentMangeCommondLine::checkCommond(QString command, QPlainTextEdit *t) {
  if (command.indexOf("help") != -1) {
    t->appendPlainText(help());
  } else {
    QProcess *process = new QProcess();

#ifdef Q_OS_WIN32
    process->start("cmd ", {"/Q"});
#else
    process->start("sh");
#endif
    if (process->waitForStarted()) {
      if (command.indexOf("sys") != -1) {
        process->write(QString(command.mid(3) + "\n").toUtf8());
      } else {
        process->write(QString(command + "\n").toUtf8());
      }
    } else {
      qDebug() << "cmd start failed !!!";
    }
    QObject::connect(process, &QProcess::readyReadStandardOutput, [=]() {
      t->appendPlainText(
          QString::fromLocal8Bit(process->readAllStandardOutput()));
    });

    QObject::connect(process, &QProcess::readyReadStandardError, [=]() {
      t->appendPlainText(
          QString::fromLocal8Bit(process->readAllStandardError()));
    });
  }
}

//检查命令应该响应到哪个客户端
void StudentMangeCommondLine::checkCommond(QString command, QTcpSocket *t) {

  if (command.indexOf("help") != -1) {
    t->write("startCMD : enable command line \r\nstopCMD : disable command "
             "line\r\n\r\n");
  }

  qDebug() << userInfoList.size();
  if (!isFind(t)) {
    UserInfo *info = new UserInfo(t->peerAddress().toString(),
                                  QString::number(t->peerPort()));
    userInfoList.append(info);
  }
  foreach (UserInfo *user, userInfoList) {
    qDebug() << user->getIsStartCMD();
    if (user->getIp().compare(t->peerAddress().toString()) == 0 &&
        user->getPort().compare(QString::number(t->peerPort())) == 0) {
      if (re_str(command).compare("startCMD") == 0) {
        user->setIsStartCMD(true);
        t->write("now startCMD!!!\r\n\r\n");
      }

      if (re_str(command).compare("stopCMD") == 0) {

        user->setIsStartCMD(false);
        t->write("now stopCMD!!!\r\n\r\n");
      }

      if (user->getIsStartCMD()) {
        if (command.indexOf("help") != -1) {
          t->write(help().toUtf8());
        }
        if (command.indexOf("sys") != -1) {
          QProcess *process = new QProcess();
#ifdef Q_OS_WIN32
          process->start("cmd", {"/Q"});
#else
          process->start("sh");
#endif
          if (process->waitForStarted()) {
            process->write(QString(command.mid(3) + "\n").toUtf8());
          } else {
            qDebug() << "cmd start failed !!!";
          }
          QObject::connect(process, &QProcess::readyReadStandardOutput, [=]() {
            t->write(process->readAllStandardOutput());
          });

          QObject::connect(process, &QProcess::readyReadStandardError, [=]() {
            t->write(process->readAllStandardError());
          });
        } else {
          t->write(MyCommand(command).toUtf8());
        }
      }
    }
  }
}

//执行私有命令
QString StudentMangeCommondLine::MyCommand(QString command) {
  if (command.mid(0, 3).compare("add") == 0) {
    qDebug() << "add :::";
    QString data = command.mid(3);
    tcpSocketMap.insert(tcpSocketMap.size(),
                        StudentMangeInfo("object", "username",
                                         data.split(":")[0], data.split(":")[1],
                                         "icon"));
    return "add ok\r\n";
  }
  if (command.mid(0, 3).compare("del") == 0) {
    qDebug() << "del :::";
    QString data = command.mid(3);

    if (!data.isEmpty()) {
      if (data.split(":").size() > 0) {
        QMap<int, StudentMangeInfo>::iterator it;
        for (it = tcpSocketMap.begin(); it != tcpSocketMap.end(); it++) {
          if (it.value().getIp().compare(data.split(":")[0]) == 0 &&
              it.value().getPort().compare(data.split(":")[0]) == 0) {
            tcpSocketMap.erase(it);
            it++;
          }
        }
      } else {
        QMap<int, StudentMangeInfo>::iterator it;
        for (it = tcpSocketMap.begin(); it != tcpSocketMap.end(); it++) {
          if (it.value().getIp().compare(data.split(":")[0]) == 0) {
            tcpSocketMap.erase(it);
            it++;
          }
        }
      }
    }
    return "del ok\r\n";
  }
  if (command.mid(0, 4).compare("find") == 0) {
    qDebug() << "find :::";
    QString data = command.mid(4);
    if (data.split(":").size() > 0) {
      QMap<int, StudentMangeInfo>::iterator it;
      for (it = tcpSocketMap.begin(); it != tcpSocketMap.end(); it++) {
        if (it.value().getIp().compare(data.split(":")[0]) == 0 &&
            it.value().getIp().compare(data.split(":")[1]) == 0) {
          return it.value().getIp() + " -- " + it.value().getPort();
        }
      }

    } else {
      QMap<int, StudentMangeInfo>::iterator it;
      for (it = tcpSocketMap.begin(); it != tcpSocketMap.end(); it++) {
        if (it.value().getIp().compare(data.split(":")[0]) == 0) {
          return it.value().getIp() + " -- " + it.value().getPort();
        }
        if (it.value().getIp().compare(data.split(":")[1]) == 0) {
          return it.value().getIp() + " -- " + it.value().getPort();
        }
      }
    }
    return "find ok\r\n";
  }
  if (command.mid(0, 4).compare("list") == 0) {
    qDebug() << "lsit :::";
    QString data;
    QMap<int, StudentMangeInfo>::iterator it;
    for (it = tcpSocketMap.begin(); it != tcpSocketMap.end(); it++) {
      data.append(it.value().getIp() + " -- " + it.value().getPort());
      data.append("\r\n");
    }

    return data;
  }
  return help();
}

//判断客户端是否存在
bool StudentMangeCommondLine::isFind(QTcpSocket *t) {
  foreach (UserInfo *u, userInfoList) {
    if (u->getIp().compare(t->peerAddress().toString()) == 0 &&
        u->getPort().compare(QString::number(t->peerPort())) == 0) {
      return true;
    }
  }
  return false;
}

StudentMangeCommondLine::StudentMangeCommondLine() {}

bool StudentMangeCommondLine::UserInfo::getIsStartCMD() const {
  return isStartCMD;
}

void StudentMangeCommondLine::UserInfo::setIsStartCMD(bool value) {
  isStartCMD = value;
}

QString StudentMangeCommondLine::UserInfo::getIp() const { return ip; }

void StudentMangeCommondLine::UserInfo::setIp(const QString &value) {
  ip = value;
}

QString StudentMangeCommondLine::UserInfo::getPort() const { return port; }

void StudentMangeCommondLine::UserInfo::setPort(const QString &value) {
  port = value;
}

StudentMangeCommondLine::UserInfo::UserInfo(bool isStartCMD, QString ip,
                                            QString port) {
  this->isStartCMD = isStartCMD;
  this->ip = ip;
  this->port = port;
}

StudentMangeCommondLine::UserInfo::UserInfo(QString ip, QString port) {
  this->ip = ip;
  this->port = port;
}
