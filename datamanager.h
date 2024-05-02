#ifndef DATAMANAGER_H
#define DATAMANAGER_H

#include <QObject>
#include <QSqlRelationalTableModel>
#include<QString>
#include<QStringList>
#include<QSqlRecord>
#include<vector>
#include<memory>
#include"funcclass.h"
class datamanagerTS : public QSqlTableModel
{
public:
    explicit datamanagerTS(QObject *parent = nullptr);
};

//QSqlTableModel不能适用的场景 直接执行sqlite语句
class sqliteOrder
{
public:
    QSqlDatabase db; //
   // QSqlQuery query; //执行sql指令
    void setDb(QSqlDatabase db);
    int getMaxId( QString zhiduan,QString table);
    bool hasName(const QString& zhiduan,const QString& text,const QString&  table);
    std::pair<int,int> getAllId(QString mt_name,QString sp_name);
    static std::shared_ptr<sqliteOrder> instance_db();

    ~sqliteOrder();
private:
    static std::shared_ptr<sqliteOrder> single_sql;
    sqliteOrder();
};

class datamanager{
public:
    QSqlDatabase DB;
    std::shared_ptr<sqliteOrder> so;
    QSqlRelationalTableModel *ma;
    QSqlTableModel* mt;
    QSqlTableModel* sp;
    QSqlTableModel* sb;
    QSqlRecord record;
    QObject *parent ;
    datamanager(QWidget *parent);
    //int addClass(const QString&  _class);
    int addMtItem(const QString&  mt_name,const QString&  _class);
    int addSpItem(const QString&  sp_name,const QString&  _class);
    int hasMt(const QString&  name);
    int hasSp(const QString&  name);
    MaterialData hasMtSp(int mt_id,int sp_id);
    QStringList getMainData(int mt_id,int sp_id);
    //得到材料的所有类别     电镀 塑胶 铜
    QStringList getAllClass();
    //得到材料的名称  镀镍 LCP-黑色  C5191 弃用
    QStringList getAllMt();
    //得到指定材料的名称
    QStringList getSomeMt(const QString&  _class);

    //得到材料的所有供应商 东旭	中连	中金--Y型端子	兴宇	兴泰	品瑞和全度	常润	新机
    QStringList getAllSp();
    //得到指定材料的供应商
    QStringList getSomeSp(const QString&  _class);


    std::vector<QStringList> getSubstance(int id);
    ~datamanager();


};


#endif // DATAMANAGER_H
