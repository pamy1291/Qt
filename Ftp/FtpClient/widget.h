#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QHostAddress>
#include <QTcpSocket>
#include <QListWidgetItem>
#include <QDir>
#include <QFile>
#include <QAction>
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

#define MAX_SIZE 4096 //存储最大信息字节数

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    void send_cmd();
    void ftpPasvOpen();
    void ftpPasvLink(quint16 port);
    void ftpPasvClose();
    void ftpPwd();
    void ftpLs();
    void ftpDel(const char* filename);
    void ftpMkdir(const char* dir);
    void ftpRmdir(const char* dir);
    void ftpCd(const char* path);

    void fileExplain(const char* buf);


private slots:
    /*服务端内容处理*/
    void on_pushButton_link_clicked();

    void on_connected();
    void on_connected_cmd();
    void on_readyRead();
    void on_readyRead_cmd();

    void handleServerItemDoubleClicked(QListWidgetItem *item);
    void on_pushButton_entry_srv_clicked();

    /*本地客户端文件内容处理*/
    void clientDirShow(const char* path);
    void on_pushButton_entry_cli_clicked();
    void handleClientItemDoubleClicked(QListWidgetItem *item);

    void on_pushButton_put_clicked();

    void on_listWidget_client_itemClicked(QListWidgetItem *item);

    /*上传与下载实际内容复制处理*/
    void putFile();
    void getFile(const char* buf);

    void on_pushButton_get_clicked();

    void on_listWidget_server_itemClicked(QListWidgetItem *item);

    /*服务端菜单*/
    //void contextMenuEvent(QContextMenuEvent *event) override;
    void on_listWidget_server_customContextMenuRequested();

    void on_listWidget_client_customContextMenuRequested();

private:
    Ui::Widget *ui;
    QTcpSocket* tcpSocket; //命令通道
    QTcpSocket* tcpCmdSocket; //命令通道
    bool user_flag;
    bool pass_flag;
    int status; //信息标志位
    bool put_flag; //上传标准位，代表发生的是上传操作
    bool get_flag; //下载标志位
    char* buf; //存储命令及提示信息
    char server_path[MAX_SIZE]; //显示的服务器路径
    char client_path[MAX_SIZE]; //显示的本地路径
    char client_file[MAX_SIZE]; //上传的文件名
    char server_file[MAX_SIZE]; //下载的文件名

};
#endif // WIDGET_H
