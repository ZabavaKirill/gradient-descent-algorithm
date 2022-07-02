#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVector>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void setDataOnGraph(QVector <double> xAxis, QVector <double> yAxis, QString xLabel, QString yLabel);
    void setDataOnGraph(QVector <double> xAxis, QVector <double> yAxis, QString xLabel, QString yLabel, QString legend);
    void addDataOnGraph(QVector <double> xAxis, QVector <double> yAxis, QColor color);
    void addDataOnGraph(QVector <double> xAxis, QVector <double> yAxis, QString legend, QColor color);
    void setQ1(double);
    void setQ2(double);
    void setQ3(double);
    void setI(double);
    double getQ1();
    double getQ2();
    double getQ3();
    double getOptQ1();
    double getOptQ2();
    double getOptQ3();
    double getK();
    double getT1();
    double getT2();
    double getTau();
    double getL();
    double getDt();

Q_SIGNALS:
    void PIDenabled();
    void PIDdisabled();
    void optimization();
    void evaulationGraph();
    void dI1Dependence();
    void dI2Dependence();
    void dI3Dependence();
    void QDependencies();
    void Q1Dependence();
    void Q2Dependence();
    void Q3Dependence();
private:
    Ui::MainWindow *ui;
private Q_SLOTS:
    void PIDchanged();
};
#endif // MAINWINDOW_H
