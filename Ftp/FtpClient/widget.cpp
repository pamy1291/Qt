#include "widget.h"
#include "ui_widget.h"
#include <QHostAddress>
#include <QMessageBox>
#include <QThread>
#include <QColor>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    //控件初始化
    ui->lineEdit_port->setText("21");
    ui->lineEdit_ip->setText("192.168.43.195");
    ui->lineEdit_user->setText("pamy");
    ui->lineEdit_pass->setText("135141");
    ui->pushButton_entry_srv->setEnabled(false);
    ui->pushButton_entry_cli->setEnabled(false);
    ui->pushButton_get->setEnabled(false);
    ui->pushButton_put->setEnabled(false);

    //建立tcp客户端socket
    tcpSocket = new QTcpSocket;
    tcpCmdSocket = new QTcpSocket;
    buf = new char[MAX_SIZE];

    //绑定相关信号
    connect(tcpSocket, SIGNAL(connected()), this, SLOT(on_connected())); //命令通道
    connect(tcpCmdSocket, SIGNAL(connected()), this, SLOT(on_connected_cmd())); //数据通道
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(on_readyRead())); //命令通道具有可读信息
    connect(tcpCmdSocket, SIGNAL(readyRead()), this, SLOT(on_readyRead_cmd())); //数据通道具有可读信息

    //双击目录时
    connect(ui->listWidget_server, &QListWidget::itemDoubleClicked, this, &Widget::handleServerItemDoubleClicked);
    connect(ui->listWidget_client, &QListWidget::itemDoubleClicked, this, &Widget::handleClientItemDoubleClicked);

    /*第一次连接服务器时发送用户名和密码*/
    user_flag = true;
    pass_flag = true;

    status = -1;
    put_flag = false;
    get_flag = false;
}

Widget::~Widget()
{
    delete[] buf;
    delete tcpCmdSocket;
    delete tcpSocket;
    delete ui;
}

/*发送指令*/
void Widget::send_cmd()
{
    buf[strlen(buf)] = '\0';
    qDebug("send:%s",buf);

    tcpSocket->write(buf, strlen(buf));
    //tcpSocket->waitForBytesWritten(); //等待发送完成

    //放入ListWidget
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget_tips);
    buf[strlen(buf)-1] = '\0';
    item->setText(buf);
    item->setTextColor(QColor(0, 0, 0)); //黑色
}

/*打开数据通道*/
void Widget::ftpPasvOpen()
{
    sprintf(buf, "PASV\n");
    send_cmd();
}

/*连接数据通道*/
void Widget::ftpPasvLink(quint16 port)
{
    tcpCmdSocket->connectToHost(QHostAddress(ui->lineEdit_ip->text()), port);
}

/*关闭数据通道*/
void Widget::ftpPasvClose()
{
    tcpCmdSocket->close();
}

/**********************文件操作************************/
void Widget::ftpPwd()
{
    strcpy(this->buf, "PWD\n");
    send_cmd();
}

void Widget::ftpLs()
{
    ftpPasvOpen(); //开启数据通道
    ui->listWidget_server->clear(); //清空再显示
    sprintf(buf, "LIST\n");
    send_cmd();
}

void Widget::ftpDel(const char* filename)
{
    sprintf(buf, "DELE %s\n",filename);
    send_cmd();
}

void Widget::ftpMkdir(const char *dir)
{
    sprintf(buf, "MKD %s\n",dir);
    send_cmd();
}
void Widget::ftpRmdir(const char *dir)
{
    sprintf(buf, "RMD %s\n",dir);
    send_cmd();
}
void Widget::ftpCd(const char* path)
{
    sprintf(buf, "CWD %s\n", path);
    send_cmd();
}

