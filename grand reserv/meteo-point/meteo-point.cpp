#include "meteo-point.h"
#include "meteo-point_p.h"


QMeteoPoint::QMeteoPoint(QMeteoPointPrivate&& dd, QObject* parent)
    :QObject(parent),
      d_ptr(&dd)
{
    Q_D(QMeteoPoint);
    d->q_ptr = this;
    d->init();
    std::cout << "init QMeteoPoint" << std::endl;
}

QMeteoPoint::QMeteoPoint(logHandle log, QObject* parent) : QObject(parent), d_ptr(new QMeteoPointPrivate(log))
{
    Q_D(QMeteoPoint);
    d->q_ptr = this;
    d->init();
    std::cout << "init QMeteoPoint" << std::endl;
}

QMeteoPoint::~QMeteoPoint()
{
    delete d_ptr;
}

void QMeteoPoint::checkMeteoPoint(QDate date, unsigned int hour, float lon, float lat, unsigned int param)
{
    Q_D(QMeteoPoint);
    std::cout << "checkMeteoPoint from public Point" << std::endl;
    d->checkMeteoPoint(date, hour, lon, lat, param);
}
