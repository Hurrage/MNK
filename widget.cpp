#include "widget.h"
using namespace std;
Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(0, 0, QGuiApplication::screens().first()->size().width()-200, QGuiApplication::screens().first()->size().height()-200);
    table = new QTableWidget(this);
    table->setGeometry(this->geometry());
    table->show();
    table->setRowCount(1);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels(QStringList({"x", "y"}));

    QPushButton *addButton = new QPushButton("Додати точку", this);
    addButton->setGeometry(250, 10, 150, 20);
    addButton->show();
    connect(addButton, SIGNAL (released()), this, SLOT (addButtonClick()));

    QPushButton *delButton = new QPushButton("Видалити виділену точку", this);
    delButton->setGeometry(250, 40, 150, 20);
    delButton->show();
    connect(delButton, SIGNAL (released()), this, SLOT (delButtonClick()));

    nmk1CheckBox = new QCheckBox("Побудувати МНК1", this);
    nmk1CheckBox->move(250, 70);
    nmk1CheckBox->show();
    nmk1CheckBox->setChecked(true);

    nmk2CheckBox = new QCheckBox("Побудувати МНК2", this);
    nmk2CheckBox->move(250, 90);
    nmk2CheckBox->show();
    nmk2CheckBox->setChecked(true);

    QPushButton *printButton = new QPushButton("Показати графік", this);
    printButton->setGeometry(250, 110, 150, 20);
    printButton->show();
    connect(printButton, SIGNAL (released()), this, SLOT (printButtonClick()));

    QLabel *fX = new QLabel("X", this);
    fX->setGeometry(252, 140, 50, 20);
    fX->show();

    findByX = new QLineEdit(this);
    findByX->setGeometry(275, 140, 124, 20);
    findByX->show();

    QLabel *fXY1 = new QLabel("Y1", this);
    fXY1->setGeometry(252, 170, 50, 20);
    fXY1->show();

    findByY1 = new QLineEdit(this);
    findByY1->setGeometry(275, 170, 124, 20);
    findByY1->show();
    findByY1->setReadOnly(true);

    QLabel *fXY2 = new QLabel("Y2", this);
    fXY2->setGeometry(252, 200, 50, 20);
    fXY2->show();

    findByY2 = new QLineEdit(this);
    findByY2->setGeometry(275, 200, 124, 20);
    findByY2->show();
    findByY2->setReadOnly(true);

    QPushButton *findYButton = new QPushButton("Знайти Y", this);
    findYButton->setGeometry(250, 230, 150, 20);
    findYButton->show();
    connect(findYButton, SIGNAL (released()), this, SLOT (FindYClick()));


    plot = new QCustomPlot(this);
    plot->setGeometry(450, 10, QGuiApplication::screens().first()->size().width()-651, QGuiApplication::screens().first()->size().height()-211);
    plot->setInteractions(plot->interactions() | QCP::iRangeDrag);
    plot->setInteractions(plot->interactions() | QCP::iRangeZoom);
}

Widget::~Widget()
{
}

void Widget::addButtonClick(){
    table->setRowCount(table->rowCount()+1);
}

void Widget::delButtonClick(){
    if(table->selectedItems().length() == 1){
        if(table->selectedItems()[0]->text() != ""){
            table->removeRow(table->selectedItems()[0]->row());
        }
    }
}

void Widget::printButtonClick(){
    plot->clearGraphs();
    vector<double>X, Y;
    minX = table->item(0, 0)->text().replace(",", ".").toDouble();
    maxX = minX;
    minY = table->item(0, 1)->text().replace(",", ".").toDouble();
    maxY = minY;
    for(int i = 0; i < table->rowCount(); i++){
        if(table->item(i, 0)->text() != "" && table->item(i, 1)->text() != ""){
            X.push_back(table->item(i, 0)->text().replace(",", ".").toDouble());
            Y.push_back(table->item(i, 1)->text().replace(",", ".").toDouble());
            if(X[i] < minX) minX = X[i];
            if(X[i] > maxX) maxX = X[i];
            if(Y[i] < minY) minY = Y[i];
            if(Y[i] > maxY) maxY = Y[i];
        }
    }

    plot->xAxis->setRange(minX-0.1, maxX+0.1);
    plot->yAxis->setRange(minY-0.1, maxY+0.1);

    int graphs = 0;
    if(nmk1CheckBox->isChecked() == true){
        vector<double> res = Widget::NMK1(X, Y);
        plotGraph(res[0], res[1], graphs);
        graphs++;
    }

    if(nmk2CheckBox->isChecked() == true){
        vector<double> res = Widget::NMK2(X, Y);
        plotGraph(res[0], res[1], res[2], graphs);
        graphs++;
    }
    plotGraph(graphs);
    if(plot->graphCount() > 1) plot->legend->setVisible(true);
    else plot->legend->setVisible(false);
    plot->replot();

}

std::vector<double> Widget::NMK1(std::vector<double> _Xi, std::vector<double> _Yi){
    double sum_x = 0, sum_y = 0, sum_xy = 0, sum_xx = 0;
    for (unsigned long long i = 0; i < _Xi.size(); i++) {
        sum_x += _Xi[i];
        sum_y += _Yi[i];
        sum_xy += _Xi[i]*_Yi[i];
        sum_xx += _Xi[i]*_Xi[i];
    }
    double x2 = (sum_y*sum_x-_Xi.size()*sum_xy)/(sum_x*sum_x-_Xi.size()*sum_xx);
    double x1 = (sum_y-sum_x*x2)/_Xi.size();
    vector<double> res;
    res.push_back(x2);
    res.push_back(x1);
    return res;
}

