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
    vector<vector<std::string>> mass=pars.pars_file(file_name.toStdString().c_str());
    int data_n=mass.size();
    QVector<QVector<double>> y;
    double min_y=0;
    double max_y=0;
    for(int j=0; j<mass.at(0).size();j++){
         QVector<double> x(data_n);
        for (int i=0; i< data_n; i++)
        {
            QString str_x=mass.at(i).at(j).c_str();
            x[i] = str_x.toDouble();
        }
        y.push_back(x);
        if(j>0){
            QCheckBox* checkbox=new QCheckBox;
            checkbox->setText(mass.at(0).at(j).c_str());
            QObject::connect(checkbox, &QCheckBox::stateChanged,[=](){
                qDebug()<<checkbox->isChecked();
                ui->widget->graph(j-1)->setVisible(checkbox->isChecked());
                    ui->widget->replot();
            });
            ui->horizontalLayout_3->addWidget(checkbox);
            ui->widget->addGraph();
            ui->widget->graph(j-1)->setData(y.at(0), y.at(j));
            ui->widget->graph(j-1)->setVisible(false);
            if(j==1){
                ui->widget->graph(j-1)->setVisible(false);
                checkbox->setChecked(true);
            }
        }
        double rang_max_x=*std::max_element(y.at(0).begin(),y.at(0).end());
        double rang_min_x=*std::min_element(y.at(0).begin(),y.at(0).end());
        double rang_max_y=*std::max_element(y.at(j).begin(),y.at(j).end());
        if(max_y>rang_max_y)
            max_y=(double)rang_max_y;
        double rang_min_y=*std::min_element(y.at(j).begin(),y.at(j).end());
        if(min_y<rang_min_y)
            min_y=(double)rang_min_y;
        double min=(double)rang_min_y;
        double max=30.0;
        ui->widget->xAxis->setRange(rang_min_x, rang_max_x);
        ui->widget->yAxis->setRange(-30, 30);
    }
    ui->widget->xAxis->setLabel("t");
    ui->widget->yAxis->setLabel("data");
    ui->widget->replot();
    ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}
