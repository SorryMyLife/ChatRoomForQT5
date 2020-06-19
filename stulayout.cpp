#include "stulayout.h"

/**
核心类
主要用于操作客户端、服务端
*/

int stuLayout::getSelectServer() const { return selectServer; }

void stuLayout::setSelectServer(int value) { selectServer = value; }

QString stuLayout::getServerIp() const { return serverIp; }

void stuLayout::setServerIp(const QString &value) { serverIp = value; }

int stuLayout::getServerPort() const { return serverPort; }

void stuLayout::setServerPort(int value) { serverPort = value; }

int stuLayout::getClickState() const { return clickState; }

void stuLayout::setClickState(int value) { clickState = value; }

//发送消息到当前发消息的客户端
void stuLayout::sendMsgOnNowClient(QByteArray msg) {
  int change = 0;
  tcpSocket = NULL;
  QMapIterator<int, StudentMangeInfo> it(tcpSocketMap);
  while (it.hasNext()) {

    if (it.next().value().getPort().compare(QString::number(clientPort)) == 0) {
      //        qDebug() << "compare !!!" << clientPort;
      //        tcpSocket = it.next().value().getTcpSocket();
      tcpSocket = tcpList[change];
      break;
    }
    change++;
    //      qDebug() << "ip :: " << it.next().value().getIp()
    //               << " -- port :: " << it.next().value().getPort();
  }
  if (tcpSocket == NULL) {
    tcpSocket = tcpList[0];
  }
  tcpSocket->write(msg);
  qDebug() << "send to cliented";
}

//显示所有在线客户端
void stuLayout::showClientList() {
  if (clickShowClient > 0) {
    if (oldLen != tcpSocketMap.size()) {
      treeWidget->clear();
      item1 = new QTreeWidgetItem(treeWidget);
      QMapIterator<int, StudentMangeInfo> it(tcpSocketMap);
      while (it.hasNext()) {
        StudentMangeInfo info = it.next().value();
        item1->addChild(new QTreeWidgetItem({info.getIcon(), info.getNickName(),
                                             info.getIp(), info.getPort()}));
      }
    }
  } else {
    item1 = new QTreeWidgetItem(treeWidget);
    QMapIterator<int, StudentMangeInfo> it(tcpSocketMap);
    while (it.hasNext()) {
      StudentMangeInfo info = it.next().value();
      item1->addChild(new QTreeWidgetItem(
          {info.getIcon(), info.getNickName(), info.getIp(), info.getPort()}));
    }
    clickShowClient++;
  }

  oldLen = tcpSocketMap.size();
  treeWidget->show();
}

