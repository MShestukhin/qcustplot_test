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
    connect(ui->horizontalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(horzScrollBarChanged(int)));
    connect(ui->verticalScrollBar, SIGNAL(valueChanged(int)), this, SLOT(vertScrollBarChanged(int)));
    connect(ui->widget->xAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(xAxisChanged(QCPRange)));
    connect(ui->widget->yAxis, SIGNAL(rangeChanged(QCPRange)), this, SLOT(yAxisChanged(QCPRange)));
    ui->horizontalScrollBar->setVisible(false);
    ui->verticalScrollBar->setVisible(false);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_toolButton_clicked()
{
    QString file_name = QFileDialog::getOpenFileName(0, "Open Dialog", "", "*.csv");
    if(file_name.size()!=0){
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
                    ui->widget->graph(j-1)->setVisible(checkbox->isChecked());
                    ui->widget->replot();
                });
                ui->horizontalLayout_3->addWidget(checkbox);
                ui->widget->addGraph();
                if(j==1){
                    ui->widget->graph(j-1)->setVisible(true);
                    checkbox->setChecked(true);
                }
                else{
                    ui->widget->graph(j-1)->setVisible(false);
                }
                ui->widget->graph(j-1)->setData(y.at(0), y.at(j));
                if(j<11)
                    ui->widget->graph(j-1)->setPen(QPen(color_plot[j-1]));
            }
            double rang_max_x=*std::max_element(y.at(0).begin(),y.at(0).end());
            double rang_min_x=*std::min_element(y.at(0).begin(),y.at(0).end());
            if(j>0){
                double rang_max_y=*std::max_element(y.at(j).begin(),y.at(j).end());
                if(max_y<rang_max_y)
                    max_y=(double)rang_max_y;
                double rang_min_y=*std::min_element(y.at(j).begin(),y.at(j).end());
                if(min_y>rang_min_y)
                    min_y=(double)rang_min_y;
            }
            ui->widget->xAxis->setRange(rang_min_x, rang_max_x);
            ui->widget->yAxis->setRange(min_y, max_y);
            ui->horizontalScrollBar->setRange(100*rang_min_x, 100*rang_max_x);
            ui->verticalScrollBar->setRange(100*min_y, 100*max_y);
        }
        ui->horizontalScrollBar->setVisible(true);
        ui->verticalScrollBar->setVisible(true);
        ui->widget->xAxis->setLabel("t");
        ui->widget->yAxis->setLabel("data");
        ui->widget->replot();
        ui->widget->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    }
}

void Widget::horzScrollBarChanged(int value)
{
  if (qAbs(ui->widget->xAxis->range().center()-value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->widget->xAxis->setRange(value/100.0, ui->widget->xAxis->range().size(), Qt::AlignCenter);
    ui->widget->replot();
  }
}

void Widget::vertScrollBarChanged(int value)
{

  if (qAbs(ui->widget->yAxis->range().center()+value/100.0) > 0.01) // if user is dragging plot, we don't want to replot twice
  {
    ui->widget->yAxis->setRange(-value/100.0, ui->widget->yAxis->range().size(), Qt::AlignCenter);
    ui->widget->replot();
  }
}

void Widget::xAxisChanged(QCPRange range)
{
  ui->horizontalScrollBar->setValue(qRound(range.center()*100.0)); // adjust position of scroll bar slider
  ui->horizontalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

void Widget::yAxisChanged(QCPRange range)
{
  ui->verticalScrollBar->setValue(qRound(-range.center()*100)); // adjust position of scroll bar slider
  ui->verticalScrollBar->setPageStep(qRound(range.size()*100.0)); // adjust size of scroll bar slider
}

