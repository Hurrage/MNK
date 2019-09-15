#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTableWidget>
#include <QPushButton>
#include <QCheckBox>
#include <QGuiApplication>
#include <QDesktopWidget>
#include <QLabel>
#include <iostream>
#include <QDebug>
#include <QLineEdit>
#include "qcustomplot.h"

class Widget : public QWidget
{
    Q_OBJECT
private slots:
    void addButtonClick();
    void delButtonClick();
    void printButtonClick();
    void FindYClick();
private:
    std::vector<double> NMK1(std::vector<double> _Xi, std::vector<double> _Yi);
    std::vector<double> NMK2(std::vector<double> _Xi, std::vector<double> _Yi);
    void plotGraph(int);
    void plotGraph(double, double, int);
    void plotGraph(double, double, double, int);
public:
    QTableWidget *table;
    QCustomPlot *plot;
    QCheckBox *nmk1CheckBox;
    QCheckBox *nmk2CheckBox;
    QLineEdit *findByX;
    QLineEdit *findByY1;
    QLineEdit *findByY2;
    double minX, maxX, minY, maxY;
    Widget(QWidget *parent = nullptr);
    ~Widget();
};
#endif // WIDGET_H