/*将数据通道中的文件解析保存*/
void Widget::fileExplain(const char* buf)
{

    int ret = strlen(buf); //所有文件信息个数
    int cnt = 0; //当前解析文件所在位置
    char token[256] = {}; //一行文件信息
    while(cnt < ret)
    {
        strcpy(token, strtok(const_cast<char*>(buf+cnt), "\n")); //解析一行文件
        cnt += strlen(token)+1;
        token[strlen(token)-1] = '\0';
        qDebug("%s", token); //调试一行解析结果

        //文件名解析
        char file_name[128] = {};
        char mode[11] = {};
        sscanf(token, "%s %*d %*d %*d %*d %*s %*s %*s %s",mode, file_name);

        //图片
        QIcon dir("dir.png");
        QIcon file("file.png");
        //放入ListWidget
        QListWidgetItem* item = new QListWidgetItem(mode[0]=='d'?dir:file, file_name);
        ui->listWidget_server->addItem(item);
    }

}
/*连接ftp服务端*/
void Widget::on_pushButton_link_clicked()
{
    tcpSocket->close(); //关闭当前连接在建立新连接
    tcpSocket->connectToHost(QHostAddress(ui->lineEdit_ip->text()), ui->lineEdit_port->text().toUShort());
    user_flag = true;
    pass_flag = true;
    //QMessageBox::information(this,"提示信息","服务器繁忙");
}

void Widget::on_connected()
{
    qDebug("连接成功！");
    ui->pushButton_entry_srv->setEnabled(true);
    ui->pushButton_entry_cli->setEnabled(true);

    //本地客户端文件内容显示
    QDir dir;
    QString currentPath = dir.currentPath(); //获取当前本地路径
    currentPath += '/';
    //QString currentPath = "E:/QT/";
    ui->lineEdit_cli_path->setText(currentPath); //显示当前本地路径
    clientDirShow(currentPath.toStdString().c_str());

    //listWidget右键菜单策略，使能出现自定义的右键菜单
    ui->listWidget_server->setContextMenuPolicy(Qt::CustomContextMenu);
    ui->listWidget_client->setContextMenuPolicy(Qt::CustomContextMenu);
}

void Widget::on_connected_cmd()
{
    qDebug("数据通道连接成功！");
}

/*接收命令通道数据*/
void Widget::on_readyRead()
{
    char buf[MAX_SIZE];
    qint64 ret = tcpSocket->read(buf, sizeof(buf));
    buf[ret] = '\0';
    strcpy(this->buf, buf);
    qDebug("recv:%s",this->buf); //打印调试

    //放入ListWidget
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget_tips);
    this->buf[strlen(this->buf)-1] = '\0'; //去掉换行
    item->setText(this->buf);
    item->setTextColor(QColor(0, 226, 0)); //绿色

    sscanf(this->buf, "%d", &status); //获取状态码

    if(user_flag) //发送用户姓名
    {
        sprintf(this->buf, "USER %s\n",ui->lineEdit_user->text().toStdString().c_str());
        send_cmd();
        user_flag = false;
        return;
    }
    if(pass_flag) //发送密码
    {
        sprintf(this->buf, "PASS %s\n",ui->lineEdit_pass->text().toStdString().c_str());
        send_cmd();

        pass_flag = false;
        return;
    }

    /*状态(接收命令回传属于哪个)选择*/
    quint8 port1,port2;
    quint16 port; //数据通道端口
    switch(status)
    {
    case 230: //密码正确后显示主目录
        ftpPwd();
        return;
    case 257: //存储服务器路径（PWD命令接收结果）并发送显示命令
        sscanf(this->buf, "%*d \"%s",server_path);//丢弃前面数字空格和第一个" 保存路径
        server_path[strlen(server_path)-1] = '/';
        ui->lineEdit_srv_path->setText(server_path); //显示服务器当前路径
        ftpLs(); //ls
        return;
    case 227: //数据通道端口解析
        sscanf(buf, "227 Entering Passive Mode (%*d,%*d,%*d,%*d,%hhu,%hhu)",&port1, &port2);//解析端口
        port = port1*256 + port2;
        ftpPasvLink(port); //连接数据通道
        return;
    case 250: //cd结果 目录变化时显示当前目录所有文件
        ftpPwd(); //更新当前目录
        return;
    case 150:
    //case 226:
        if(put_flag) //上传
        {
            qDebug("正在上传！");
            putFile();
            ftpPwd(); //更新服务端显示的文件数
        }
        return;
    case 421: //timeout
        user_flag = true;
        pass_flag = true;
        return;
    default:
        qDebug("status:%d",status);
        return;
    }
}
/*命令通道接收数据*/
void Widget::on_readyRead_cmd()
{
    QString buf;
    buf = tcpCmdSocket->readAll();

    switch(status)
    {
    case 150: //命令通道的状态码，数据通道开始发送数据
    //case 226: //命令通道的状态码，数据发送完毕
        if(!put_flag && !get_flag) //ls数据处理
            fileExplain(buf.toStdString().c_str());
        else if(get_flag) //下载
        {
            qDebug("正在下载！");
            getFile(buf.toStdString().c_str());
            clientDirShow(ui->lineEdit_cli_path->text().toStdString().c_str()); //更新本地显示文件数量
        }
        break;
    default:
        qDebug("cmd_status:%d",status);
        break;
    }
    //接收完数据后关闭当前数据通道
    tcpCmdSocket->close();
}

