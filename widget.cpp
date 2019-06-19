#include "widget.h"
#include "ui_widget.h"
#include "parser.h"
#include <QDebug>
#include <iostream>
#include <string>
#include <vector>
Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_toolButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.csv");
    ui->label->setText(file_name);
    parser pars;
    vector<vector<std::string>> mass=pars.pars_file(file_name.toStdString().c_str(),4,2);
    int data_n=mass.size();
    QVector<double> x(data_n), y1(data_n), y2(data_n), y3(data_n);
    for (int i=0; i<data_n; ++i)
    {
      QString str_x=mass.at(i).at(0).c_str();
      QString str_y1=mass.at(i).at(1).c_str();
      QString str_y2=mass.at(i).at(2).c_str();
      QString str_y3=mass.at(i).at(3).c_str();
      x[i] = str_x.toDouble();
      y1[i] = str_y1.toDouble();
      y2[i] = str_y2.toDouble();
      y3[i] = str_y3.toDouble();
    }

    double rang_max_x=*std::max_element(x.begin(),x.end());
    double rang_min_x=*std::min_element(x.begin(),x.end());
    double rang_max_y=*std::max_element(y1.begin(),y1.end());
    double rang_min_y=*std::min_element(y1.begin(),y1.end());

    ui->widget->addGraph();
    ui->widget->graph(0)->setData(x, y1);
    ui->widget->addGraph();
    ui->widget->graph(1)->setPen(QPen(Qt::red));
    ui->widget->graph(1)->setData(x, y2);
    ui->widget->addGraph();
    ui->widget->graph(2)->setPen(QPen(Qt::green));
    ui->widget->graph(2)->setData(x, y3);
    ui->widget->xAxis->setLabel("t");
    ui->widget->yAxis->setLabel("data");
    ui->widget->xAxis->setRange(rang_min_x, rang_max_x);
    ui->widget->yAxis->setRange(rang_min_y, rang_max_y);
    ui->widget->replot();
}
