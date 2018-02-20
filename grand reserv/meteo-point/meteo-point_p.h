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

        meteo_source->init();
        std::cout << "init meteo_source" << std::endl;
    }

private slots:
    void onGribDataReloaded()
    {
        if (hasTask)
        {
            grids_total_info = meteo_source->getGribData();

            std::cout << "getGribData form private Point, going calculate" << std::endl;
            chooseGribData();
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

private:

    void chooseGribData()
    {
        Q_Q(QMeteoPoint);
        grib_data_index_t index;

        std::cout << "choose grib from vector from private Point" << std::endl;
        for (index = 0; index < (*(grids_total_info)).size(); ++index)
        {
            const auto& item = (*(grids_total_info)).at(index);

            if (item.indicator_of_parameter_and_units() == param_
                    && item.grid_identification() == 37)
            {
                if (item.lat_long_gauss_grid().first_point_latitude() > item.lat_long_gauss_grid().last_point_latitude())
                {
                    if (!(item.lat_long_gauss_grid().first_point_latitude() <= lat_
                          || lat_ < item.lat_long_gauss_grid().last_point_latitude()))
                    {
                        continue;
                    }
                }
                else
                {
                    if (!(item.lat_long_gauss_grid().first_point_latitude() <= lat_
                          && lat_ < item.lat_long_gauss_grid().last_point_latitude()))
                    {
                        continue;
                    }
                }

                if (item.lat_long_gauss_grid().first_point_longitude() > item.lat_long_gauss_grid().last_point_longitude())
                {
                    if (!(item.lat_long_gauss_grid().first_point_longitude() <= lon_
                          || lon_ < item.lat_long_gauss_grid().last_point_longitude()))
                    {
                        continue;
                    }
                }
                else
                {
                    if (!(item.lat_long_gauss_grid().first_point_longitude() <= lon_
                          && lon_ < item.lat_long_gauss_grid().last_point_longitude()))
                    {
                        continue;
                    }
                }

                data = item;
                std::cout << "longitude step is " << data.lat_long_gauss_grid().longitude_direction_increment() << std::endl;
                std::cout << "latitude step is " << data.lat_long_gauss_grid().latitude_direction_increment() << std::endl;
                std::cout << "longitude first is " << data.lat_long_gauss_grid().first_point_longitude() << std::endl;
                std::cout << "latitude first is " << data.lat_long_gauss_grid().first_point_latitude() << std::endl;
                std::cout << "grib was chosen" << std::endl;
                calcMeteoPoint();
                break;
            }
        }
        if (index == (*(grids_total_info)).size()) emit (q_ptr->meteoPointCalculated());  // add "fail" parameter for signal
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
        std::cout << lon_ << " " << lat_ << std::endl;

        if ((data.lat_long_gauss_grid().first_point_longitude() > data.lat_long_gauss_grid().last_point_longitude())
                && lon_ < data.lat_long_gauss_grid().last_point_longitude())
        {
            lon_+=360000;
        }
        if ((data.lat_long_gauss_grid().first_point_latitude() > data.lat_long_gauss_grid().last_point_latitude())
                && lat_ < data.lat_long_gauss_grid().last_point_latitude())
        {
            lat_+=360000;
        }

        float absolute_lon = (lon_-data.lat_long_gauss_grid().first_point_longitude())/1000;
        float absolute_lat = (lat_-data.lat_long_gauss_grid().first_point_latitude())/1000;
        std::pair<int, int> positions = getClosestReducedPoint(absolute_lon, absolute_lat);

        new_grid_point_data = thinned_grid.at(positions.first)*(1-absolute_lon/90)*(1-absolute_lat/90)
                + thinned_grid.at(positions.first+1)*(absolute_lon/90)*(1-absolute_lat/90)
                + thinned_grid.at(positions.second)*(1-absolute_lon/90)*(absolute_lat/90)
                + thinned_grid.at(positions.second+1)*(absolute_lon/90)*(absolute_lat/90);

        std::cout << "now we have temperature in closest point, it is " << new_grid_point_data
                  << std::endl;

        meteo_point = std::make_shared<meteo_point_t>();
        meteo_point.get()->push_back(lon_);
        meteo_point.get()->push_back(lat_);
        meteo_point.get()->push_back(param_);
        meteo_point.get()->push_back(new_grid_point_data);
        q->meteo_point = meteo_point;

        std::cout << "calculated!" << std::endl;
        hasTask = false;
        emit (q->meteoPointCalculated());
    }

    void checkMeteoPoint(QDate date, unsigned int hour, float lon, float lat, unsigned int param)
    {
        lon_ = lon;
        lat_ = lat;
        param_ = param;
        hasTask = true;

        std::cout << "check MeteoPoint from private Point" << std::endl;
        meteo_source->loadGribData(date, hour);
    }

    std::pair<int, int> getClosestReducedPoint(float lon, float lat)
    {
        std::cout << "getting closest point" << std::endl;
        std::cout << lon << std::endl;
        std::cout << lat << std::endl;
        int bottom_position = 0;
        int top_position = 0;
        int npoints = 72;
        int limit = lat / 1.25;

        for (auto j = 0; j < limit; ++j)
        {
            npoints = (int)(2.0 + (90.0 / 1.25) * cos(j * 1.25 * std::atan2(0, -1) / 180.0));
            if(!j)npoints--;
            bottom_position += npoints;
        }

        top_position = bottom_position;
        bottom_position += lon / (90.0 / npoints);
        std::cout << "return bottom new position in vector " << bottom_position << std::endl;

        npoints = (int)(2.0 + (90.0 / 1.25) * cos(limit * 1.25 * std::atan2(0, -1) / 180.0));
        top_position += npoints;
        top_position += lon / (90.0 / npoints);
        std::cout << "return top new position in vector " << top_position << std::endl;

        std::pair<int, int> left_border = std::make_pair(bottom_position, top_position);

        return left_border;
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

};

#endif