/*服务端双击文件,去到所点击的文件路径*/
void Widget::handleServerItemDoubleClicked(QListWidgetItem *item)
{
    QListWidgetItem item_copy = *item;
    ui->listWidget_server->clear(); //清空前目录显示的文件
    ftpCd(item_copy.text().toStdString().c_str()); //去到指定目录
}
/*本地双击文件,去到所点击的文件路径*/
void Widget::handleClientItemDoubleClicked(QListWidgetItem *item)
{
    QString path = ui->lineEdit_cli_path->text() + item->text() + '/';
    ui->listWidget_client->clear(); //清空前目录显示的文件
    ui->lineEdit_cli_path->setText(path); //更新显示的路径
    clientDirShow(path.toStdString().c_str()); //去到指定目录
}

/*进入指定服务器目录*/
void Widget::on_pushButton_entry_srv_clicked()
{
    ftpCd(ui->lineEdit_srv_path->text().toStdString().c_str()); //去到指定目录
}
/*进入指定本地目录*/
void Widget::on_pushButton_entry_cli_clicked()
{
    clientDirShow(ui->lineEdit_cli_path->text().toStdString().c_str());
}

/*显示当前目录中的所有文件及目录*/
void Widget::clientDirShow(const char *path)
{
    ui->listWidget_client->clear(); //先清空再显示
    // 获取目录下的文件列表
    QDir directory(path);
    QStringList files = directory.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);

    //图标
    QIcon _dir("dir.png");
    QIcon _file("file.png");
    // 将文件列表添加到 QListWidget 中
    for(const QString& file : files)
    {
        QFileInfo entryInfo(directory.absoluteFilePath(file)); //获取文件属性
        QListWidgetItem* item = new QListWidgetItem(entryInfo.isDir()?_dir:_file, file);
        ui->listWidget_client->addItem(item);
    }
}

/*本地文件/目录被选中时保存上传的文件名*/
void Widget::on_listWidget_client_itemClicked(QListWidgetItem *item)
{

    ui->pushButton_put->setEnabled(true); //激活上传控件
    //准备文件路径
    strcpy(client_file, item->text().toStdString().c_str()); //文件路径
    client_file[strlen(client_file)] = '\0';
}
/*从本地上传文件到服务器 使能上传标准位 发送相关命令*/
void Widget::on_pushButton_put_clicked()
{
    //发送上传指令并打开数据通道
    ftpPasvOpen();
    sprintf(buf, "STOR %s\n",client_file);
    put_flag = true;
    send_cmd();
}
/*实际上传内容拷贝函数*/
void Widget::putFile()
{
    QString real_file_path = ui->lineEdit_cli_path->text() + client_file; //本地绝对路径打开文件上传到服务器
    qDebug() << real_file_path;
    QFile file(real_file_path);
    // 读取文件内容
    QByteArray content;
    if(file.open(QFile::ReadOnly))
    {
        QString content = file.readAll();
        qDebug() << "put:\n" << content;
        tcpCmdSocket->write(content.toStdString().c_str());
        tcpCmdSocket->waitForBytesWritten(); //等待发送完成
        file.close();
        qDebug() << "文件上传成功";
        tcpCmdSocket->close(); //上传时服务器不通过数据通道发送内容给本地，而是本地发送上去，因此结束后需要手动关闭数据通道
    }
    else
    {
        qDebug() << "上传时无法打开文件 " << real_file_path;
    }
    put_flag = false;
}

/*服务器文件/目录被选中时保存下载的文件名*/
void Widget::on_listWidget_server_itemClicked(QListWidgetItem *item)
{
    ui->pushButton_get->setEnabled(true); //激活上传控件

    //准备文件路径
    strcpy(server_file, item->text().toStdString().c_str()); //文件路径
    server_file[strlen(server_file)] = '\0';
}


