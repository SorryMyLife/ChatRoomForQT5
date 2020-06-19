#ifndef LOGININDEXWINDOW_H
#define LOGININDEXWINDOW_H

#include "studentmangeutils.h"

#include "stuclient.h"
#include "stuserver.h"
#include <QCheckBox>
#include <QComboBox>
#include <QMainWindow>
#include <QWidget>
class LoginIndexWindow : public StudentMangeUtils {

public:
  QVBoxLayout *vbox;
  QLineEdit *userNameEdit, *userPwdEdit;
  QCheckBox *savePwd, *autoLogin;
  QPushButton *loginButton, *exitButton;
  QLabel *userName, *userPwd, *savePwdLabel, *autoLoginLabel;
  QMainWindow *m;
  QWidget *ww;
  QComboBox *comboBox;

public:
  bool isServer();
  bool login(QString username, QString userpwd);
  void addWidget(QWidget *w1, QWidget *w2, QWidget *w3, QWidget *w4);
  void addWidget(QWidget *w1, QWidget *w2);
  void setFont(QList<QWidget *> widget, int h);
  void connectFun();
  void init();
  bool check();
  void showLayout();

public:
  LoginIndexWindow();
  LoginIndexWindow(QMainWindow *m);
};

#endif // LOGININDEXWINDOW_H
