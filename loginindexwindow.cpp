#include "loginindexwindow.h"

//判断当前默认登录是否为服务端
bool LoginIndexWindow::isServer() {

  if (getLocalJsonFileValue("server").compare("true") == 0) {
    comboBox->setCurrentIndex(0);
    return true;
  }
  comboBox->setCurrentIndex(1);

  return false;
}
//登录客户端、服务端，判断用户名与密码是否正确
bool LoginIndexWindow::login(QString username, QString userpwd) {

  if (username.compare("root") == 0 && userpwd.compare("123") == 0) {
    if (isServer()) {
      StuServer *server = new StuServer();
      server->showLayout();
    } else {
      stuclient *client = new stuclient();
      client->showLayout();
    }
    m->close();
    ww->close();

    return true;
  } else {
    QMessageBox::critical(m, "登录错误", "请确认密码或用户名正确",
                          QMessageBox::YesToAll);
  }
  return false;
}

//添加控件
void LoginIndexWindow::addWidget(QWidget *w1, QWidget *w2, QWidget *w3,
                                 QWidget *w4) {
  QHBoxLayout *hbox = new QHBoxLayout();
  QHBoxLayout *hbox1 = new QHBoxLayout();
  hbox->addWidget(w1);
  hbox->addWidget(w2);
  hbox->setAlignment(Qt::AlignLeft);
  hbox1->addLayout(hbox);
  hbox = new QHBoxLayout();
  hbox->addWidget(w3);
  hbox->addWidget(w4);
  hbox->setAlignment(Qt::AlignLeft);
  hbox1->addLayout(hbox);
  vbox->addLayout(hbox1);
}

//添加控件
void LoginIndexWindow::addWidget(QWidget *w1, QWidget *w2) {
  QHBoxLayout *hbox = new QHBoxLayout();
  hbox->addWidget(w1);
  hbox->addWidget(w2);
  vbox->addLayout(hbox);
}

//设置控件字体
void LoginIndexWindow::setFont(QList<QWidget *> widget, int h) {
  foreach (QWidget *w, widget) {
    w->setFont(QFont("华文琥珀"));
    w->setFixedHeight(h);
  }
}

//信号槽部分
void LoginIndexWindow::connectFun() {

  QObject::connect(loginButton, &QPushButton::clicked, [=]() {
    if (comboBox->currentIndex() == 0) {
      writeLocalJsonFile("server", "true");
      writeLocalJsonFile("client", "false");

    } else {
      writeLocalJsonFile("server", "false");
      writeLocalJsonFile("client", "true");
    }
    QString username = re_str(userNameEdit->text());
    QString userpwd = re_str(userPwdEdit->text());
    if (!check()) {
      StudentMangeUtils::init();
      qDebug() << "login";

      if (savePwd->checkState() == Qt::Checked &&
          autoLogin->checkState() == Qt::Checked) {
        writeLocalJsonFile("username", username);
        writeLocalJsonFile("userpwd", userpwd);
        writeLocalJsonFile("savepwd", "true");
        writeLocalJsonFile("autoLogin", "true");
      } else if (savePwd->checkState() == Qt::Checked) {
        writeLocalJsonFile("username", username);
        writeLocalJsonFile("userpwd", userpwd);
        writeLocalJsonFile("savepwd", "true");
        writeLocalJsonFile("autoLogin", "false");
      } else {
        writeLocalJsonFile("savepwd", "false");
        writeLocalJsonFile("autoLogin", "false");
      }
    }

    login(username, userpwd);
  });

  QObject::connect(exitButton, &QPushButton::clicked, [=]() { m->close(); });
}

//初始化控件
void LoginIndexWindow::init() {
  vbox = new QVBoxLayout();
  userNameEdit = new QLineEdit();
  userPwdEdit = new QLineEdit();
  savePwd = new QCheckBox();
  autoLogin = new QCheckBox();

  loginButton = new QPushButton("登录");
  exitButton = new QPushButton("退出");
  userName = new QLabel("用户名");
  userPwd = new QLabel("密码");
  savePwdLabel = new QLabel("保存密码");
  autoLoginLabel = new QLabel("自动登录");
  comboBox = new QComboBox();
  userNameEdit->setPlaceholderText("请输入管理员用户名");
  userPwdEdit->setPlaceholderText("请输入管理员密码");
  userPwdEdit->setEchoMode(QLineEdit::EchoMode::Password);
  comboBox->addItems({"服务端", "客户端"});
  setFont({userNameEdit, userPwdEdit}, 50);
  setFont({userName, userPwd, savePwdLabel, autoLoginLabel}, 50);
  connectFun();
}

//判断关键配置文件信息参数是否齐全
bool LoginIndexWindow::check() {
  isServer();
  if (exists()) {

    if (getLocalJsonFileValue("savepwd").compare("true") == 0) {
      QString username = getLocalJsonFileValue("username");
      QString userpwd = getLocalJsonFileValue("userpwd");
      userNameEdit->setText(username);
      userPwdEdit->setText(userpwd);
      savePwd->setCheckState(Qt::Checked);
      if (getLocalJsonFileValue("autoLogin").compare("true") == 0) {
        autoLogin->setCheckState(Qt::Checked);
        loginButton->setText("登录中...");
        login(username, userpwd);
        //        ww->close();
        //        m->close();
      }

      return true;
    }
  }

  return false;
}

//显示登录界面的主要布局控件
void LoginIndexWindow::showLayout() {
  init();
  ww = new QWidget();
  m->setCentralWidget(ww);
  m->close();
  vbox->addWidget(comboBox);
  addWidget(userName, userNameEdit);
  addWidget(userPwd, userPwdEdit);
  addWidget(savePwdLabel, savePwd, autoLoginLabel, autoLogin);
  addWidget(loginButton, exitButton);
  if (!check()) {
    ww->setLayout(vbox);
  }
  //  ww->setLayout(vbox);
}

LoginIndexWindow::LoginIndexWindow() {}

LoginIndexWindow::LoginIndexWindow(QMainWindow *m) { this->m = m; }
