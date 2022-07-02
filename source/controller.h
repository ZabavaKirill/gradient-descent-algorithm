#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <mainwindow.h>
#include <modeling.h>

class Controller : public QObject
{
    Q_OBJECT

public:
    Controller(MainWindow& w);
    ~Controller();

private:
    MainWindow* view;
    modeling* model;
    QVector<QVector <double>> data;

    QTimer* timer;

private Q_SLOTS:
    void onPIDenabled();
    void onPIDdisabled();
    void onOptimization();
    void optimizationStep();
    void onEvaulationGraph();
    void onDI1Dependence();
    void onDI2Dependence();
    void onDI3Dependence();
    void onQDependencies();
    void onQ1Dependence();
    void onQ2Dependence();
    void onQ3Dependence();
};

#endif // CONTROLLER_H
