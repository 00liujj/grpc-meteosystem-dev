#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QQuickView *view = new QQuickView();
    QWidget *container = QWidget::createWindowContainer(view, this);
    container->setFocusPolicy(Qt::TabFocus);
    view->setSource(QUrl("qrc:/thinned-grid.qml"));
    ui->verticalLayout_6->addWidget(container);
}

MainWindow::~MainWindow()
{
    delete ui;
}

float MainWindow::getLon()
{
    return ui->lineEdit->text().toFloat();
}

float MainWindow::getLat()
{
    return ui->lineEdit_2->text().toFloat();
}

std::string MainWindow::getParam()
{
    return ui->comboBox_2->currentText().toStdString();
}

void MainWindow::setOutput(QString s)
{
    ui->textBrowser->insertPlainText(s);
}
