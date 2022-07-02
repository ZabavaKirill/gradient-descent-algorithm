#ifndef MODELING_H
#define MODELING_H
#include <QVector>

class modeling
{
public:
    modeling();

    void setParameters(double k, double T1, double T2, double tau, double l, double dt);
    void setParameters(double k, double T1, double T2, double tau, double q1, double q2, double q3, double l, double dt);
    QVector<QVector <double>> getGraph();
    QVector<QVector <double>> getGraphWithPID();
    QVector <QVector <double>> optimization();
    void setQ1(double q1);
    void setQ2(double q2);
    void setQ3(double q3);
    double getQ1();
    double getQ2();
    double getQ3();
private:
    double k, T1, T2, q1, q2, q3, tau, l, dt;

    double RungeKutta(double a0, double alpha, double x, double z);
};

#endif // MODELING_H