std::vector<double> Widget::NMK2(std::vector<double> _Xi, std::vector<double> _Yi){
    double sum_x = 0, sum_y = 0, sum_xx = 0, sum_xxx = 0, sum_xxxx = 0, sum_xy = 0, sum_xxy = 0;
    for (unsigned long long i = 0; i < _Xi.size(); i++) {
        sum_x += _Xi[i];
        sum_y += _Yi[i];
        sum_xx += _Xi[i]*_Xi[i];
        sum_xxx += _Xi[i]*_Xi[i]*_Xi[i];
        sum_xxxx += _Xi[i]*_Xi[i]*_Xi[i]*_Xi[i];
        sum_xy += _Xi[i]*_Yi[i];
        sum_xxy += _Xi[i]*_Xi[i]*_Yi[i];

    }
    vector<vector<double>> A;
    vector<double> temp;

    temp.push_back(_Xi.size());
    temp.push_back(sum_x);
    temp.push_back(sum_xx);
    A.push_back(temp);
    temp.clear();

    temp.push_back(sum_x);
    temp.push_back(sum_xx);
    temp.push_back(sum_xxx);
    A.push_back(temp);
    temp.clear();

    temp.push_back(sum_xx);
    temp.push_back(sum_xxx);
    temp.push_back(sum_xxxx);
    A.push_back(temp);
    temp.clear();

    vector<double> B;
    B.push_back(sum_y);
    B.push_back(sum_xy);
    B.push_back(sum_xxy);

    double detA = (A[0][0]*A[1][1]*A[2][2]+A[0][2]*A[1][0]*A[2][1]+A[0][1]*A[1][2]*A[2][0])-(A[0][2]*A[1][1]*A[2][0]+A[0][0]*A[1][2]*A[2][1]+A[0][1]*A[1][0]*A[2][2]);
    double detA1 = (B[0]*A[1][1]*A[2][2]+A[0][2]*B[1]*A[2][1]+A[0][1]*A[1][2]*B[2])-(A[0][2]*A[1][1]*B[2]+B[0]*A[1][2]*A[2][1]+A[0][1]*B[1]*A[2][2]);
    double detA2 = (A[0][0]*B[1]*A[2][2]+A[0][2]*A[1][0]*B[2]+B[0]*A[1][2]*A[2][0])-(A[0][2]*B[1]*A[2][0]+A[0][0]*A[1][2]*B[2]+B[0]*A[1][0]*A[2][2]);
    double detA3 = (A[0][0]*A[1][1]*B[2]+B[0]*A[1][0]*A[2][1]+A[0][1]*B[1]*A[2][0])-(B[0]*A[1][1]*A[2][0]+A[0][0]*B[1]*A[2][1]+A[0][1]*A[1][0]*B[2]);
    double A1, A2, A3;
    A1 = detA1/detA;
    A2 = detA2/detA;
    A3 = detA3/detA;
    vector<double> res;
    res.push_back(A3);
    res.push_back(A2);
    res.push_back(A1);

    return res;
}

void Widget::plotGraph(int gr){
    plot->addGraph();
    QVector<double>X, Y;
    for(int i = 0; i < table->rowCount(); i++){
        if(table->item(i, 0)->text() != "" && table->item(i, 1)->text() != ""){
            X.push_back(table->item(i, 0)->text().replace(",", ".").toDouble());
            Y.push_back(table->item(i, 1)->text().replace(",", ".").toDouble());
        }
    }
    plot->graph(gr)->setData(X, Y);
    plot->graph(gr)->setAdaptiveSampling(false);
    plot->graph(gr)->removeFromLegend();
    plot->graph(gr)->setLineStyle(QCPGraph::lsNone);
    plot->graph(gr)->setScatterStyle(QCPScatterStyle::ssCircle);
    plot->graph(gr)->setPen(QPen(QColor("Dark violet"),2));
}

void Widget::plotGraph(double A0, double A1, int gr){
    QVector<double> Xi, Yi;
    Xi.push_back(minX);
    Yi.push_back(minX*A0+A1);
    Xi.push_back(maxX);
    Yi.push_back(maxX*A0+A1);
    plot->addGraph();
    plot->graph(gr)->setData(Xi, Yi);
    plot->graph(gr)->setName("y = "+QString::number(A0)+"x"+QString("%1%2").arg(A1 < 0 ? " - " : " + ").arg(A1));
}

void Widget::plotGraph(double A0, double A1, double A2, int gr){
    QVector<double> Xi, Yi;
    for (double i = minX; i < maxX; i = i+0.01) {
        Xi.push_back(i);
        Yi.push_back(i*i*A0+A1*i+A2);
    }
    plot->addGraph();
    plot->graph(gr)->setData(Xi, Yi);
    plot->graph(gr)->setPen(QPen(QColor("red")));
    plot->graph(gr)->setName("y = "+QString::number(A0)+"x^2"+QString("%1%2").arg(A1 < 0 ? " - " : " + ").arg(A1)+"x"+QString("%1%2").arg(A2 < 0 ? '-' : '+').arg(A2));

}

void Widget::FindYClick(){
    vector<double>X, Y;
    for(int i = 0; i < table->rowCount(); i++){
        if(table->item(i, 0)->text() != "" && table->item(i, 1)->text() != ""){
            X.push_back(table->item(i, 0)->text().replace(",", ".").toDouble());
            Y.push_back(table->item(i, 1)->text().replace(",", ".").toDouble());
        }
    }
    vector<double> A1;
    vector<double> A2;
    A1 = NMK1(X, Y);
    A2 = NMK2(X, Y);
    double x = findByX->text().replace(",", ".").toDouble();
    findByY1->setText(QString::number(x*A1[0]+A1[1]));
    findByY2->setText(QString::number(x*x*A2[0]+A2[1]*x+A2[2]));
}
