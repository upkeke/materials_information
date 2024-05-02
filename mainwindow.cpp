#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include<QFileDialog>
#include<QMessageBox>
#include<QDateTime>
#include<QUrl>

static QStringList SGS_LS{"Pb", "Cd", "Hg", "Cr6+", "PBB", "PBDE", "DEHP", "BBP", "DBP", "DIBP", "Br", "Cl", "PVC", "SCCP", "HBCDD", "Sb", "Be", "PAHs", "PFOS", "PFOA", "Red Phosphorus"};
static QStringList SGS_Class{"Rohs","hf","PVC","Sb_Be","PAHs","Red_Phosphorus"};
static std::vector<int> SGS_Class_Count{10,2,3,2,3,1};

void test1();
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow),log(MyLog::instance())
{
    ui->setupUi(this);
    initData();
    initBtn();
    initTreeViewSGS();
    log.setCMD(ui->textBrowser);

    QStringList horizontalHeader;
    horizontalHeader << "物质名称" << "CAS No" << "均质材料中的含量(%)";
    ui->tableWidget_substance ->setColumnCount(horizontalHeader.size()); // 设置列数
    ui->tableWidget_substance->setHorizontalHeaderLabels(horizontalHeader); // 设置水平表头标签

    //connect(ui->cbx_class,&QComboBox::activated,this,&MainWindow::cbx_display);


}

MainWindow::~MainWindow()
{
    delete data;
    delete ui;
}

void MainWindow::initData()
{
    data =  new datamanager(this);
    QStringList _class = data->getAllClass();
    ui->cbx_class->addItems(_class);
    connect(ui->cbx_class,&QComboBox::activated,this,&MainWindow::cbx_display);
    ui->cbx_mtname->setEnabled(false);
    ui->cbx_suppelier->setEnabled(false);
}

void MainWindow::initBtn()
{
    ui->btn_SGS->setEnabled(false);
    ui->btn_Msds->setEnabled(false);
    ui->btn_UL->setEnabled(false);
    ui->btn_confide->setEnabled(false);
}

void MainWindow::initTreeViewSGS()
{
    //1，构造Model，这里示例具有3层关系的model构造过程
    QStandardItemModel* model = new QStandardItemModel(ui->treeView_SGS);
    model->setHorizontalHeaderLabels(QStringList()<<QStringLiteral("项目")<<QStringLiteral("含量"));
    int i=0;
    /*
    QStandardItem* Rohs = new QStandardItem(QStringLiteral("Rohs"));
    //QStandardItem* item2 = new QStandardItem(QStringLiteral("一级节点"));
    model->appendRow(Rohs);

    for(;i<10;++i)
    {
        QList<QStandardItem*> sgs_rohs;
        QStandardItem* x1 = new QStandardItem(SGS_LS[i]);
        QStandardItem* x2 = new QStandardItem(QString::number(i));
        sgs_rohs.append(x1);
        sgs_rohs.append(x2);
        Rohs->appendRow(sgs_rohs);

    }

    QStandardItem* HF = new QStandardItem(QStringLiteral("HF"));
    model->appendRow(HF);
    for(;i<12;++i)
    {
        QList<QStandardItem*> itme2_;
        QStandardItem* x1 = new QStandardItem(SGS_LS[i]);
        QStandardItem* x2 = new QStandardItem("");
        itme2_.append(x1);
        itme2_.append(x2);
        HF->appendRow(itme2_);
    }

    QStandardItem* PVC = new QStandardItem(QStringLiteral("PVC"));
    model->appendRow(PVC);
    for(;i<15;++i)
    {
        QList<QStandardItem*> itme2_;
        QStandardItem* x1 = new QStandardItem(SGS_LS[i]);
        QStandardItem* x2 = new QStandardItem("");
        itme2_.append(x1);
        itme2_.append(x2);
        PVC->appendRow(itme2_);
    }


    QStandardItem* Sb_Be = new QStandardItem(QStringLiteral("Sb_Be"));
    model->appendRow(Sb_Be);
    for(;i<17;++i)
    {
        QList<QStandardItem*> itme2_;
        QStandardItem* x1 = new QStandardItem(SGS_LS[i]);
        QStandardItem* x2 = new QStandardItem("");
        itme2_.append(x1);
        itme2_.append(x2);
        Sb_Be->appendRow(itme2_);
    }

    QList<QStandardItem*> Red_Phosphorus;
    QStandardItem* x1 = new QStandardItem(SGS_LS[i]);
    QStandardItem* x2 = new QStandardItem("xx");
    Red_Phosphorus.append(x1);
    Red_Phosphorus.append(x2);
    //Red_Phosphorus->appendRow(itme2_);
    model->appendRow(Red_Phosphorus);
    */
    std::function<void(QString,int)> func = [&](QString _class,int count){
        QStandardItem* item = new QStandardItem(_class);
        model->appendRow(item);
        for(int j=0;j<count;++j)
        {
            QList<QStandardItem*> itme2_;
            QStandardItem* x1 = new QStandardItem(SGS_LS[i++]);
            QStandardItem* x2 = new QStandardItem("");
            itme2_.append(x1);
            itme2_.append(x2);
            item->appendRow(itme2_);
        }
    };
    for(int k=0;k<SGS_Class.size();++k)
    {
        func(SGS_Class[k],SGS_Class_Count[k]);
    }
    ui->treeView_SGS->setModel(model);
    ui->treeView_SGS->show();
    //ui->treeView->setModel(model)
}

