#include "mainwindow.h"

#include <QApplication>
#include <QByteArray>
int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  MainWindow w;
  //如果已经设置了自动登录配置文件，那么下面 这段可以被注释掉
  w.show();
  return a.exec();
}
