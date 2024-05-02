#include "datamanager.h"
#include<QSqlRecord>
#include<QSqlError>
#include<QtSql>
#include<QSqlDatabase>
#include<QSqlQuery>
#include<QSqlError>
#include<QMessageBox>


using std::shared_ptr;
datamanagerTS::datamanagerTS(QObject *parent)
    : QSqlTableModel{parent}
{}



void test1()
{

    QStringList fields = {
        "Pb TEXT",
        "Cd TEXT",
        "Hg TEXT",
        "Cr6 TEXT",
        "PBB TEXT",
        "PBDE TEXT",
        "DEHP TEXT",
        "BBP TEXT",
        "DBP TEXT",
        "DIBP TEXT",
        "Br TEXT",
        "Cl TEXT",
        "PVC TEXT",
        "SCCP TEXT",
        "HBCDD TEXT",
        "Sb TEXT",
        "Be TEXT",
        "PAHs TEXT",
        "PFOS TEXT",
        "PFOA TEXT",
        "\"Red Phosphorus\" TEXT"
    };
    // 执行每个字段的添加操作
    QSqlQuery query;
    foreach (const QString& field, fields) {
        QString sql = "ALTER TABLE main ADD COLUMN " + field;
        bool success = query.exec(sql);

        if (!success) {
            qDebug() << "Error: Failed to add column" << field << ":" << query.lastError().text();
        }
    }

}



shared_ptr<sqliteOrder> sqliteOrder::single_sql = nullptr;

void sqliteOrder::setDb(QSqlDatabase db)
{
    this->db = db;
}

int sqliteOrder::getMaxId(QString zhiduan,QString table)
{
    QString order = QString("SELECT MAX(%1) FROM %2").arg(zhiduan).arg(table);
    QSqlQuery query(order);
    int maxId;
    if (query.exec() && query.next()) {
        maxId = query.value(0).toInt();
        qDebug() << "Max ID:" << maxId;
    } else {
        qDebug() << "Failed to query max ID:" << query.lastError().text();
        maxId = -1;
    }
    return maxId;
}

bool sqliteOrder::hasName(const QString &zhiduan, const QString &text, const QString &table)
{

    // // 执行SQL查询以检查是否存在符合条件的数据
    // QString queryString = "SELECT COUNT(*) FROM your_table_name WHERE name = :name";
    // QSqlQuery query;
    // query.prepare(queryString);
    // query.bindValue(":name", "kk");
    QString queryString = QString("SELECT COUNT(*) FROM %1 WHERE %2 = '%3'").arg(table).arg(zhiduan).arg(text);
    QSqlQuery query(queryString);
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        if (count > 0) {
            return true;
            qDebug() << "Data with name=kk exists in the table.";
        } else {
            qDebug() << "Data with name=kk does not exist in the table.";
        }
    } else {
        qDebug() << "Failed to query data:" << query.lastError().text();
    }
    return false;
}

shared_ptr<sqliteOrder> sqliteOrder::instance_db()
{
    if(single_sql == nullptr)
        single_sql = shared_ptr<sqliteOrder>(new sqliteOrder());
    return single_sql;
}

sqliteOrder::~sqliteOrder()
{

}


sqliteOrder::sqliteOrder()
{
}



datamanager::datamanager(QWidget *parent):parent(parent)
{
    DB = QSqlDatabase::addDatabase("QSQLITE");
    //DB.setDatabaseName("123.db3");
    DB.setDatabaseName("material.db");
    if(DB.open())
    {
        qDebug()<<"yes";
    }
    else
    {
        QMessageBox::warning(parent,"error","打开数据库失败");
    }
    so =  sqliteOrder::instance_db();
    so->setDb(DB);


    ma = new QSqlRelationalTableModel(parent,DB);
    ma->setTable("main");
    ma->setEditStrategy(QSqlTableModel::OnManualSubmit);

    mt = new QSqlTableModel (parent,DB);
    mt->setTable("materials");
    mt->setEditStrategy(QSqlTableModel ::OnManualSubmit);
    mt->select();

    sp = new QSqlTableModel (parent,DB);
    sp->setTable("suppliers");
    sp->setEditStrategy(QSqlTableModel ::OnManualSubmit);
    sp->select();


    sb = new QSqlTableModel (parent,DB);
    sb->setTable("substance");
    sb->setEditStrategy(QSqlTableModel ::OnManualSubmit);
    sb->select();
}


