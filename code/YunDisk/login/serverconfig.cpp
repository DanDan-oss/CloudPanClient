#include "serverconfig.h"

ServerConfig::ServerConfig(const QRect &rect, QWidget *parent)
    : QWidget{parent}
{
    this->initScene(rect);
    this->initShowData();
}

ServerConfig::~ServerConfig()
{

}

void ServerConfig::initScene(const QRect &rect)
{
    int iHeight=rect.height(), iWidget=rect.width();
    this->setGeometry(rect);

    this->setFont(QFont("Microsoft YaHei UI", 9, QFont::Bold, false));

    // 设置用户登录标签
    this->m_title.setParent(this);
    this->m_title.setText("服务器设置");
    this->m_title.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_title.setFont(QFont("华文新魏", 26, QFont::Bold, false));
    this->m_title.setGeometry(iWidget/7*2, 0, 200, 50);    // 带了show()

    this->m_server.setParent(this);
    this->m_server.setText("服务器地址:");
    this->m_server.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_server.setGeometry(iWidget/8*2, iHeight/5*1, 63, 25);
    this->m_server_address.setParent(this);
    this->m_server_address.setGeometry(iWidget/8*4, iHeight/5*1, 133, 25);

    this->m_port.setParent(this);
    this->m_port.setText("服务器端口:");
    this->m_port.setAlignment(Qt::AlignCenter);   // 设置文本居中
    this->m_port.setGeometry(iWidget/8*2, iHeight/5*2, 63, 25);
    this->m_server_port.setParent(this);
    this->m_server_port.setGeometry(iWidget/8*4, iHeight/5*2, 133, 25);

    this->m_button_ok.setParent(this);
    this->m_button_ok.setText("确定");
    this->m_button_ok.setGeometry(iWidget/7*2, iHeight/5*3 , 200, 50);
    this->m_button_ok.setStyleSheet("background-image: url(:/images/balckButton.png);font: 75 18pt \"新宋体\";color: rgb(255, 255, 255);");
    this->m_button_ok.setAutoRaise(true);
}

void ServerConfig::initShowData()
{
    this->m_server_address.setFocus();
    this->m_server_address.setText(this->m_server_info.ip);
    this->m_server_port.setText(this->m_server_info.port);
}
