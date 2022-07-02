#include "controller.h"
#include <QVector>
#include <QDebug>
#include <QTimer>

//взаимодействие между UI и логикой
Controller::Controller(MainWindow& w)
{
    view = &w;
    model = new modeling();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(), view->getL(), view->getDt());
    data = model->getGraph();
    view->setDataOnGraph(data.at(0), data.at(1), "t", "h");

    timer = new QTimer();

    connect(view, SIGNAL(PIDenabled()), this, SLOT(onPIDenabled()));
    connect(view, SIGNAL(PIDdisabled()), this, SLOT(onPIDdisabled()));
    connect(view, SIGNAL(optimization()), this, SLOT(onOptimization()));
    connect(timer, SIGNAL(timeout()), this, SLOT(optimizationStep()));
    connect(view, SIGNAL(evaulationGraph()), this, SLOT(onEvaulationGraph()));
    connect(view, SIGNAL(dI1Dependence()), this, SLOT(onDI1Dependence()));
    connect(view, SIGNAL(dI2Dependence()), this, SLOT(onDI2Dependence()));
    connect(view, SIGNAL(dI3Dependence()), this, SLOT(onDI3Dependence()));
    connect(view, SIGNAL(QDependencies()), this, SLOT(onQDependencies()));
    connect(view, SIGNAL(Q1Dependence()), this, SLOT(onQ1Dependence()));
    connect(view, SIGNAL(Q2Dependence()), this, SLOT(onQ2Dependence()));
    connect(view, SIGNAL(Q3Dependence()), this, SLOT(onQ3Dependence()));
}
Controller::~Controller() {
    delete view;
    delete model;
}

/*
    Построение графика с ПИД регулятором
*/
void Controller::onPIDenabled() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->getGraphWithPID();
    view->setDataOnGraph(data.at(0), data.at(1), "t", "h");
}

/*
    Построение графика без ПИД регулятора
*/
void Controller::onPIDdisabled() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->getGraph();
    view->setDataOnGraph(data.at(0), data.at(1), "t", "h");
}

/*
    Оптимизация параметров ПИД регулятора
*/
void Controller::onOptimization() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->optimization();
    view->setQ1(model->getQ1());
    view->setQ2(model->getQ2());
    view->setQ3(model->getQ3());
    view->setI(data.at(1).last());

    model->setQ1(view->getQ1());
    model->setQ2(view->getQ1());
    model->setQ3(view->getQ1());
    timer->start(50);
}

/*
    Пошаговая визуализация работы алгоритма градиентного спуска
*/
void Controller::optimizationStep() {
    data = model->getGraphWithPID();
    view->setDataOnGraph(data.at(0), data.at(1), "t", "h");
    /*qDebug() << "";
    qDebug() << "Q1 = " << model->getQ1();
    qDebug() << "Q2 = " << model->getQ2();
    qDebug() << "Q3 = " << model->getQ3();*/
    if(model->getQ1() < view->getOptQ1()) {
       model->setQ1(model->getQ1() + (view->getOptQ1()-view->getQ1())/50);
    }
    else if(model->getQ2() < view->getOptQ2()) {
       model->setQ2(model->getQ2() + (view->getOptQ2()-view->getQ2())/50);
    }
    else if(model->getQ3() < view->getOptQ3()) {
       model->setQ3(model->getQ3() + (view->getOptQ3()-view->getQ3())/50);
    }
    else {
        //model->setQ1(view->getOptQ1());
        //model->setQ2(view->getOptQ2());
        //model->setQ3(view->getOptQ3());
        data = model->getGraphWithPID();
        view->setDataOnGraph(data.at(0), data.at(1), "t", "h");
        timer->stop();
    }
}

/*
    Построение графика модульного критерия
*/
void Controller::onEvaulationGraph() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->optimization();
    view->setQ1(model->getQ1());
    view->setQ2(model->getQ2());
    view->setQ3(model->getQ3());
    view->setI(data.at(1).last());
    view->setDataOnGraph(data.at(0), data.at(1), "l", "I");
}

