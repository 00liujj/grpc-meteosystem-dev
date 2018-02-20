
#include "wchar.h"
#include <stdint.h>
#include "log.h"
#include "odf_meteo-point-tester.h"

#include <QApplication>
#include <QTextCodec>
#include "meteo-point.h"
#include <locale.h>
#include "assert.h"

#include <string>
#include "mainwindow.h"

#include <iostream>
#include <QObject>

class Tester : public MainWindow
{
    Q_OBJECT

public:
    Tester(){}
    Tester(logHandle log)
    {
        meteopoint = new QMeteoPoint(log);

        bool is_ok = false;
        std::cout << "connecting from tester" << std::endl;

        is_ok = connect(meteopoint, SIGNAL(meteoPointCalculated()), SLOT(onMeteoPointCalculated())); assert(is_ok);
        is_ok = connect(meteopoint, SIGNAL(gribAllDataReloaded()), SLOT(onAllGribDataReloaded())); assert(is_ok);

    }

public slots:
    void onAllGribDataReloaded()
    {
        std::cout << "get meta from tester" << std::endl;
        meteo_grib_data_all = meteopoint->getMeteoGribDataAll();
        std::cout << "META!" << std::endl;
    }

    void onMeteoPointCalculated()
    {
        std::cout << "get point from tester" << std::endl;
        meteo_point = meteopoint->getMeteoPoint();
        std::cout << "got it" << std::endl;
        QString out = QString::number(meteo_point.get()->at(3) - 273)
                + " is " + QString::fromStdString(getParam()) + " in POINT!\n";
        setOutput(out);

    }

    void on_pushButton_clicked()
    {
        QDate d1(2016, 2, 7);
        std::cout << "check point from tester" << std::endl;
        meteopoint->checkMeteoPoint(d1, 12, getLon(), getLat(), Parameters.at(getParam())); // d1 and 12 - from tables!
    }

signals:

private:
    QMeteoPoint* meteopoint;

    std::shared_ptr<grib_all_data_t > meteo_grib_data_all;
    std::shared_ptr<meteo_point_t> meteo_point;

    std::map<std::string, int> Parameters = {{"Temperature", 11}};
};


/// Системное имя службы
const char* SERVICE_NAME = "meteo/meteo-point-tester";

/// Отображаемое имя службы
const char* SERVICE_DISPLAY_NAME = "Тестирование расчета метеорологической информации в точке";

Tester* tester;

/// Обработчик события "Останов службы"
void service_stop()
{
    assert(tester);
}

/// Точка входа в службу
int32_t main(int argc, char* argv[], bool /*fAppMode*/, logHandle log)
{
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    QTextCodec::setCodecForTr(QTextCodec::codecForName("utf-8"));
#endif

    QApplication app(argc, argv);
    int result;
    try
    {
        std::cout << "hi" << std::endl;
        tester = new Tester(log);
        tester->show();
    }
    catch (std::bad_alloc&)
    {
        logMessage(log, LOG_ERROR, "meteo-point-tester", "Непоправимая ошибка выделения памяти");
        return 1;
    }
    catch (std::exception& e)
    {
        logFmtMessage(log, LOG_ERROR, "meteo-point-tester", "В ходе работы программы возникла неустранимая ошибка: %s", e.what());
        return 2;
    }
    result = app.exec();
    return result;
}

const char ** get_instance_id_array()
{
    static const char * id[] = { NULL, };
    return id;
}

#include "meteo-point-tester.moc"
