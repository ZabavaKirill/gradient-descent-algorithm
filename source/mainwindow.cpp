#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <modeling.h>
#include <QVector>
#include <QFile>
#include <QDir>

/*
    В данном классе реализован графический интерфейс приложения
*/
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->comboBox->addItem("График переходной функции");
    ui->comboBox->addItem("Оптимизация параметров ПИД регулятора");
    ui->comboBox->addItem("График модульного критерия");
    ui->comboBox->addItem("График зависимости dI/dQ1");
    ui->comboBox->addItem("График зависимости dI/dQ2");
    ui->comboBox->addItem("График зависимости dI/dQ3");
    ui->comboBox->addItem("Графики Q1, Q2, Q3");
    ui->comboBox->addItem("Графики Q1");
    ui->comboBox->addItem("Графики Q2");
    ui->comboBox->addItem("Графики Q3");

    QString path = QDir::currentPath();
    path.append("/res/qss/style.qss");
    QFile style(path);
    style.open(QIODevice::ReadOnly);
    QString s = style.readAll();
    style.close();
    this->setStyleSheet("background-color: rgb(64,64,64);");
    ui->pushButton->setStyleSheet(s);
    ui->title->setStyleSheet(s);
    ui->groupBox->setStyleSheet(s);
    ui->PID->setStyleSheet(s);
    ui->groupBox_2->setStyleSheet(s);
    ui->groupBox_3->setStyleSheet(s);

    /*ui->plot->setBackground(QColor(128, 128, 128));
    ui->plot->xAxis->setTickLabelColor(Qt::white);
    ui->plot->xAxis->setLabelColor(Qt::white);
    ui->plot->yAxis->setTickLabelColor(Qt::white);
    ui->plot->yAxis->setLabelColor(Qt::white);*/

    ui->plot->xAxis->setTickLabelFont(QFont("Tahoma", 10));
    ui->plot->xAxis->setLabelFont(QFont("Tahoma", 14));

    ui->plot->yAxis->setTickLabelFont(QFont("Tahoma", 10));
    ui->plot->yAxis->setLabelFont(QFont("Tahoma", 14));


    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot->axisRect()->setupFullAxesBox();

    connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(PIDchanged()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
    Выбор типа графика для построения
*/
void MainWindow::PIDchanged() {
    switch (ui->comboBox->currentIndex()) {
        case 0: {
            if(ui->PID->isChecked())
               emit PIDenabled();
            else
               emit PIDdisabled();

            break;
        }
        case 1: {
            ui->PID->setChecked(true);
            emit optimization();
            break;
        }
        case 2: {
            ui->PID->setChecked(true);
            emit evaulationGraph();
            break;
        }
        case 3: {
            ui->PID->setChecked(true);
            emit dI1Dependence();
            break;
        }
        case 4: {
            ui->PID->setChecked(true);
            emit dI2Dependence();
            break;
        }
        case 5: {
            ui->PID->setChecked(true);
            emit dI3Dependence();
            break;
        }
        case 6: {
            ui->PID->setChecked(true);
            emit QDependencies();
            break;
        }
        case 7: {
            ui->PID->setChecked(true);
            emit Q1Dependence();
            break;
        }
        case 8: {
            ui->PID->setChecked(true);
            emit Q2Dependence();
            break;
        }
        case 9: {
            ui->PID->setChecked(true);
            emit Q3Dependence();
            break;
        }
    }

}

/*
    Построение графика без легенды
*/
void MainWindow::setDataOnGraph(QVector <double> xAxis, QVector <double> yAxis, QString xLabel, QString yLabel) {
    ui->plot->legend->setVisible(false);
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    ui->plot->graph(0)->setData(xAxis, yAxis);
    ui->plot->graph(0)->setPen(QPen(Qt::red, 2));
    ui->plot->xAxis->setLabel(xLabel);
    ui->plot->yAxis->setLabel(yLabel);
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

/*
    Построение графика с легендой
*/
void MainWindow::setDataOnGraph(QVector <double> xAxis, QVector <double> yAxis, QString xLabel, QString yLabel, QString legend) {
    this->setDataOnGraph(xAxis, yAxis, xLabel, yLabel);
    ui->plot->graph(0)->setName(legend);
    ui->plot->legend->setVisible(true);
}

/*
    Добавление графика без легенды
*/
void MainWindow::addDataOnGraph(QVector<double> xAxis, QVector<double> yAxis, QColor color) {
    ui->plot->addGraph();
    ui->plot->graph(ui->plot->graphCount()-1)->setData(xAxis, yAxis);
    ui->plot->graph(ui->plot->graphCount()-1)->setPen(QPen(color, 2));
    ui->plot->rescaleAxes();
    ui->plot->replot();
}

/*
    Добавление графика с легендой
*/
void MainWindow::addDataOnGraph(QVector<double> xAxis, QVector<double> yAxis, QString legend, QColor color) {
    ui->plot->addGraph();
    ui->plot->graph(ui->plot->graphCount()-1)->setData(xAxis, yAxis);
    ui->plot->graph(ui->plot->graphCount()-1)->setName(legend);
    ui->plot->graph(ui->plot->graphCount()-1)->setPen(QPen(color, 2));

    ui->plot->rescaleAxes();
    ui->plot->replot();
}

double MainWindow::getQ1() {
    return ui->q1->value();
}
double MainWindow::getOptQ1() {
    return ui->optQ1->text().toDouble();
}
double MainWindow::getOptQ2() {
    return ui->optQ2->text().toDouble();
}
double MainWindow::getOptQ3() {
    return ui->optQ3->text().toDouble();
}
double MainWindow::getQ2() {
    return ui->q2->value();
}
double MainWindow::getQ3() {
    return ui->q3->value();
}
double MainWindow::getK() {
    return ui->k->text().toDouble();
}
double MainWindow::getT1() {
    return ui->T1->text().toDouble();
}
double MainWindow::getT2() {
    return ui->T2->text().toDouble();
}
double MainWindow::getTau() {
    return ui->tau->text().toDouble();
}
double MainWindow::getL() {
    return ui->L->text().toDouble();
}
double MainWindow::getDt() {
    return ui->dt->text().toDouble();
}
void MainWindow::setQ1(double q1) {
    ui->optQ1->setText(QString::number(q1, 'f', 2));
}
void MainWindow::setQ2(double q2) {
    ui->optQ2->setText(QString::number(q2, 'f', 2));
}
void MainWindow::setQ3(double q3) {
    ui->optQ3->setText(QString::number(q3, 'f', 2));
}
void MainWindow::setI(double I) {
    ui->Icrit->setText(QString::number(I, 'f', 2));
}
