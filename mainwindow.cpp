#include "mainwindow.h"
#include "ui_mainwindow.h"

void MainWindow::showLogin() {
  setWindowTitle("学生聊天室登录窗口");
  setFixedHeight(260);
  LoginIndexWindow *login = new LoginIndexWindow(this);
  login->showLayout();
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);
  showLogin();
}

MainWindow::~MainWindow() { delete ui; }