//返回客户端头部消息
QString stuLayout::getClientData(QTcpSocket *tcpSocket) {
  return ">>> client " + tcpSocket->peerAddress().toString() + ":" +
         QString::number(tcpSocket->peerPort());
}
//返回客户端头部消息
QString stuLayout::getClientData() {
  clientIP = tcpSocket->peerAddress().toString();
  clientPort = tcpSocket->peerPort();
  return ">>> client " + clientIP + ":" + QString::number(clientPort);
}
//在后台监听是否有客户端断开连接，如果有，则移除
void stuLayout::clientDiscon(QTcpSocket *tcpSocket) {
  QTimer *timer = new QTimer();
  timer->start(1000);
  clientIP = tcpSocket->peerAddress().toString();
  clientPort = tcpSocket->peerPort();
  QObject::connect(timer, &QTimer::timeout, [=]() {
    if (tcpSocket->error() ==
        QAbstractSocket::SocketError::RemoteHostClosedError) {
      int ccount = 0;
      if (tcpList.size() > 1) {
        tcpSocketMap.clear();
        for (int j = 0; j < tcpList.size(); j++) {
          if (tcpList[j]->peerPort() == 0 &&
              tcpList[j]->peerAddress().toString().isEmpty()) {
            tcpList.removeAt(j);
            connCount--;
          }
          tcpSocketMap.insert(
              ccount,
              StudentMangeInfo("object", userName,
                               QString::number(tcpList[j]->peerPort()),
                               tcpList[j]->peerAddress().toString(), "icon"));
          ccount++;
        }
      }

      timer->stop();
    }
  });
  timerList.append(timer);
}
// QTcpSocket信号槽
void stuLayout::tcpScoketConnectFun(QTcpSocket *tSocket) {
  clientIP = tcpSocket->peerAddress().toString();
  clientPort = tcpSocket->peerPort();
  //  qDebug() << "62   clienip ::: " << clientIP
  //           << " client port ::: " << clientPort;
  QObject::connect(tSocket, &QTcpSocket::readyRead, [=]() {
    clientIP = tSocket->peerAddress().toString();
    clientPort = tSocket->peerPort();
    //    qDebug() << "this tcpSocket connect fun client ip :: " << clientIP
    //             << " -- port :: " << clientPort;
    QByteArray arry = tSocket->readAll();
    QString data = QString::fromLocal8Bit(arry);
    if (data.compare("online") == 0) {
      foreach (QTcpSocket *tc, tcpList) {
        QString dattt = QString("%1--%2")
                            .arg(tc->peerAddress().toString())
                            .arg(tc->peerPort());
        sendMsgOnNowClient(dattt.toUtf8());
      }
    }

    if (arry[0] == 'm') {

      listWidget->addItem(new QListWidgetItem(getCurrentTime() + "\r\n" +
                                              getClientData(tSocket) + "\r\n"));

      data = getClientData(tSocket) + "\r\n";
      sendMSGForClient(data.toUtf8() + arry);
      QListWidgetItem *item = new QListWidgetItem();
      QLabel *imageLabel = new QLabel();
      QPixmap pix;
      pix.loadFromData(arry.remove(0, 1));
      imageLabel->setPixmap(pix);
      imageLabel->setFixedSize(pix.size());
      item->setSizeHint(pix.size());
      listWidget->addItem(item);
      listWidget->setItemWidget(item, imageLabel);

    } else if (arry[0] == 'f') {
      listWidget->addItem(new QListWidgetItem(getCurrentTime() + "\r\n" +
                                              getClientData(tSocket) + "\r\n"));

      QByteArray tt = arry;

      QString dd = QString(arry);
      //提取文件信息,解析传输过来数据的头部信息
      QString filename =
          "server-" + dd.section("##@@", 0, 0).section("f", 1, 1);

      tt = tt.remove(0, dd.indexOf(dd.section("##@@", 1, 1)));
      data = getClientData(tSocket) + "\r\n";
      QListWidgetItem *item = new QListWidgetItem();
      QPushButton *fileOpenButton = new QPushButton("file");
      QObject::connect(fileOpenButton, &QPushButton::clicked, [=]() {
        QFile file(filename);
        file.open(QFile::ReadWrite);
        file.write(tt);
        file.close();
        fileOpenButton->setText(filename + " downloaded");
        fileOpenButton->disconnect();
      });
      listWidget->addItem(item);
      listWidget->setItemWidget(item, fileOpenButton);
      sendMSGForClient(data.toUtf8() + arry);

    } else {
      listWidget->addItem(new QListWidgetItem(getClientData(tSocket) +
                                              " -- data : " + data + " -- " +
                                              getCurrentTime()));

      sendMSGForClient(
          QString(getClientData(tSocket) + "\r\n" + data).toUtf8());
    }

    if (getSelectServer() == 0) {
      //      studentMangeCommondLine->checkCommond(data, textBrowser);
      studentMangeCommondLine->checkCommond(data, tSocket);
    }
  });
}
//关闭所有QTcpSocket信号槽连接
void stuLayout::discon() {
  for (int i = 0; i < tcpList.size(); i++) {
    tcpList[i]->disconnect();
  }
  timerDiscon();
}
//初始化服务端
void stuLayout::initServer() {
  tcpServer->listen(QHostAddress(getServerIp()), getServerPort());
  QObject::connect(tcpServer, &QTcpServer::newConnection, [=]() {
    ClientTextEdit->setPlaceholderText("请输入需要向客户端发送的内容");
    connCount++;
    discon();
    tcpList.append(tcpServer->nextPendingConnection());
    tcpSocket = tcpServer->nextPendingConnection();
    for (int i = 0; i < tcpList.size(); i++) {
      tcpSocket = tcpList[i];
      tcpScoketConnectFun(tcpSocket);
      clientDiscon(tcpSocket);
    }
    tcpSocketMap.insert(connCount - 1,
                        StudentMangeInfo("object", userName,
                                         QString::number(clientPort), clientIP,
                                         "icon"));
    studentMangeCommondLine->setTcpSocketMap(tcpSocketMap);

    listWidget->addItem(getClientData(tcpSocket) + " -- connected " +
                        getCurrentTime());
  });
}
//初始化客户端
void stuLayout::initClient() {
  tcpSocket->connectToHost(getServerIp(), getServerPort());
  QObject::connect(tcpSocket, &QTcpSocket::connected, [=]() {
    ClientTextEdit->setPlaceholderText("请输入需要向服务端发送的内容");
  });
  QObject::connect(tcpSocket, &QTcpSocket::readyRead, [=]() {
    QByteArray arry = tcpSocket->readAll(), tmp;
    QString msg = QString::fromLocal8Bit(arry);

    tmp = arry;
    if (arry[28] == 'm') {
      listWidget->addItem(new QListWidgetItem(
          getCurrentTime() + "\r\n" +
          QString::fromLocal8Bit(arry.remove(28, arry.size())) + "\r\n"));

      QListWidgetItem *item = new QListWidgetItem();
      QLabel *imageLabel = new QLabel();
      QPixmap pix;
      pix.loadFromData(tmp.remove(0, 29));
      imageLabel->setPixmap(pix);
      imageLabel->setFixedSize(pix.size());
      item->setSizeHint(pix.size());

      listWidget->addItem(item);
      listWidget->setItemWidget(item, imageLabel);
    } else if (arry[28] == 'f') {
      QByteArray tt = arry;

      QString dd = QString(arry);
      listWidget->addItem(new QListWidgetItem(
          getCurrentTime() + "\r\n" +
          QString::fromLocal8Bit(arry.remove(28, arry.size())) + "\r\n"));

      QMessageBox::information(w, "title", dd.section("##@@", 0, 0),
                               QMessageBox::Yes);
      QString filename =
          "client-" + dd.section("##@@", 0, 0).section("f", 1, 1);

      tt = tt.remove(0, dd.indexOf(dd.section("##@@", 1, 1)) + 4);
      QListWidgetItem *item = new QListWidgetItem();
      QPushButton *fileOpenButton = new QPushButton("file");
      QObject::connect(fileOpenButton, &QPushButton::clicked, [=]() {
        QFile file(filename);
        file.open(QFile::ReadWrite);
        file.write(tt);
        file.close();
        fileOpenButton->setText(filename + " downloaded");
        fileOpenButton->disconnect();
      });
      listWidget->addItem(item);
      listWidget->setItemWidget(item, fileOpenButton);

    } else {

      listWidget->addItem(
          new QListWidgetItem(getCurrentTime() + "\r\n" + msg + "\r\n"));
    }
  });
}

