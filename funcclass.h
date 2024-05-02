#ifndef FUNCCLASS_H
#define FUNCCLASS_H
#include<QString>
#include<QDateTime>
#include<vector>
#include<QStringList>
#include <iostream>
#include <fstream>
#include <memory>
#include <mutex>
#include<QTextBrowser>
using std::vector;
class FuncClass
{
public:
    FuncClass();
};

int distance_date(const QString& last);



class MyLog {
private:
    enum class leve{
        normal,
        warning,
        error
    };
public:
    // 获取单例实例的方法
    static MyLog& instance() {
        static MyLog instance; // 线程安全的单例实现
        //instance.bw = nullptr;
        return instance;
    }
    void setCMD(QTextBrowser* tb)
    {
        this->bw = tb;
    }
    // 记录日志的方法
    void preprocessing(leve lv) {
        //std::lock_guard<std::mutex> lock(mutex_); // 确保线程安全
        // 在文本末尾追加文本而不换行
        //bw.moveCursor(QTextCursor::End);
        switch (lv) {
        case leve::normal:
            bw->append("<font color='black'>正常：</font>");
            break;
        case leve::warning:
            bw->append("<font color='yellow'>警告：</font>");
            break;
        case leve::error:
            bw->append("<font color='red'>错误：</font>");
            break;
        default:
            break;
        }
        bw->moveCursor(QTextCursor::End);
        //bw->append(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss")+": ");
        //bw->append(message);
    }
    void normal(const QString& message)
    {
        //std::lock_guard<std::mutex> lock(mutex_); // 确保线程
        preprocessing(leve::normal);
        bw->insertPlainText(message);
    }
    void warn(const QString& message){
        //std::lock_guard<std::mutex> lock(mutex_); // 确保线程安全
        preprocessing(leve::warning);
        bw->insertPlainText(message);
        //bw->append (QString("" message));
    }
    void error(const QString& message)
    {
        // 使用 HTML 标记添加带颜色的文字
        //textBrowser.append("<font color='red'>This is red text.</font>");
        //std::lock_guard<std::mutex> lock(mutex_); // 确保线程
        preprocessing(leve::error);
        bw->insertPlainText(message);
    }

    // // 设置日志文件
    // void setLogFile(const std::string& filePath) {
    //     std::lock_guard<std::mutex> lock(mutex_);
    //     logStream_.open(filePath, std::ios::app); // 打开日志文件用于追加
    // }

private:

    // 私有构造函数，确保不能在类外创建实例
    MyLog() {}

    // 禁止拷贝构造函数和赋值运算符
    MyLog(const MyLog&) = delete;
    MyLog& operator=(const MyLog&) = delete;

    //std::ofstream logStream_; // 用于记录日志的流
    std::mutex mutex_;        // 确保线程安全的互斥锁
    QTextBrowser * bw;
};
class MaterialData
{
public:
    MaterialData();
    MaterialData(const std::vector<int> &idList,
                 const QStringList& other);
    bool has;

    int id();
    int mt_id();
    int sp_id();
    QString Pb();
    QString Cd();
    QString Hg();
    QString Cr6Plus();
    QString PBB();
    QString PBDE();
    QString DEHP();
    QString BBP();
    QString DBP();
    QString DIBP();
    QString Br();
    QString Cl();
    QString PVC();
    QString SCCP();
    QString HBCDD();
    QString Sb();
    QString Be();
    QString PAHs();
    QString PFOS();
    QString PFOA();
    QString Red_Phosphorus();
    QString SGS_NO();
    QString SGS_Date();
    QString SGS_Path();
    QString MSDS_Path();
    QString MSDS_Date();
    QString UL();
    QString confide();

    std::vector<int> idList;
    QStringList sgsData;

};

#endif // FUNCCLASS_H