/*
    Построение графика зависимости dI/dQ1 от L
*/
void Controller::onDI1Dependence() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->optimization();
    view->setQ1(model->getQ1());
    view->setQ2(model->getQ2());
    view->setQ3(model->getQ3());
    view->setI(data.at(1).last());
    view->setDataOnGraph(data.at(0), data.at(2), "l", "dI/dQ1");
}

/*
    Построение графика зависимости dI/dQ2 от шагов алгоритма
*/
void Controller::onDI2Dependence() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->optimization();
    view->setQ1(model->getQ1());
    view->setQ2(model->getQ2());
    view->setQ3(model->getQ3());
    view->setI(data.at(1).last());
    view->setDataOnGraph(data.at(0), data.at(3), "l", "dI/dQ2");
}

/*
    Построение графика зависимости dI/dQ3 от шагов алгоритма
*/
void Controller::onDI3Dependence() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->optimization();
    view->setQ1(model->getQ1());
    view->setQ2(model->getQ2());
    view->setQ3(model->getQ3());
    view->setI(data.at(1).last());
    view->setDataOnGraph(data.at(0), data.at(4), "l", "dI/dQ3");
}

/*
    Построение графика зависимостей Q1, Q2, Q3 от шагов алгоритма
*/
void Controller::onQDependencies() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         view->getQ1(), view->getQ2(), view->getQ3(), view->getL(), view->getDt());
    data = model->optimization();
    view->setQ1(model->getQ1());
    view->setQ2(model->getQ2());
    view->setQ3(model->getQ3());
    view->setI(data.at(1).last());
    view->setDataOnGraph(data.at(0), data.at(5), "l", "Q", "Q1");
    view->addDataOnGraph(data.at(0), data.at(6), "Q2", Qt::green);
    view->addDataOnGraph(data.at(0), data.at(7), "Q3", Qt::blue);
}

/*
    Построение графика сходимости параметра Q1 из 3х различных начальных точек
*/
void Controller::onQ1Dependence() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         4, 4, 2, view->getL(), view->getDt());

    data = model->optimization();
    view->setDataOnGraph(data.at(0), data.at(5), "l", "Q1", "Нач. знач. 4");

    model->setQ1(5);
    model->setQ2(6);
    model->setQ3(1);

    data = model->optimization();
    view->addDataOnGraph(data.at(0), data.at(5), "Нач. знач. 5", Qt::green);

    model->setQ1(7);
    model->setQ2(1);
    model->setQ3(4);

    data = model->optimization();
    view->addDataOnGraph(data.at(0), data.at(5), "Нач. знач. 7", Qt::blue);
}

/*
    Построение графика сходимости параметра Q2 из 3х различных начальных точек
*/
void Controller::onQ2Dependence() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         4, 4, 2, view->getL(), view->getDt());

    data = model->optimization();
    view->setDataOnGraph(data.at(0), data.at(6), "l", "Q2", "Нач. знач. 4");

    model->setQ1(5);
    model->setQ2(6);
    model->setQ3(1);

    data = model->optimization();
    view->addDataOnGraph(data.at(0), data.at(6), "Нач. знач. 6", Qt::green);

    model->setQ1(7);
    model->setQ2(1);
    model->setQ3(4);

    data = model->optimization();
    view->addDataOnGraph(data.at(0), data.at(6), "Нач. знач. 1", Qt::blue);
}

/*
    Построение графика сходимости параметра Q3 из 3х различных начальных точек
*/
void Controller::onQ3Dependence() {
    timer->stop();
    model->setParameters(view->getK(), view->getT1(), view->getT2(), view->getTau(),
                         4, 4, 2, view->getL(), view->getDt());

    data = model->optimization();
    view->setDataOnGraph(data.at(0), data.at(7), "l", "Q3", "Нач. знач. 2");

    model->setQ1(5);
    model->setQ2(6);
    model->setQ3(1);

    data = model->optimization();
    view->addDataOnGraph(data.at(0), data.at(7), "Нач. знач. 1", Qt::green);

    model->setQ1(7);
    model->setQ2(1);
    model->setQ3(4);

    data = model->optimization();
    view->addDataOnGraph(data.at(0), data.at(7), "Нач. знач. 4", Qt::blue);
}