void stuLayout::sendMSGForClient(QByteArray msg) {
  if (getSelectServer() == 0) {
    //如果是服务端，则将消息转发至所有在线客户端
    foreach (QTcpSocket *t, tcpList) { t->write(msg); }
  } else {
    //如果是客户端，则将消息发送至服务端
    tcpSocket->write(msg);
  }
}

//初始化控件、布局
void stuLayout::init() {

  if (selectServer == 0) {
    tcpServer = new QTcpServer();
  } else {
    tcpServer = NULL;
  }
  tcpSocket = new QTcpSocket();
  treeWidget = new QTreeWidget();

  ClientTextEdit = new QPlainTextEdit();
  startServer = new QPushButton("启动服务");
  sendMsg = new QPushButton("回复客户端消息");
  startClient = new QPushButton("启动客户端");
  showContactList = new QPushButton("显示在线用户");
  FileTransfer = new QPushButton("发送文件");
  ImageTransfer = new QPushButton("发送图片");
  vbox = new QVBoxLayout();
  ipEdit = new QLineEdit();
  portEdit = new QLineEdit();
  ipLabel = new QLabel("当前IP");
  portLabel = new QLabel("当前端口");

  listWidget = new QListWidget();

  sendMsg->setEnabled(false);
  ipEdit->setText(serverIp);
  studentMangeCommondLine = new StudentMangeCommondLine();
  portEdit->setText(QString::number(serverPort));
  if (getSelectServer() == 1) {
    sendMsg->setText("回复服务端消息");
  } else {
    sendMsg->setText("回复客户端消息");
  }
  connFun();
}

