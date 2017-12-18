#ifndef METEO_POINT_P_H
#define METEO_POINT_P_H

#include "meteo-point.h"
#include <stdint.h>
#include "meteo-source.h"


class QMeteoPointPrivate : public QObject, public IceMeteoStatus
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(QMeteoPoint)

    std::shared_ptr<meteo_grids_info_t > grids_total_info;
    std::shared_ptr<grib_all_data_t > meteo_grib_data_all;
    std::shared_ptr<meteo_point_t> meteo_point;

private:
    LogWrapper log;
    MeteoSource* meteo_source;

    GribData data;
    float lon_;
    float lat_;
    unsigned int param_;
    bool hasTask;

public:
    QMeteoPoint* q_ptr;
    QMeteoPointPrivate(logHandle log_) : log(log_, "QMeteoPoint")
    {
    }

    virtual ~QMeteoPointPrivate(){}

    void init()
    {
        Q_Q(QMeteoPoint);
        try
        {
            meteo_source = new MeteoSource(this);
        }
        catch (std::bad_alloc&)
        {
            log.fatal("Непоправимая ошибка выделения памяти");
            return;
        }

        hasTask = false;
        bool is_ok = false;

        is_ok = connect(meteo_source, SIGNAL(gribDataReloaded()), SLOT(onGribDataReloaded())); assert(is_ok);
        is_ok = connect(meteo_source, SIGNAL(gribAllDataReloaded()), SLOT(onAllGribDataReloaded())); assert(is_ok);

        is_ok = connect(this, SIGNAL(gribDataReloaded()), SLOT(chooseGribData())); assert(is_ok);
        is_ok = connect(this, SIGNAL(gribDataChoosed()), SLOT(calcMeteoPoint())); assert(is_ok);

        meteo_source->init();
        //        QDate d1(2016, 2, 7);
        //        meteo_source->loadGribData(d1, 12);
        std::cout << "init meteo_source" << std::endl;
    }

private slots:
    void onGribDataReloaded()
    {
        if (hasTask)
        {
            grids_total_info = meteo_source->getGribData();

            std::cout << "getGribData form private Point, going calculate" << std::endl;
            emit gribDataReloaded();
        }
    }

    void onAllGribDataReloaded()
    {
        Q_Q(QMeteoPoint);
        meteo_grib_data_all = meteo_source->getAllGribData();
        q->meteo_grib_data_all = meteo_grib_data_all;

        std::cout << "get meta from private Point" << std::endl;
        emit (q->gribAllDataReloaded());
    }

    void chooseGribData()
    {
        std::cout << "choose grib from vector from private Point" << std::endl;
        for (grib_data_index_t index = 0; index < (*(grids_total_info)).size(); ++index)
        {
            const auto& item = (*(grids_total_info)).at(index);
            if (item.indicator_of_parameter_and_units() == param_
                    //&& item.lat_long_gauss_grid().first_point_latitude() <= lon_*1000
                    //&& lon_*1000 <= item.lat_long_gauss_grid().last_point_latitude()
                    //&& item.lat_long_gauss_grid().first_point_longitude() <= lat_*1000
                    //&& lat_*1000 <= item.lat_long_gauss_grid().last_point_longitude()
                    && item.grid_identification() == 37)
                // because lat_ and lon_ are temporary absolute - from 0 till 90 - fix it!
            {
                data = item;
                std::cout << "longitude step is " << data.lat_long_gauss_grid().longitude_direction_increment() << std::endl;
                std::cout << "latitude step is " << data.lat_long_gauss_grid().latitude_direction_increment() << std::endl;
                std::cout << "grib was chosen" << std::endl;
                emit gribDataChoosed();
                break;
            }
        }
    }

    void calcMeteoPoint()
    {
        Q_Q(QMeteoPoint);
        std::vector<float> thinned_grid;
        float new_grid_point_data;

        int i = 0;

        std::cout << "going calculate from private Point" << std::endl;
        for (auto grid_index = 0; grid_index < data.grid_point_data_size(); ++grid_index)
        {
            thinned_grid.push_back((float)data.grid_point_data(grid_index));
            ++i;
        }
        std::cout << "total amount of points is " << i << std::endl;

        new_grid_point_data = thinned_grid.at(getClosestReducedPoint(lon_, lat_));
//        new_grid_point_data = thinned_grid.at(0);
        std::cout << "now we have temperature in closest point, it is " << new_grid_point_data
                  << std::endl;

        meteo_point = std::make_shared<meteo_point_t>();
        meteo_point.get()->push_back(lon_);
        meteo_point.get()->push_back(lat_);
        meteo_point.get()->push_back(param_);
        meteo_point.get()->push_back(new_grid_point_data);
        q->meteo_point = meteo_point;

        std::cout << "calculated!" << std::endl;
        emit (q->meteoPointCalculated());
    }

private:

    void checkMeteoPoint(QDate date, unsigned int hour, float lon, float lat, unsigned int param)
    {
        lon_ = lon;
        lat_ = lat;
        param_ = param;
        hasTask = true;

        std::cout << "check MeteoPoint from private Point" << std::endl;
        meteo_source->loadGribData(date, hour);
    }

    int getClosestReducedPoint(float lon, float lat)
    {
        std::cout << "getting closest point" << std::endl;
        int position = 0;
        int npoints = 0;
        int limit = lat / 1.25;

        for (auto j = 0; j < limit; ++j)
        {
            npoints = (int)(2.0 + (90.0 / 1.25) * cos(j * 1.25 * std::atan2(0, -1) / 180.0));
            if(!j)npoints--;
            position += npoints;
        }

        position += lon / (90.0 / npoints);
        std::cout << "return new position in vector " << position << std::endl;
        return position;
    }

    void ready(QString task) override
    {

    }
    void progress(QString task, unsigned char percent) override
    {

    }
    void error(QString task, QString error) override
    {

    }

signals:
    void gribDataReloaded();
    void gribDataChoosed();

};

#endif
