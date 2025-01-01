#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidget>
#include <QTableWidget>
#include <QSqlDatabase>
#include <QSqlQuery>

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

private slots:
    void on_comboBox_activated(int index);
    //void onListWidgetItemClicked(QListWidgetItem *item); // 点击ListWidget项时触发
    void onOptionClicked(QListWidgetItem *item);


private:
    Ui::MainWindow *ui;
    void updateListWidget(int index);
    QSqlDatabase db; // 数据库连接
    void setupDatabase(); // 初始化数据库
    void executeProcedure(); // 执行存储过程
    void displayResults(QSqlQuery &query); // 显示查询结果到TableWidget
    QString selectedOption;
};
#endif // MAINWINDOW_H
