#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSqlError>
#include <QDebug>
#include <QSqlRecord>
#include <QMap>

QMap<QString, QString> optionQuestions = {
    {"问题 1-1", "查询在指定时间段指定区域的社会消费品零售总额。（输入地区、时间）"},
    {"问题 1-2-1", "查询指定时间段社会消费品零售总额最高和最低区域。分析近三年最高区域和最低区域是否有变化。（输入时间）"},
    {"问题 1-2-2", "查询指定时间段社会消费品零售总额最高和最低区域。分析近三年最高区域和最低区域是否有变化。"},
    {"问题 1-3", "分析在近三年(2020~2022)中，哪些区域的社会消费品零售总额在2020年同比下降，2021年同比增加，2022年同比下降。计算这些省份在所有省份和直辖市的占比。"},
    {"问题 1-4", "分析省会城市的消费对其所在省份的贡献，即查询在指定时间段省会城市社会消费品零售总额在其所在的省份的占比。（输入时间）"},
    {"问题 1-5", "分析比较指定时间段东部、西部、中部、东北部地区的社会消费品零售总额，并进行排序。（输入时间）"},

    //居民收入
    {"问题 2-1", "查询在分析在指定时间段指定区域居民收入环比增长率。（输入地区、时间）"},
    {"问题 2-2", "查询在指定时间段区域居民收入最高和最低区域，以及它们与全国平均值的差异。（输入时间）"},
    {"问题 2-3", "分析在指定时间段某个区域居民的各项收入。（输入地区、时间）"},
    {"问题 2-4", "分析比较指定区域各项收入占比的变化。（输入地区、时间）"},
    {"问题 2-5", "查询分析在指定时间段区域居民收入最高的3个区域和最低的3个区域的以及它们的人口特征，包括人口数量、人口密度，等等。人口密度用人口数量/区域面积得到，可不用另行采集存人口密度数据。（输入起始时间）"},
    {"问题 2-6-1", "如果将居民收入划分为高、中、低三个等级，给每个区域收入赋予等级，将结果存储在数据库中，并且统计在指定时间段各种等级的区域数量。查询哪些区域居民收入一直位于高等级，哪些一直位于低等级。注意不同时间段划分高中低等级的标准不同。（输入时间）"},
    {"问题 2-6-2", "如果将居民收入划分为高、中、低三个等级，给每个区域收入赋予等级，将结果存储在数据库中，并且统计在指定时间段各种等级的区域数量。查询哪些区域居民收入一直位于高等级，哪些一直位于低等级。注意不同时间段划分高中低等级的标准不同。（输入时间）"},
    {"问题 2-7", "比较东部、西部、中部、东北部地区的居民收入的环比增长率。（输入时间）"},
    {"问题 2-8-1", "分析比较在指定时间段指定区域的居民收入与社会消费品零售额的变化趋势，在最近三年，哪些区域的居民收入与社会消费品零售额的变化趋势相同？（输入地区、时间）"},
    {"问题 2-8-2", "分析比较在指定时间段指定区域的居民收入与社会消费品零售额的变化趋势，在最近三年，哪些区域的居民收入与社会消费品零售额的变化趋势相同？"},

    {"问题 3-1", "查询在指定时间段指定区域的GDP。（输入地区、时间）"},
    {"问题 3-2", "查询指定时间段GDP总额最高和最低区域。（输入时间）"},
    {"问题 3-3", "分析比较指定时间段东部、西部、中部、东北部地区的GDP，并进行排序。（输入时间）"},
};


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("1");
    ui->comboBox->addItem("2");
    ui->comboBox->addItem("3");
    ui->listWidget->addItem("问题 1-1");
    ui->listWidget->addItem("问题 1-2-1");
    ui->listWidget->addItem("问题 1-2-2");
    ui->listWidget->addItem("问题 1-3");
    ui->listWidget->addItem("问题 1-4");
    ui->listWidget->addItem("问题 1-5");
    // 连接ListWidget的点击信号到槽函数
    //connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onListWidgetItemClicked);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &MainWindow::onOptionClicked);
    connect(ui->queryButton, &QPushButton::clicked, this, &MainWindow::executeProcedure);
    // 初始化数据库连接
    setupDatabase();

    ui->provinceComboBox->addItem("地区");
    ui->provinceComboBox->addItem("北京");
    ui->provinceComboBox->addItem("天津");
    ui->provinceComboBox->addItem("河北");
    ui->provinceComboBox->addItem("山西");
    ui->provinceComboBox->addItem("内蒙古");
    ui->provinceComboBox->addItem("辽宁");
    ui->provinceComboBox->addItem("吉林");
    ui->provinceComboBox->addItem("黑龙江");
    ui->provinceComboBox->addItem("上海");
    ui->provinceComboBox->addItem("江苏");
    ui->provinceComboBox->addItem("浙江");
    ui->provinceComboBox->addItem("安徽");
    ui->provinceComboBox->addItem("福建");
    ui->provinceComboBox->addItem("江西");
    ui->provinceComboBox->addItem("山东");
    ui->provinceComboBox->addItem("河南");
    ui->provinceComboBox->addItem("湖北");
    ui->provinceComboBox->addItem("湖南");
    ui->provinceComboBox->addItem("广东");
    ui->provinceComboBox->addItem("广西");
    ui->provinceComboBox->addItem("海南");
    ui->provinceComboBox->addItem("重庆");
    ui->provinceComboBox->addItem("四川");
    ui->provinceComboBox->addItem("贵州");
    ui->provinceComboBox->addItem("云南");
    ui->provinceComboBox->addItem("西藏");
    ui->provinceComboBox->addItem("陕西");
    ui->provinceComboBox->addItem("甘肃");
    ui->provinceComboBox->addItem("青海");
    ui->provinceComboBox->addItem("宁夏");
    ui->provinceComboBox->addItem("新疆");

}


MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_comboBox_activated(int index)
{
    updateListWidget(index);
}

void  MainWindow::updateListWidget(int index){
    ui->listWidget->clear();
    if (index == 0) { // 当选择 "1" 时
        ui->listWidget->addItem("问题 1-1");
        ui->listWidget->addItem("问题 1-2-1");
        ui->listWidget->addItem("问题 1-2-2");
        ui->listWidget->addItem("问题 1-3");
        ui->listWidget->addItem("问题 1-4");
        ui->listWidget->addItem("问题 1-5");
    } else if (index == 1) { // 当选择 "2" 时
        ui->listWidget->addItem("问题 2-1");
        ui->listWidget->addItem("问题 2-2");
        ui->listWidget->addItem("问题 2-3");
        ui->listWidget->addItem("问题 2-4");
        ui->listWidget->addItem("问题 2-5");
        ui->listWidget->addItem("问题 2-6-1");
        ui->listWidget->addItem("问题 2-6-2");
        ui->listWidget->addItem("问题 2-7");
        ui->listWidget->addItem("问题 2-8-1");
        ui->listWidget->addItem("问题 2-8-2");
    }else if (index == 2) { // 当选择 "3" 时
        ui->listWidget->addItem("问题 3-1");
        ui->listWidget->addItem("问题 3-2");
        ui->listWidget->addItem("问题 3-3");
    }
}

void MainWindow::setupDatabase() {
    // 配置数据库连接
    db = QSqlDatabase::addDatabase("QODBC"); //选择的模块ODBC
    db.setHostName("127.0.0.1");   //填入本地IP即可
    db.setPort(3306);              //随便填入即可
    db.setDatabaseName("zYuex");   //填入创建的ODBC数据源的名称
    db.setUserName("sa");          //登录账号
    db.setPassword("123456");      //登录密码

    if (!db.open()) {
        qDebug() << "Database connection failed:" << db.lastError().text();
    } else {
        qDebug() << "Database connected successfully.";
    }
}

/*
void MainWindow::onListWidgetItemClicked(QListWidgetItem *item) {
    QString selectedOption = item->text();
    qDebug() << "Selected option:" << selectedOption;

    // 根据选项执行存储过程或函数
    executeProcedure(selectedOption);
}
*/
void MainWindow::onOptionClicked(QListWidgetItem *item) {
    selectedOption = item->text();
    QString option = item->text(); // 获取点击的选项文本
    ui->provinceComboBox->setCurrentIndex(0); // 设置为第三个选项
    ui->startYearEdit->clear();
    ui->endYearEdit->clear();
    // 获取对应的问题
    if (optionQuestions.contains(option)) {
        QString question = optionQuestions[option];
        ui->textEdit_3->setText(question); // 设置到文本框
    } else {
        ui->textEdit_3->setText("未定义的问题");
    }
}