/*从服务器下载文件到本地 使能下载标准位 发送相关命令*/
void Widget::on_pushButton_get_clicked()
{
    //发送上传指令并打开数据通道
    ftpPasvOpen();
    sprintf(buf, "RETR %s\n",server_file);
    get_flag = true;
    send_cmd();
}
/*实际下载内容拷贝函数*/
void Widget::getFile(const char* buf)
{
    // 创建文件并写入二进制数据
    QString real_file_path = ui->lineEdit_cli_path->text() + server_file; //下载到本地在本地新建文件时需要绝对路径
    qDebug() << real_file_path;
    QFile file(real_file_path);
    if(file.open(QFile::WriteOnly))
    {
        file.write(buf);
        file.close();
        qDebug() << "文件下载成功";
    }
    else
    {
        qDebug() << "下载时无法打开文件 " << real_file_path;
    }
    get_flag = false;
}


/****************************************listWidget右键菜单设置**********************************************/

/*服务器右键菜单*/
void Widget::on_listWidget_server_customContextMenuRequested()
{
    // 创建菜单
    QMenu menu(ui->listWidget_server);
    QAction *deleteAction = menu.addAction("删除文件");
    QAction *mkdirAction = menu.addAction("新建目录");
    QAction *rmdirAction = menu.addAction("删除目录");

    // 处理菜单的点击事件
    QPoint posMenu(ui->listWidget_server->x()+ui->listWidget_server->width()/3, ui->listWidget_server->y()+ui->listWidget_server->height()/3);
    QPoint globalPos = mapToGlobal(posMenu);
    QAction *action = menu.exec(globalPos); //等待用户点击

    if(action == deleteAction) //删除文件
    {
        // 删除选定的项目
        ftpDel(ui->listWidget_server->currentItem()->text().toStdString().c_str());
    }
    else if(action == mkdirAction) //新建目录/文件
    {
        // 弹出输入框
        bool ok = false;
        QString text = QInputDialog::getText(nullptr, "新建", "文件名", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty()) {
            ftpMkdir(text.toStdString().c_str());
        }
    }
    else if(action == rmdirAction)//删除目录
    {
        ftpRmdir(ui->listWidget_server->currentItem()->text().toStdString().c_str());
    }
}

void Widget::on_listWidget_client_customContextMenuRequested()
{
    // 创建菜单
    QMenu menu(ui->listWidget_client);
    QAction *deleteAction = menu.addAction("删除文件");
    QAction *mkdirAction = menu.addAction("新建文件");

    // 处理菜单的点击事件
    QPoint posMenu(ui->listWidget_client->x()+ui->listWidget_client->width()/3, ui->listWidget_client->y()+ui->listWidget_client->height()/3);
    QPoint globalPos = mapToGlobal(posMenu);
    QAction *action = menu.exec(globalPos); //等待用户点击

    if(action == deleteAction) //删除文件
    {
        // 删除选定的项目
        QFile file(ui->lineEdit_cli_path->text()+ui->listWidget_client->currentItem()->text());
        if(file.exists())
        {
            if(file.remove())
            {
                qDebug() << ui->lineEdit_cli_path->text()+ui->listWidget_client->currentItem()->text() << " 文件删除成功";
            }
            else
            {
                qDebug() << ui->lineEdit_cli_path->text()+ui->listWidget_client->currentItem()->text() << " 权限不足";
            }
        }
        else
        {
            qDebug() << ui->lineEdit_cli_path->text()+ui->listWidget_client->currentItem()->text() << " 文件不存在";
        }

    }
    else if(action == mkdirAction) //新建目录/文件
    {
        // 弹出输入框
        bool ok = false;
        QString text = QInputDialog::getText(nullptr, "新建", "文件名", QLineEdit::Normal, "", &ok);
        if (ok && !text.isEmpty())
        {
            //新建文件
            QFile file(ui->lineEdit_cli_path->text()+text);
            if(file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                qDebug() << ui->lineEdit_cli_path->text()+text << " 新建成功！";
                file.close();
            } else
            {
                qDebug() << "权限不足.";
            }
        }
    }
    clientDirShow(ui->lineEdit_cli_path->text().toStdString().c_str());
}
