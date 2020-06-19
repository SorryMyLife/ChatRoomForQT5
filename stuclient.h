#ifndef STUCLIENT_H
#define STUCLIENT_H

#include "stulayout.h"

/**
添加一个QComboBox用于选择客户端与服务端登录
编写一套客户端与服务端一体的聊天软件
支持文件传输、图像显示、用户信息增删改查、音频媒体播放与在线解析功能
支持好友列表显示与其他功能的实现
带有管理员授权的用户可享受比管理员低一级的远程命令操作
其他用户均以普通权限运行
可参考QQ

*/

class stuclient : public stuLayout {

public:
  stuclient();
};

#endif // STUCLIENT_H