void MainWindow::dataIntoTreeViewSGS()
{
    QStringList sgsdata = allData.sgsData;
    int i =0; //指针指sgsdata的位置
    int row = 0;
    auto model = ui->treeView_SGS->model();
    /*
    // QModelIndex rohs =model->index(row++,0,ui->treeView_SGS->rootIndex());
    // for(int j=0;j<10;++j)
    // {
    //     QModelIndex index1_=model->index(j,1,rohs);
    //     model->setData(index1_,sgsdata[i++],Qt::DisplayRole);
    // }

    // QModelIndex hf =model->index(row++,0,ui->treeView_SGS->rootIndex());
    // for(int j=0;j<2;++j)
    // {
    //     QModelIndex index1_=model->index(j,1,rohs);
    //     model->setData(index1_,sgsdata[i++],Qt::DisplayRole);
    // }
    */
    std::function<void(int)> func = [&](int weici){
        QModelIndex index =model->index(row++,0,ui->treeView_SGS->rootIndex()); //树的第一层第几个
        for(int j=0;j<weici;++j)
        {
            QModelIndex index1_=model->index(j,1,index); //树的第二层的项目，修改第二列
            model->setData(index1_,sgsdata[i++],Qt::DisplayRole);
        }
    };
    vector<int> lc{10,2,3,2,3,1}; //rohs 10项，hf 2项， pvc 3项，sb-be 2项 ...
    for(auto& x:lc)
    {
        func(x);
    }


}

void MainWindow::dataIntoTableWidget_substance()
{
    std::vector<QStringList> xx = data->getSubstance(allData.id());

    int row = ui->tableWidget_substance->rowCount(); // 获取当前行数
    for (int i = 0; i < xx[0].size(); ++i) {
        qDebug() << xx[0].at(i);
    }
    ui->tableWidget_substance->setRowCount(row +xx.size()); // 增加一行
    for(int i=0;i<xx.size();++i)
    {
        for (int col = 0; col < xx[i].size(); ++col) {
            QTableWidgetItem* item = new QTableWidgetItem(xx[i].at(col)); // 创建一个新的 QTableWidgetItem
            ui->tableWidget_substance->setItem(row, col, item); // 将 QTableWidgetItem 添加到表格中的指定单元格
        }
        row++;
    }
}


void MainWindow::on_btn_test_clicked()
{

    auto model = ui->treeView_SGS->model();
    QModelIndex index =model->index(0,0,ui->treeView_SGS->rootIndex()); //第一层的第一个节点
    QModelIndex index1_2=model->index(2,1,index); //第一层的第一个节点的第三个节点的第二列
    model->setData(index1_2,"999",Qt::DisplayRole);


}


