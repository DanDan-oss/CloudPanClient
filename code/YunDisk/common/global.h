#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef _DEBUG
#define WinPrintA qDebug() << "[ " << __FILE__ << ":"  << __LINE__ << " ] "
#define WinPrintW qDebug() << "[ " << __FILE__ << ":"  << __LINE__ << " ] "
#else
#define WinPrintA OutputDebugStringFA
#define WinPrintW OutputDebugStringFW
#endif


// 正则表达式
#define USER_REG        "^[a-zA-Z\\d_@#-\*]\{3,16\}$"
#define PASSWD_REG      "^[a-zA-Z\\d_@#-\*]\{6,18\}$"
#define PHONE_REG       "1\\d\{10\}"
#define EMAIL_REG       "^[a-zA-Z\\d\._-]\+@[a-zA-Z\\d_\.-]\+(\.[a-zA-Z0-9_-]\+)+$"
#define IP_REG          "((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)"
#define PORT_REG        "^[1-9]$|(^[1-9][0-9]$)|(^[1-9][0-9][0-9]$)|(^[1-9][0-9][0-9][0-9]$)|(^[1-6][0-5][0-5][0-3][0-5]$)"

#define LOGIN_SCENE_WINDOW_H 480     // 登录窗口界面高
#define LOGIN_SCENE_WINDOW_W 600     // 登录窗口界面宽

#define MAIN_SCENE_WINDOW_H 611     // 主窗口界面高
#define MAIN_SCENE_WINDOW_W 755     // 主窗口界面宽


#define WINDOW_TITLE_TEXT "YunDisk"
#define WINDOW_ICON_PATH ":/images/favicon.ico"
#define WINDOW_BACKUP_PATH ":/images/login2.jpg"
#define WINDOW_LOGO_PATH ":/images/logo.jpg"

#define CONFFILE        "conf/cfg.json"     // 配置文件
#define RECORDDIR       "conf/record/"      // 用户文件上传下载记录
#define FILETYPEDIR     "conf/fileType"     // 存放文件类型图片目录

#endif // GLOBAL_H