void MainWindow::executeProcedure() {
    ui->tableWidget->clear();

    if (!db.isOpen()) {
        qDebug() << "Database is not open.";
        return;
    }

    // 确保用户已选择 Option
    if (selectedOption.isEmpty()) {
        qDebug() << "No option selected!";
        ui->textEdit_3->setPlainText("请先选择一个选项。");
        return;
    }

    // 从 UI 获取用户输入
    QString province = ui->provinceComboBox->currentText();
    int startYear = ui->startYearEdit->toPlainText().toInt();
    int endYear = ui->endYearEdit->toPlainText().toInt();

    // 构建 SQL 存储过程调用
    QSqlQuery query;

    // 根据 selectedOption 执行不同的存储过程
    if (selectedOption == "问题 1-1") {
        // 验证输入值
        if (province.isEmpty() || startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的省份和时间范围。");
            return;
        }
        query.prepare("EXEC GetSalesByProvince :province_name, :start_year, :end_year");
        // 绑定参数
        query.bindValue(":province_name", province);
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    } else if (selectedOption == "问题 1-2-1") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("EXEC GetMaxMinSalesByYearRange :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    } else if (selectedOption == "问题 1-2-2") {
        query.prepare("EXEC AnalyzeMaxMinSales");
    } else if (selectedOption == "问题 1-3") {
        query.prepare("EXEC AnalyzeGrowthRates");
    }else if (selectedOption == "问题 1-4") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("AnalyzeCapitalCityContribution :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 1-5") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("EXEC AnalyzeRegionalSales :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-1") {
        // 验证输入值
        if (province.isEmpty() || startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的省份和时间范围。");
            return;
        }
        query.prepare("EXEC AnalyzeIncomeGrowthRate :start_year, :end_year, :province_name");
        // 绑定参数
        query.bindValue(":province_name", province);
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-2") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("AnalyzeRegionIncomeDifference :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-3") {
        // 验证输入值
        if (province.isEmpty() || startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的省份和时间范围。");
            return;
        }
        query.prepare("EXEC AnalyzeIndividualIncome :province_name, :start_year, :end_year");
        // 绑定参数
        query.bindValue(":province_name", province);
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-4") {
        // 验证输入值
        if (province.isEmpty() || startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的省份和时间范围。");
            return;
        }
        query.prepare("EXEC AnalyzeIncomePercentageChange :province_name, :start_year, :end_year");
        // 绑定参数
        query.bindValue(":province_name", province);
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-5") {
        if (startYear <= 0 ) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间。");
            return;
        }
        query.prepare("EXEC GetTopAndBottomRegions :start_year");
        query.bindValue(":start_year", startYear);
    }else if (selectedOption == "问题 2-6-1") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("EXEC GetIncomeLevelStatistics :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-6-2") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("EXEC GetExtremeLevelRegions :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-7") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("EXEC CompareRegionalGrowthRate :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-8-1") {
        if (province.isEmpty() || startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的省份和时间范围。");
            return;
        }
        query.prepare("EXEC AnalyzeIncomeAndSalesTrends :province_name,  :start_year, :end_year");
        query.bindValue(":province_name", province);
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }else if (selectedOption == "问题 2-8-2") {
        query.prepare("EXEC GetMatchingRegions");
    }
    else if (selectedOption == "问题 3-1") {
        if (province.isEmpty() || startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的省份和时间范围。");
            return;
        }
        query.prepare("EXEC GetGDPByRegionAndTime :province_name,  :start_year, :end_year");
        query.bindValue(":province_name", province);
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }
    else if (selectedOption == "问题 3-2") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("EXEC GetMaxAndMinGDPByTime :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }
    else if (selectedOption == "问题 3-3") {
        if (startYear <= 0 || endYear <= 0 || startYear > endYear) {
            qDebug() << "Invalid input values!";
            ui->textEdit_3->setPlainText("请输入有效的时间范围。");
            return;
        }
        query.prepare("EXEC GetRegionalGDP :start_year, :end_year");
        query.bindValue(":start_year", startYear);
        query.bindValue(":end_year", endYear);
    }
    //还少2-5
    else {
        qDebug() << "Unsupported option:" << selectedOption;
        ui->textEdit_3->setPlainText("不支持的选项，请选择其他选项。");
        return;
    }


    // 执行查询
    if (!query.exec()) {
        qDebug() << "Failed to execute procedure:" << query.lastError().text();
        ui->textEdit_3->setPlainText("查询失败，请检查输入值和数据库连接。");
        return;
    }

    // 显示查询结果
    displayResults(query);
}

void MainWindow::displayResults(QSqlQuery &query) {
    ui->tableWidget->clear(); // 清空TableWidget

    // 设置表头
    QStringList headers;
    for (int i = 0; i < query.record().count(); ++i) {
        headers << query.record().fieldName(i);
    }
    ui->tableWidget->setColumnCount(headers.size());
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // 填充数据
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < query.record().count(); ++col) {
            //qDebug() << "Column" << col << ":" << query.value(col).toString();
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            ui->tableWidget->setItem(row, col, item);
        }
        ++row;
    }
    // 自动调整列宽
    ui->tableWidget->resizeColumnsToContents();

}



