#include "modeling.h"
#include <QVector>
#include <QDebug>
#include <QtMath>

/*
    В данном классе реализовано моделирование апериодического звена второго порядка
    с заданными параметрами. Моделирование реализовано разбиением исходного звена
    на 2 последовательно соединённых апериодических звена первого порядка и с дальнейшим
    использованием метода Рунге-Кутты.
    Реализована возможность моделирования с ПИД-регулятором с заданными параметрами q1, q2, q3.
    Реализован алгоритм параметрической оптимизации (метод градиентного спуска) для поиска оптимальных параметров ПИД-регулятора.
*/
modeling::modeling()
{

}

/*
    Установка параметров звена
*/
void modeling::setParameters(double k, double T1, double T2, double tau, double l, double dt) {
    this->k = k;
    this->T1 = T1;
    this->T2 = T2;
    this->tau = tau;
    this->l = l;
    this->dt = dt;
}

/*
    Установка параметров звена и параметров ПИД-регулятора
*/
void modeling::setParameters(double k, double T1, double T2, double tau, double q1, double q2, double q3, double l, double dt) {
    this->q1 = q1;
    this->q2 = q2;
    this->q3 = q3;
    setParameters(k, T1, T2, tau, l, dt);
}

/*
    Моделирование без ПИД-регулятора
*/
QVector <QVector <double>> modeling::getGraph() {
    QVector <QVector <double>> data;
    QVector <double> x, y, delay;
    double std = 0, err, in=1, out=0, y1=0, y2=0;
    int ns = tau/dt, i=0;

    while(std < l) {
        err = in - out;

        y1 += RungeKutta(1/T1, k/T1, err, y1);   //разбиение на 2 последовательных звена:
        y2 += RungeKutta(1/T2, 1/T2, y1, y2);    //y1 - выход первого, y2 - выход второго

        y.append(y2);
        //out = y2;

        if(i > ns) {   //запаздывание
            out = y.at(i-ns);
        }
        else
            out = 0;
        i++;


        x.append(std);
        //y.append(out);
        delay.append(out);

        std += dt;
    }

    data.append(x);
    //data.append(y);
    data.append(delay);

    return data;
}


/*
    Моделирование с ПИД-регулятором
*/
QVector <QVector <double>> modeling::getGraphWithPID() {
    QVector <QVector <double>> data;
    QVector <double> x, y, delay;
    double std = 0, err, prevErr=0, in=1, out=0, y1=0, y2=0, I=0, ev=0, PIDoutput;
    int ns = tau/dt, i=0;

    while(std < l) {
        err = in - out;
        ev += std::abs(err)*dt;
        //ev += err*err*dt;            //квадратичный критерий качества
        I += err*dt;
        PIDoutput = q1*err + q2*I + q3*(err - prevErr)/dt;  //выход пид регулятора

        y1 += RungeKutta(1/T1, k/T1, PIDoutput, y1);
        y2 += RungeKutta(1/T2, 1/T2, y1, y2);

        y.append(y2);
        //out = y2;

        if(i > ns) {   //запаздывание
            out = y.at(i-ns);
        }
        else
            out = 0;
        i++;

        x.append(std);
        //y.append(out);
        delay.append(out);

        prevErr = err;
        std += dt;
    }

    data.append(x);
    //data.append(y);
    data.append(delay);

    return data;

}

/*
    Метод Рунге-Кутты для моделирования апериодического звена 1го порядка
*/
double modeling::RungeKutta(double a0, double alpha, double x, double z) {
    double K1, K2, K3, K4;

    K1 = (-a0*z + alpha*x)*dt;
    K2 = (-a0*(z + K1/2) + alpha*x)*dt;
    K3 = (-a0*(z + K2/2) + alpha*x)*dt;
    K4 = (-a0*(z + K3) + alpha*x)*dt;

    return (K1 + 2*K2 + 2*K3 + K4)/6;
}

