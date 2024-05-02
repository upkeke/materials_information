#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDesktopServices>
#include<QDate>
#include"datamanager.h"
#include<QStandardItemModel>
#include<functional>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private:
    void initData();
    void initBtn();
    void initTreeViewSGS();
    void dataIntoTreeViewSGS();

    void dataIntoTableWidget_substance();

private slots:

    void on_btn_test_clicked();

    void on_btn_search_clicked();

    void on_btn_SGS_clicked();

    void on_btn_UL_clicked();

    void on_btn_Msds_clicked();

    void on_btn_confide_clicked();

    void cbx_display(int);

    void on_btn_insert_clicked();

private:
    Ui::MainWindow *ui;

    datamanager *data;
    MaterialData allData;
    MyLog & log;

};
#endif // MAINWINDOW_H
