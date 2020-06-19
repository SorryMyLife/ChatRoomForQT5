#include "studentmangeutils.h"

//移除配置文件
bool StudentMangeUtils::removeLocalConfiFile() {
  QFile file(jsonFilePath);
  file.open(QFile::ReadWrite);
  if (file.exists()) {
    return file.remove();
  }
  return false;
}

//读取配置文件
QString StudentMangeUtils::readLocalFile(QString path) {
  QFile file(path);
  file.open(QFile::ReadOnly);
  if (file.exists()) {
    return QString::fromLocal8Bit(file.readAll());
  }
  return "";
}

//判断文件是否存在
bool StudentMangeUtils::exists() {
  QFile file(jsonFilePath);
  file.open(QFile::ReadOnly);
  return file.exists();
}

//初始化
void StudentMangeUtils::init() {
  QDir dir(cacheDir);
  QFile file(jsonFilePath);
  file.open(QFile::WriteOnly | QFile::ReadOnly);
  if (!dir.exists()) {
    qDebug() << "dir not found !!!";
    dir.mkpath(cacheDir);
  }
  if (!file.exists()) {
    file.write("");
    file.flush();
    qDebug() << "create file ok 1!!";
  }
  file.close();
}
//移除字符串内特殊字符
QString StudentMangeUtils::re_str(QString src) {
  return src.replace(QRegExp("\\s+"), "");
}

//将读取到的配置文件信息以QJsonDocument返回
QJsonDocument StudentMangeUtils::readLocalJsonFile(QString path) {
  return QJsonDocument::fromJson(readLocalFile(path).toUtf8());
}
//获取配置文件键值
QString StudentMangeUtils::getLocalJsonFileValue(QString key) {
  QJsonDocument json = readLocalJsonFile(jsonFilePath);
  if (json.toJson().size() > 0) {

    return json[key].toString();
  }
  return "";
}

//写入新的参数到配置文件
bool StudentMangeUtils::writeLocalJsonFile(QString key, QString value) {

  QFile file(jsonFilePath);
  file.open(QFile::ReadWrite);
  if (file.exists()) {

    QJsonDocument json = readLocalJsonFile(jsonFilePath);
    QJsonObject jsonObj = json.object();
    jsonObj.insert(key, value);
    json.setObject(jsonObj);
    file.write(json.toJson());
    file.flush();
    file.close();
    return true;
  }
  file.close();
  return false;
}

StudentMangeUtils::StudentMangeUtils() { init(); }
