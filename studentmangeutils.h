#ifndef STUDENTMANGEUTILS_H
#define STUDENTMANGEUTILS_H

#include "studentmangeinfo.h"
#include <QDebug>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QLabel>
#include <QLineEdit>
#include <QList>
#include <QListIterator>
#include <QMap>
#include <QMapIterator>
#include <QMessageBox>
#include <QPlainTextEdit>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>

/*
主要核心类，用于操作一些常用的操作与头文件导入

*/

class StudentMangeUtils {

public:
  QString data;

public:
  QString cacheDir = "cache";
  QString jsonFilePath = cacheDir + "/conf.ini";

public:
  bool removeLocalConfiFile();
  QString readLocalFile(QString path);
  bool exists();
  void init();
  QString re_str(QString src);
  QJsonDocument readLocalJsonFile(QString path);
  QString getLocalJsonFileValue(QString key);
  bool writeLocalJsonFile(QString key, QString value);

public:
  StudentMangeUtils();
};

#endif // STUDENTMANGEUTILS_H