/*
    Алгоритм параметрической оптимизации для нахождения оптимальных параметров ПИД-регулятора
*/
QVector <QVector <double>> modeling::optimization() {
    QVector <QVector <double>> data;
    QVector <double>    xVector, yVector, evVector,
                        xi1Vector, xi2Vector, xi3Vector,
                        dI1Vector, dI2Vector, dI3Vector,
                        q1Vector, q2Vector, q3Vector;
    QVector <double> xi1DelayVector(int(l/dt)), xi2DelayVector(int(l/dt)), xi3DelayVector(int(l/dt));

    double std, err, prevErr, in=1, out,
            y1, y2, y11, y12, y13, y21, y22, y23,
            I, IXi1, IXi2, IXi3,
            PIDoutput,
            xi1, prevXi1,
            xi2, prevXi2,
            xi3, prevXi3,
            dI1, dI2, dI3,
            du1, du2, du3,
            h=0.03, ev, prevEv=0,
            prevQ1, prevQ2, prevQ3;
    int ns = tau/dt, i, step=0;

    do
    {
        std=0, out=0, prevErr=0;
        y1=0, y2=0, y11=0, y12=0, y13=0, y21=0, y22=0, y23=0;
        I=0, IXi1=0, IXi2=0, IXi3=0;
        xi1=0, prevXi1=0;
        xi2=0, prevXi2=0;
        xi3=0, prevXi3=0;
        dI1=0, dI2=0, dI3=0;
        ev=0;
        i=0;
        yVector.clear();
        xi1Vector.clear();
        xi2Vector.clear();
        xi3Vector.clear();
        while(std < l) {
            err = in - out;
            //ev += err*err*dt;         //квадратичный критерий
            ev += std::abs(err) * dt;

            I += err*dt;
            IXi1 += xi1*dt;
            IXi2 += xi2*dt;
            IXi3 += xi3*dt;

            PIDoutput = q1*err + q2*I + q3*(err - prevErr)/dt;   //пид регулятор

            du1 = err - q1*xi1 - q2*IXi1 - q3*(xi1-prevXi1)/dt;                 //du/dq1
            du2 = -q1*xi2 + I - q2*IXi2 - q3*(xi2-prevXi2)/dt;                  //du/dq2
            du3 = -q1*xi3 - q2*IXi3 + (err-prevErr)/dt - q3*(xi3-prevXi3)/dt;   //du/dq3

            y1 += RungeKutta(1/T1, k/T1, PIDoutput, y1);       //вычисление функций чувствительности
            y2 += RungeKutta(1/T2, 1/T2, y1, y2);

            y11 += RungeKutta(1/T1, k/T1, du1, y11);
            y21 += RungeKutta(1/T2, 1/T2, y11, y21);

            y12 += RungeKutta(1/T1, k/T1, du2, y12);
            y22 += RungeKutta(1/T2, 1/T2, y12, y22);

            y13 += RungeKutta(1/T1, k/T1, du3, y13);
            y23 += RungeKutta(1/T2, 1/T2, y13, y23);

            yVector.append(y2);
            xi1Vector.append(y21);
            xi2Vector.append(y22);
            xi3Vector.append(y23);

            prevXi1 = xi1;
            prevXi2 = xi2;
            prevXi3 = xi3;

            if(i > ns) {
                xi1 = xi1Vector.at(i-ns);
                xi2 = xi2Vector.at(i-ns);
                xi3 = xi3Vector.at(i-ns);
                out = yVector.at(i-ns);
            }
            else {
                xi1 = 0;
                xi2 = 0;
                xi3 = 0;
                out = 0;
            }
            i++;

            /*dI1 += (-2 * err * xi1) * dt;     //вычисление dI1/dq1, dI1/dq2, dI1/dq3 для квадратичного критерия
            dI2 += (-2 * err * xi2) * dt;
            dI3 += (-2 * err * xi3) * dt;*/

            if(err >= 0) {          //вычисление dI1/dq1, dI1/dq2, dI1/dq3  для модульного критерия
                dI1 += -xi1 * dt;
                dI2 += -xi2 * dt;
                dI3 += -xi3 * dt;
            }
            else {
                dI1 += xi1 * dt;
                dI2 += xi2 * dt;
                dI3 += xi3 * dt;
            }

            prevErr = err;
            std += dt;
        }

        prevQ1 = q1;
        prevQ2 = q2;
        prevQ3 = q3;

        q1 -= h*dI1/(qSqrt(dI1*dI1 + dI2*dI2 + dI3*dI3));         //вычисление новых параметров
        q2 -= h*dI2/(qSqrt(dI1*dI1 + dI2*dI2 + dI3*dI3));
        q3 -= h*dI3/(qSqrt(dI1*dI1 + dI2*dI2 + dI3*dI3));

        if(q1 < 0)
            q1 = prevQ1;
        if(q2 < 0)
            q2 = prevQ2;
        if(q3 < 0)
            q3 = prevQ3;

        if(ev <= prevEv)
            h *= 1.5;
        else
            h *= 0.5;

        xVector.append(step);
        evVector.append(ev);
        dI1Vector.append(dI1);
        dI2Vector.append(dI2);
        dI3Vector.append(dI3);
        q1Vector.append(q1);
        q2Vector.append(q2);
        q3Vector.append(q3);
        prevEv = ev;
        step++;
    } while(step <= 200);

    data.append(xVector);       //ось X - шаги алгоритма
    data.append(evVector);      //интегральнай оценка
    data.append(dI1Vector);     //dI/dq1
    data.append(dI2Vector);     //dI/dq2
    data.append(dI3Vector);     //dI/dq3
    data.append(q1Vector);      //q1
    data.append(q2Vector);      //q2
    data.append(q3Vector);      //q3

    return data;
}


void modeling::setQ1(double q1) {
    this->q1 = q1;
}
void modeling::setQ2(double q2) {
    this->q2 = q2;
}
void modeling::setQ3(double q3) {
    this->q3 = q3;
}
double modeling::getQ1() {
    return this->q1;
}
double modeling::getQ2() {
    return this->q2;
}
double modeling::getQ3() {
    return this->q3;
}
