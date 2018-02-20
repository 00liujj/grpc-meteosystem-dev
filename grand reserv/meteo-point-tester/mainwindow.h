#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QQuickView>
#include <string>

#include <Qt3DCore>
#include <Qt3DRenderer>
#include <Qt3DInput>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    float getLon();
    float getLat();
    std::string getParam();

    void setOutput(QString s);

private slots:
    virtual void on_pushButton_clicked(){}

private:
    Ui::MainWindow *ui;

};

#endif // MAINWINDOW_H