int datamanager::addMtItem(const QString&  name,const QString&  _class)
{


    if(so->hasName("mt_name",name,"materials"))
        return -1;
    else
    {
        mt->select();
        QSqlRecord record = mt->record();
        int rowNum = so->getMaxId("mt_id","materials")+1;//获得最达mt_id+1
        record.setValue("mt_id",rowNum);
        record.setValue("mt_name",name);
        record.setValue("class",_class);
        //先判断有没有这个元素

        mt->insertRecord(-1,record);
        bool x=  mt->submitAll();
        if(x)
        {
            qDebug()<<"success";
        }
        else {
            qDebug() << "数据插入失败: " << mt->lastError().text();
        }
        return rowNum;
    }



}
int datamanager::addSpItem(const QString&  name,const QString&  _class)
{
    if(so->hasName("sp_name",name,"suppliers"))
        return -1;
    sp->select();
    QSqlRecord record = sp->record();
    int rowNum = so->getMaxId("sp_id","suppliers")+1;//获得最达mt_id+1
    record.setValue("sp_id",rowNum);
    record.setValue("sp_name",name);
    record.setValue("class",_class);
    sp->insertRecord(-1,record);
    bool x=  sp->submitAll();
    if(x)
    {
        qDebug()<<"success";
    }
    else {
        qDebug() << "数据插入失败: " << sp->lastError().text();
    }
    return rowNum;
}

int datamanager::hasMt(const QString &name)
{
    mt->setFilter(QString("mt_name = '%1'").arg(name));
    mt->select();
    if(mt->rowCount()>0)
    {
        return  mt->record(0).value("mt_id").value<int>();
    }
    return -1;
}

int datamanager::hasSp(const QString &name)
{
    sp->setFilter(QString("sp_name = '%1'").arg(name));
    sp->select();
    if(sp->rowCount()>0)
    {
        return sp->record(0).value("sp_id").value<int>();
    }

    return -1;
}

MaterialData datamanager::hasMtSp(int mt_id, int sp_id)
{
    ma->setFilter(QString("mt_id = %1 AND sp_id = %2").arg(mt_id).arg(sp_id));
    ma->select();

    if(ma->rowCount()>0)
    {
        QSqlRecord rd = ma->record(0);
        std::vector<int> r1;
        QStringList r2;
        for(int i = 0;i<ma->columnCount();++i)
        {
            if(i<3)
            {
                r1.push_back(rd.value(i).value<int>());
            }
            else
                r2<<rd.value(i).value<QString>();
        }
        return MaterialData(r1,r2);
    }

    return MaterialData();

}

QStringList datamanager::getMainData(int mt_id, int sp_id)
{
    ma->setFilter (QString("mt_id = %1 AND sp_id = %2").arg(mt_id).arg(sp_id));
    // 执行查询
    ma->select();
    QStringList sl;
    QSqlRecord ro = ma->record(0);
    for(int j=0;j<ma->columnCount();++j)
    {
        sl<<ro.value(j).value<QString>();
    }
    return sl;
}

QStringList datamanager::getAllClass()
{
    // 选择数据
    mt->select();
    // 创建 QStringList 以存储数据
    QStringList dataList;
    // 遍历每一行，将特定列的值添加到 QStringList 中
    for (int row = 0; row < mt->rowCount(); ++row) {
        // 获取特定列的值，这里假设您想获取第一列的数据
        QVariant value = mt->record(row).value(2);

        // 将 QVariant 转换为 QString 并添加到 QStringList 中
        dataList << value.toString();

    }
    dataList.removeDuplicates();
    return dataList;
}

QStringList datamanager::getAllMt()
{
    // 选择数据
    mt->select();
    QStringList dataList;
    for (int row = 0; row < mt->rowCount(); ++row) {
        QVariant value = mt->record(row).value(1);
        dataList << value.toString();
    }
    return dataList;
}

QStringList datamanager::getSomeMt(const QString &_class)
{
    mt->setFilter (QString("class = '%1'").arg(_class));
    mt->select();
    QStringList dataList;
    for (int row = 0; row < mt->rowCount(); ++row) {
        QVariant value = mt->record(row).value(1);
        dataList << value.toString();
    }
    return dataList;
}

QStringList datamanager::getAllSp()
{
    // 选择数据
    sp->select();
    // 创建 QStringList 以存储数据
    QStringList dataList;
    // 遍历每一行，将特定列的值添加到 QStringList 中
    for (int row = 0; row < sp->rowCount(); ++row) {
        // 获取特定列的值，这里假设您想获取第一列的数据
        QVariant value = sp->record(row).value(1);

        // 将 QVariant 转换为 QString 并添加到 QStringList 中
        dataList << value.toString();
    }
    return dataList;
}

QStringList datamanager::getSomeSp(const QString &_class)
{
    sp->setFilter (QString("class = '%1'").arg(_class));
    sp->select();
    QStringList dataList;
    for (int row = 0; row < sp->rowCount(); ++row) {
        QVariant value = sp->record(row).value(1);
        dataList << value.toString();
    }
    return dataList;
}

// 查询
std::vector<QStringList> datamanager::getSubstance(int id)
{
    std::vector<QStringList> res;
    sb->setFilter(QString("main_id = %1").arg(id));
    sb->select();

    for(int i=0;i<sb->rowCount();++i)
    {
        QStringList sl;
        QSqlRecord ro = sb->record(i);
        for(int j=1;j<ro.count();++j)
        {
            sl<<ro.value(j).value<QString>();
        }
        res.push_back(sl);
    }
    return res;
}

datamanager::~datamanager()
{
    DB.close();

    //delete sp;
    //delete mt;
}
