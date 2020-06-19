#ifndef STULAYOUT_H
#define STULAYOUT_H

#include "studentmangecommondline.h"
#include "studentmangeinfo.h"
#include "studentmangeutils.h"
#include <QDateTime>
#include <QFileDialog>
#include <QKeyEvent>
#include <QListView>
#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QStandardItemModel>
#include <QString>
#include <QTcpServer>
#include <QTcpSocket>
#include <QTimer>

/*

ClientTextEdit : 服务/客户端用户编辑消息控件
startServer :
            在当前设置为服务端时，此控件担任启动服务端功能
            在当前设置为客户端时，此控件担任重启客户端功能
startClient :
            在当前设置为客户端时，此控件担任启动客户端功能
            在当前设置为服务端时，此控件担任重启服务端功能
sendMsg : 服务/客户端用户发送消息控件

showContactList : 显示在线人员控件

FileTransfer : 文件选择控件

ImageTransfer : 图片文件选择控件

tcpSocketMap : 用于保存StudentMangeInfo对象

tcpList : 用于存放连接过的QTcpSocket客户端


*/

class stuLayout : public StudentMangeUtils {
private:
  QPlainTextEdit *ClientTextEdit;
  QPushButton *startServer, *sendMsg, *showContactList, *startClient,
      *FileTransfer, *ImageTransfer;
  QWidget *w;
  QVBoxLayout *vbox;
  QTcpServer *tcpServer;
  QTcpSocket *tcpSocket;
  QMap<int, StudentMangeInfo> tcpSocketMap;
  QLineEdit *ipEdit, *portEdit;
  QLabel *ipLabel, *portLabel;
  QList<QTcpSocket *> tcpList;
  QList<QTimer *> timerList;
  QTreeWidget *treeWidget;
  QTreeWidgetItem *item1;
  StudentMangeCommondLine *studentMangeCommondLine;
  QListWidget *listWidget;

private:
  QByteArray fileByte;
  QString clientDataMsg;
  QString clientIP;
  QString userName = "admin";
  QString serverIp = "127.0.0.1";
  int clientPort = 53665;
  int clickState = 0;
  int serverPort = 53665;
  int connCount = 0;
  int selectServer = 0;
  int oldLen = 0;
  int clickShowClient = 0;
  int sendFile = 0;

public:
  void sendMsgOnNowClient(QByteArray msg);
  void showClientList();
  QString getClientData(QTcpSocket *tcpSocket);
  QString getClientData();
  void clientDiscon(QTcpSocket *tcpSocket);
  void tcpScoketConnectFun(QTcpSocket *tcpSocket);
  void discon();
  void initServer();
  void initClient();
  void sendMSGForClient(QByteArray msg);
  void init();
  void showLayout();
  void connFun();
  void closeServer();
  void closeClient();
  void disableButton();
  void enableButton();
  void timerDiscon();
  QString getCurrentTime();

public:
  stuLayout();
  ~stuLayout();
  int getSelectServer() const;
  void setSelectServer(int value);
  QString getServerIp() const;
  void setServerIp(const QString &value);
  int getServerPort() const;
  void setServerPort(int value);
  int getClickState() const;
  void setClickState(int value);
};

#endif // STULAYOUT_H