void MainWindow::on_btn_search_clicked()
{
    initBtn();
    QString mt_name = ui->cbx_mtname->currentText();
    QString sp_name = ui->cbx_suppelier->currentText();
    int mt_id = data->hasMt(mt_name);
    int sp_id = data->hasSp(sp_name);
    allData = data->hasMtSp(mt_id,sp_id);
    if(mt_id==-1||sp_id==-1||!allData.has)
    {
        log.error("没有对应材料+供应商");
        return;
    }
    log.normal("找到对应材料+供应商");
    dataIntoTreeViewSGS();

    //填写SGS 编号
    ui->le_SGS_No->setText(allData.SGS_NO());
    //填写SGS 日期
    QString sgs_date = allData.SGS_Date();

    ui->dateEdit_SGS->setDate(QDate::fromString(sgs_date, "yyyy/M/d"));
    int dis = distance_date(sgs_date);
    if(dis/365>=1)
        log.error("SGS报告已过期,当前最新版本日期为："+allData.SGS_Date());
    else
    {
        ui->btn_SGS->setEnabled(true);
        if(dis<30)
        {
            log.warn(QString("还有%1天，SGS过期").arg(dis));
        }
        else
        {
            log.normal("SGS报告正常使用");
        }
    }

    // 填写MSDS日期
    QString msds_date = allData.MSDS_Date();
    //ui->le_MsdsDate ->setText(msds_date);
    ui->dateEdit_MSDS->setDate(QDate::fromString(msds_date.trimmed(), "yyyy/M/d"));
    QString xx1 ="2024/4/30";
    QDate d = QDate::fromString(xx1, "yyyy/MM/dd");
    dis = distance_date(msds_date);
    if(dis/365>=5)
        log.error("MSDS报告已过期,当前最新版本日期为："+allData.MSDS_Date());
    else
    {
        ui->btn_Msds->setEnabled(true);
        if(dis/365==4 && dis%365<30)
        {
            log.warn(QString("还有%1天，MSDS过期").arg(dis));
        }
        else
        {
            log.normal("MSDS报告正常使用");
        }
    }
    //有无UL
    if(allData.UL().isEmpty())
        log.normal("无UL");
    else
    {
        log.normal("有UL");
        ui->btn_UL->setEnabled(true);
    }

    //填写材质证明
    ui->btn_confide->setEnabled(true);
    // 填写物质含量比例
    dataIntoTableWidget_substance();

}


void MainWindow::on_btn_SGS_clicked()
{
    QUrl fl = QUrl::fromLocalFile(allData.SGS_Path());
    // 打开文件夹
    if (!QDesktopServices::openUrl(fl)) {
        qDebug() << "Failed to open folder";
    }
}


void MainWindow::on_btn_UL_clicked()
{
    QUrl fl = QUrl::fromLocalFile(allData.UL());
    // 打开文件夹
    if (!QDesktopServices::openUrl(fl)) {
        qDebug() << "Failed to open folder";
    }
}


void MainWindow::on_btn_Msds_clicked()
{
    QUrl fl = QUrl::fromLocalFile(allData.MSDS_Path());
    // 打开文件夹
    if (!QDesktopServices::openUrl(fl)) {
        qDebug() << "Failed to open folder";
    }
}


void MainWindow::on_btn_confide_clicked()
{
    QUrl fl = QUrl::fromLocalFile(allData.confide());
    // 打开文件夹
    if (!QDesktopServices::openUrl(fl)) {
        qDebug() << "Failed to open folder";
    }
}

void MainWindow::cbx_display(int index)
{
    QString _class(ui->cbx_class->itemText(index));
    ui->cbx_class->setCurrentIndex(index);
    disconnect(ui->cbx_class,&QComboBox::activated,this,&MainWindow::cbx_display);
    //ui->cbx_class->clear();
    ui->cbx_mtname->clear();
    ui->cbx_suppelier->clear();
    ui->cbx_mtname->setEnabled(true);
    ui->cbx_suppelier->setEnabled(true);
    QStringList mtname = data->getSomeMt(_class);
    QStringList spname = data->getSomeSp(_class);
    ui->cbx_mtname->addItems(mtname);
    ui->cbx_suppelier->addItems(spname);
    connect(ui->cbx_class,&QComboBox::activated,this,&MainWindow::cbx_display);
}


void MainWindow::on_btn_insert_clicked()
{
    // QString _class = ui->cbx_class->currentText();
    // QString mt_name = ui->cbx_mtname->currentText();
    // QString sp_name = ui->cbx_suppelier->currentText();
    QString _class = ui->le_class->text();
    QString mt_name = ui->le_mt_name->text();
    QString sp_name = ui->le_suppelier->text();
    //添加到数据库中
    data->addMtItem(mt_name,_class);
    data->addSpItem(sp_name,_class);

    //添加到combobox中
    ui->cbx_class->addItem(_class);
    //ui->cbx_mtname->addItem(mt_name);
    //ui->cbx_suppelier->addItem(sp_name);

}

