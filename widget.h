#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "qcustomplot.h"

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    Qt::GlobalColor color_plot[11]={Qt::red, Qt::green, Qt::black,Qt::darkGray, Qt::darkGreen, Qt::gray,Qt::darkBlue, Qt::darkCyan, Qt::darkYellow, Qt::darkMagenta, Qt::darkRed};
    ~Widget();

private slots:
    void on_toolButton_clicked();
    void horzScrollBarChanged(int);
    void vertScrollBarChanged(int);
    void xAxisChanged(QCPRange);
    void yAxisChanged(QCPRange);

private:
    Ui::Widget *ui;
};

#endif // WIDGET_H
