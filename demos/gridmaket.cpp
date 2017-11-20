#include <iostream>
#include <vector>
#include <iomanip>
#include <cmath>

class Point
{
private:

    float lon_;
    float lat_;
    float z_;

public:
    
    float getLon()
    {
        return lon_;
    }

    float getLat()
    {
        return lat_;
    }

    float getZ()
    {
        return z_;
    }

    void setLon(float lon)
    {
        lon_ = lon;
    }

    void setLat(float lat)
    {
        lat_ = lat;
    }

    void setZ(float z)
    {
        z_ = z;
    }

};


int getClosestRegularPoint( float lon, float lat )
{
    int position = (int)(lat / 2.5) * 49 + (int)(lon / 2.5);
    return position;
}

int getClosestReducedPoint( float lon, float lat )
{
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
    return position;
}


float cubicInterpolate(float p[4], float x)
{
    float y = p[1] + (-0.5*p[0] + 0.5*p[2])*x + (p[0] - 2.5*p[1] + 2.0*p[2] - 0.5*p[3])*x*x + (-0.5*p[0] + 1.5*p[1] - 1.5*p[2] + 0.5*p[3])*x*x*x;
    return y;
}

float bicubicInterpolate (float p[4][4], float lon, float lat)
{
    float tmp[4];
    tmp[0] = cubicInterpolate(p[0], lon);
    tmp[1] = cubicInterpolate(p[1], lon);
    tmp[2] = cubicInterpolate(p[2], lon);
    tmp[3] = cubicInterpolate(p[3], lon);
    return cubicInterpolate(tmp, lat);
}


float getInterpolatedRegularPoint( std::vector<Point> *vec, float lon, float lat )
{
    int position = getClosestRegularPoint(lon, lat);

    std::cout << vec->at(position).getLon() << ' ' << vec->at(position).getLat() << ' ' << vec->at(position).getZ() << std::endl;

    float p[4][4];
    for (auto j = 0; j < 4; ++j)
        for (auto i = 0; i < 4; ++i)
        {
            p[j][i] = vec->at(position - 1 + i - 49 + 49*j).getZ();
            std::cout << p[j][i] << std::endl;
        }

    std::cout << (lon - ((position - (int)(lat / 2.5) * 49) * 2.5)) / 2.5 << ' ' << (lat - ((position - (int)(lon / 2.5)) / 49) * 2.5) / 2.5 << std::endl;

    return bicubicInterpolate(p, (lon - ((position - (int)(lat / 2.5) * 49) * 2.5)) / 2.5, (lat - ((position - (int)(lon / 2.5)) / 49) * 2.5) / 2.5);
}

float getInterpolatedReducedPoint( std::vector<Point> *vec, float lon, float lat )
{
    int position = getClosestReducedPoint(lon, lat);

    std::cout << vec->at(position).getLon() << ' ' << vec->at(position).getLat() << ' ' << vec->at(position).getZ() << std::endl;
    
    float p[4][4];

    return vec->at(position).getZ();
}

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

int main()
{
    std::vector<Point>RegularGrid(1813);

    for (auto j = 0; j < 37; ++j)
        for (auto i = 0; i < 49; ++i)
        {
            RegularGrid[j*49 + i].setLon(i*2.5);
            RegularGrid[j*49 + i].setLat(j*2.5);
            RegularGrid[j*49 + i].setZ(37 - j + (float)i / 49);
        }

    for (auto j = 0; j < 37; ++j)
    {
        for (auto i = 0; i < 49; ++i)
        {
            //std::cout << std::fixed << std::setprecision(3) << RegularGrid[j*49 + i].getZ() << '|';
        }

        //std::cout << std::endl << std::endl;
    }

    std::cout /*<< std::fixed << std::setprecision(3)*/ << getInterpolatedRegularPoint(&RegularGrid, 60.0, 45.0) << std::endl;

//------------------------------------------------------------------------------------------------------------------------------------------------------------------------

    std::vector<Point>ReducedGrid(3447);
    int range = 0;

    for (auto j = 0; j < 73; ++j)
    {
        int npoints = (int)(2.0 + (90.0 / 1.25) * cos(j * 1.25 * std::atan2(0, -1) / 180.0));
        if(!j)npoints--;
        float step = 90.0  / npoints;
        
        for (auto i = 0; i < npoints; ++i)
        {
            ReducedGrid[range + i].setLon(i*step);
            ReducedGrid[range + i].setLat(j*1.25);
            ReducedGrid[range + i].setZ(73 - j + (float)i / npoints);
        }
        range += npoints;
    }
    range=0;

    for (auto j = 0; j < 73; ++j)
    {
        int npoints = (int)(2.0 + (90.0 / 1.25) * cos(j * 1.25 * std::atan2(0, -1) / 180.0));
        if(!j)npoints--;
        float step = 90.0  / npoints;

        for (auto i = 0; i < npoints; ++i)
        {
            //std::cout << std::fixed << std::setprecision(3) << ReducedGrid[range + i].getZ() << '|';
        }
        range += npoints;

        //std::cout << std::endl << std::endl;
    }
    range=0;

    std::cout /*<< std::fixed << std::setprecision(3)*/ << getInterpolatedReducedPoint(&ReducedGrid, 45.0, 45.0) << std::endl;


    return 0;
}