//显示布局、控件
void stuLayout::showLayout() {
  init();
  w = new QWidget();
  if (getSelectServer() == 0) {
    w->setWindowTitle("当前处于服务端");
    ipEdit->setReadOnly(true);
    startClient->setEnabled(false);
    ClientTextEdit->setPlaceholderText("这里是给客户端发送消息的位置");
    startClient->setText("重启服务器");
    startServer->setText("启动服务");
  } else {
    w->setWindowTitle("当前处于客户端");
    ipEdit->setReadOnly(false);
    startServer->setEnabled(false);
    startClient->setText("启动客户端");
    startServer->setText("重新连接");
    ClientTextEdit->setPlaceholderText("这里是给服务端发送消息的位置");
  }
  treeWidget->setColumnCount(2);
  treeWidget->setHeaderLabels({"头像", "昵称", "IP", "端口"});
  treeWidget->setWindowTitle("你的联系人们");
  treeWidget->setMaximumWidth(512);
  treeWidget->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  treeWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);

  QHBoxLayout *hbox = new QHBoxLayout();
  QHBoxLayout *hbox2 = new QHBoxLayout();
  hbox->addWidget(ipLabel);
  hbox->addWidget(ipEdit);
  hbox2->addLayout(hbox);
  hbox = new QHBoxLayout();
  hbox->addWidget(portLabel);
  hbox->addWidget(portEdit);
  hbox2->addLayout(hbox);
  vbox->addLayout(hbox2);
  hbox = new QHBoxLayout();
  hbox->addWidget(startServer);
  hbox->addWidget(startClient);
  hbox->addWidget(showContactList);
  vbox->addLayout(hbox);

  vbox->addWidget(listWidget);
  hbox = new QHBoxLayout();
  hbox->addWidget(FileTransfer);
  hbox->addWidget(ImageTransfer);

  vbox->addLayout(hbox);

  vbox->addWidget(ClientTextEdit);
  hbox = new QHBoxLayout();
  hbox->addWidget(sendMsg);
  hbox->setAlignment(Qt::AlignRight);
  vbox->addLayout(hbox);
  w->setLayout(vbox);
  w->show();
}
//控件响应事件信号槽
void stuLayout::connFun() {

  QObject::connect(startServer, &QPushButton::clicked, [=]() {
    //    qDebug() << clickState;
    startClient->setEnabled(true);
    setServerIp(re_str(ipEdit->text()));
    setServerPort(re_str(portEdit->text()).toInt());
    if (getSelectServer() == 0) {
      if (clickState == 0) {
        initServer();
        sendMsg->setEnabled(true);
        startServer->setText("关闭服务");
        clickState++;
      } else {
        sendMsg->setEnabled(false);
        tcpSocket->disconnectFromHost();
        tcpSocket->close();
        tcpServer->close();
        startServer->setText("启动服务");
        clickState = 0;
      }
    } else {
      disableButton();
      closeClient();
      initClient();
      enableButton();
    }
  });

  QObject::connect(startClient, &QPushButton::clicked, [=]() {
    //    qDebug() << clickState;
    startServer->setEnabled(true);
    setServerIp(re_str(ipEdit->text()));
    setServerPort(re_str(portEdit->text()).toInt());
    if (getSelectServer() == 0) {
      tcpServer->deleteLater();
      tcpServer = new QTcpServer();
      disableButton();
      closeServer();
      initServer();
      enableButton();
    } else {
      if (clickState == 0) {
        initClient();
        sendMsg->setEnabled(true);
        startClient->setText("退出");
        clickState++;
      } else {
        tcpSocket->close();
        w->close();
      }
    }
  });

  QObject::connect(sendMsg, &QPushButton::clicked, [=]() {
    if (sendFile == 1 || sendFile == 2) {
      sendMSGForClient(fileByte);
      ClientTextEdit->setPlainText(" ");
    } else {
      sendMSGForClient(ClientTextEdit->toPlainText().toUtf8());
    }
    ClientTextEdit->clear();
    sendFile = 0;
    fileByte.clear();
  });

  QObject::connect(showContactList, &QPushButton::clicked,
                   [=]() { showClientList(); });

  QObject::connect(FileTransfer, &QPushButton::clicked, [=]() {
    QString filepath = QFileDialog::getOpenFileName(
        w, "请选择需要发送的文件", QDir::currentPath(), "ALL (*.*)");
    qDebug() << filepath;
    QFile file(filepath);
    QFileInfo info(filepath);

    file.open(QFile::ReadOnly);
    fileByte = file.readAll();
    //添加文件头部信息
    QString fileHEAD = re_str(QString("%1##@@").arg(info.fileName()));
    qDebug() << fileHEAD;
    fileByte.prepend(fileHEAD.toUtf8());
    fileByte.prepend('f');
    sendFile = 1;
    ClientTextEdit->setPlaceholderText("请发送文件");
  });

  QObject::connect(ImageTransfer, &QPushButton::clicked, [=]() {
    qDebug() << "select image";
    QString filepath = QFileDialog::getOpenFileName(
        w, "请选择需要发送的文件", QDir::currentPath(),
        "Image (*.jpg *.png *.gif *.jpeg);;ALL (*.*)");

    QFile file(filepath);
    file.open(QFile::ReadOnly);
    fileByte = file.readAll();
    fileByte.prepend('m');

    sendFile = 2;
    ClientTextEdit->setPlaceholderText("请发送图片文件");
  });
}

//关闭服务端
void stuLayout::closeServer() {
  closeClient();
  tcpServer->close();
}
//关闭客户端端
void stuLayout::closeClient() {
  tcpSocket->disconnectFromHost();
  tcpSocket->close();
}

//不显示控件
void stuLayout::disableButton() {
  startServer->setEnabled(false);
  startClient->setEnabled(false);
  sendMsg->setEnabled(false);
}
//显示控件
void stuLayout::enableButton() {
  startServer->setEnabled(true);
  startClient->setEnabled(true);
  sendMsg->setEnabled(true);
}

//关闭计时器信号槽，防止多次调用与内存溢出
void stuLayout::timerDiscon() {
  foreach (QTimer *t, timerList) { t->disconnect(); }
  timerList.clear();
}

//返回当前时间
QString stuLayout::getCurrentTime() {
  return QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
}

stuLayout::stuLayout() {}

stuLayout::~stuLayout() { delete this; }
