#include <stdlib.h>
#include <stdio.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QDesktopWidget>
#include <QScreen>
#include <QMessageBox>
#include <QMetaEnum>
#include <iostream>
#include "Utility/ext.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    //setGeometry(400, 250, 742, 590);

    ui->stackedWidget->setCurrentIndex(0);

    //-- Initialize Data
    this->key = 0;
    //[Maximum Waveform chart History-Time-Length : defualt 30s]
    this->max_HTL = 30;
    this->Xrange = 10;
    ui->HTL->setText("10");

    //checkbox
    this->If_scaleI = 2;
    this->If_scaleQ = 2;
    this->If_scaleIQ = 2;
    ui->tab_2->setEnabled(false);

    //Setup Input Validator
    ui->HTL->setValidator(new QIntValidator (1, this->max_HTL, this));
    ui->IMax->setValidator(new QDoubleValidator (-3.3, 3.3, 5, this));
    ui->IMin->setValidator(new QDoubleValidator (-3.3, 3.3, 5, this));
    ui->QMax->setValidator(new QDoubleValidator (-3.3, 3.3, 5, this));
    ui->QMin->setValidator(new QDoubleValidator (-3.3, 3.3, 5, this));

    setupRealtimeDataDemo(ui->IQ_Plot , ui->Phase_Plot );

    setWindowTitle("Signal Monitor: ");
    statusBar()->clearMessage();

    ui->Phase_Plot->replot();

}

MainWindow::~MainWindow()
{
    delete ui;
}


//---Data Collector

void MainWindow::Dataslot(double I ,double Q ,double F_I ,double F_Q ,double key)
{

    // add data to lines:
    this->key = key;
    ui->Phase_Plot->graph(0)->addData(key, atan2(Q,I));
    ui->Phase_Plot->graph(1)->addData(key, atan2(F_Q,F_I));

    ui->IQ_Plot->graph(0)->addData(key, I);
    ui->IQ_Plot->graph(1)->addData(key, Q);
}

void MainWindow::setupRealtimeDataDemo(QCustomPlot *IQ_Plot , QCustomPlot *PhasePlot)
{


    // include this section to fully disable antialiasing for higher performance:
    PhasePlot->setNotAntialiasedElements(QCP::aeAll);
    QFont font;
    font.setStyleStrategy(QFont::NoAntialias);
    PhasePlot->xAxis->setTickLabelFont(font);
    PhasePlot->yAxis->setTickLabelFont(font);
    PhasePlot->legend->setFont(font);

    IQ_Plot->setNotAntialiasedElements(QCP::aeAll);
    QFont fontIQ;
    fontIQ.setStyleStrategy(QFont::NoAntialias);
    IQ_Plot->xAxis->setTickLabelFont(fontIQ);
    IQ_Plot->yAxis->setTickLabelFont(fontIQ);
    IQ_Plot->yAxis2->setTickLabelFont(fontIQ);
    IQ_Plot->legend->setFont(fontIQ);
    IQ_Plot->setNotAntialiasedElements(QCP::aeAll);

    //-- Set Time Ticker Form
    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%h:%m:%s");

    //-- Add Plot for Atan
    PhasePlot->addGraph();
    PhasePlot->graph(0)->setPen(QPen(QColor(40, 110, 255),2) );
    PhasePlot->addGraph(PhasePlot->xAxis,PhasePlot->yAxis2);
    PhasePlot->yAxis2->setVisible(true);
    PhasePlot->graph(1)->setPen(QPen(QColor(255, 110, 40),5) );
    PhasePlot->graph(0)->setName("Raw");
    PhasePlot->graph(1)->setName("Filtered");
    PhasePlot->legend->setVisible(true);
    PhasePlot->legend->setFont(QFont("Helvetica",9));
    PhasePlot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignTop);
    PhasePlot->plotLayout()->insertRow(0);
    PhasePlot->plotLayout()->addElement(0, 0, new QCPTextElement(PhasePlot, "Arctan Data Plot", QFont("sans", 12, QFont::Bold)));
    PhasePlot->xAxis->setTicker(timeTicker);
    //Comment next line if you want to display both Left & Right -Yaxis
    //    customPlot->axisRect()->setupFullAxesBox();

    //-- Add Plot for I & Q
    //-IQ Data plot
    IQ_Plot->addGraph();
    IQ_Plot->graph(0)->setPen(QPen(QColor(40, 110, 255),2) );
    IQ_Plot->addGraph(IQ_Plot->xAxis,IQ_Plot->yAxis2);//Use Right Y axis for Q Data
    IQ_Plot->yAxis2->setVisible(true);
    IQ_Plot->graph(1)->setPen(QPen(QColor(255, 110, 40),2) );
    IQ_Plot->graph(0)->setName("I Channel");
    IQ_Plot->graph(1)->setName("Q Channel");
    IQ_Plot->legend->setVisible(true);
    IQ_Plot->legend->setFont(QFont("Helvetica",9));
    IQ_Plot->axisRect()->insetLayout()->setInsetAlignment(0,Qt::AlignLeft|Qt::AlignTop);
    IQ_Plot->plotLayout()->insertRow(0);
    IQ_Plot->plotLayout()->addElement(0, 0, new QCPTextElement(ui->IQ_Plot, "Raw Data Plot", QFont("sans", 12, QFont::Bold)));
    IQ_Plot->xAxis->setTicker(timeTicker);
    //Comment next line if you want to display both Left & Right -Yaxis
    //    ui->Iplot->axisRect()->setupFullAxesBox();


    // make left and bottom axes transfer their ranges to right and top axes:
    connect(PhasePlot->xAxis, SIGNAL(rangeChanged(QCPRange)), PhasePlot->xAxis2, SLOT(setRange(QCPRange)));
    connect(PhasePlot->yAxis, SIGNAL(rangeChanged(QCPRange)), PhasePlot->yAxis2, SLOT(setRange(QCPRange)));
    //    connect(ui->IQ->xAxis, SIGNAL(rangeChanged(QCPRange)), ui->IQ->xAxis2, SLOT(setRange(QCPRange)));
    //    connect(ui->IQ->yAxis, SIGNAL(rangeChanged(QCPRange)), ui->IQ->yAxis2, SLOT(setRange(QCPRange)));

    // setup a timer that repeatedly calls MainWindow::realtimeDataSlot:
    connect(&dataTimer, SIGNAL(timeout()), this, SLOT(realtimeDataSlot()));
    if(GUI_Switch!=0)
        dataTimer.start(10); // Interval 0 means to refresh as fast as possible
}


//---View
void MainWindow::realtimeDataSlot()
{
    //Display Start
    static double range = 0;
    static double YM,Ym;
    static double lastPointKey = 0;

    if (key-lastPointKey > 0.010) // at most add point every 10 ms
    {
        //Delete Data out of Windows
        lastPointKey = key;
        int keep_Length = (Xrange > this->max_HTL)?Xrange:this->max_HTL;
        ui->Phase_Plot->graph(0)->data().data()->removeBefore(key-keep_Length);
        ui->Phase_Plot->graph(1)->data().data()->removeBefore(key-keep_Length);
        ui->IQ_Plot->graph(0)->data().data()->removeBefore(key-keep_Length);
        ui->IQ_Plot->graph(1)->data().data()->removeBefore(key-keep_Length);
    }
    // make key axis range scroll with the data (at a constant range size of the specific time space):
    ui->Phase_Plot->xAxis->setRange(key, Xrange, Qt::AlignRight);//-- 3 secs here
    ui->IQ_Plot->xAxis->setRange(key, Xrange, Qt::AlignRight);


    //-- Make sure scope scales to current data
    //-- Padding Value Axis for better FPS
    if(this->If_scaleIQ == 2){

        ui->Phase_Plot->graph(0)->rescaleValueAxis(false, true);
        range = ui->Phase_Plot->yAxis->range().upper - ui->Phase_Plot->yAxis->range().center();

        YM = ui->Phase_Plot->yAxis->range().center()+range*1.5;
        Ym = ui->Phase_Plot->yAxis->range().center()-range*1.5;
        ui->Phase_Plot->yAxis->setRange(YM,Ym);


        ui->Phase_Plot->graph(1)->rescaleValueAxis(false, true);
        range = ui->Phase_Plot->yAxis2->range().upper - ui->Phase_Plot->yAxis2->range().center();

        YM = ui->Phase_Plot->yAxis2->range().center()+range*1.5;
        Ym = ui->Phase_Plot->yAxis2->range().center()-range*1.5;
        ui->Phase_Plot->yAxis2->setRange(YM,Ym);


    }
    //-I
    if(this->If_scaleI == 2){
        ui->IQ_Plot->graph(0)->rescaleValueAxis(false, true);
        range = ui->IQ_Plot->yAxis->range().upper - ui->IQ_Plot->yAxis->range().center();
        YM = ui->IQ_Plot->yAxis->range().center()+range*1.5;
        this->IMax = YM;
        Ym = ui->IQ_Plot->yAxis->range().center()-range*1.5;
        this->Imin = Ym;
        ui->IQ_Plot->yAxis->setRange(YM,Ym);
    }else{
        ui->IQ_Plot->yAxis->setRange(this->IMax,this->Imin);
    }
    //-Q
    if(this->If_scaleQ == 2){
        ui->IQ_Plot->graph(1)->rescaleValueAxis(false, true);
        range = ui->IQ_Plot->yAxis2->range().upper - ui->IQ_Plot->yAxis2->range().center();
        YM = ui->IQ_Plot->yAxis2->range().center()+range*1.5;
        this->QMax = YM;
        Ym = ui->IQ_Plot->yAxis2->range().center()-range*1.5;
        this->Qmin = Ym;
        ui->IQ_Plot->yAxis2->setRange(YM,Ym);
    }else{
        ui->IQ_Plot->yAxis2->setRange(this->QMax,this->Qmin);
    }

    //-- Update Grapgh Plot
    ui->Phase_Plot->replot();
    ui->IQ_Plot->replot();

    //-- calculate frames per second & Refresh status bar & MQTT
    static double lastFpsKey;
    static int frameCount;
    ++frameCount;
    if (key-lastFpsKey > 2) // average fps over 2 seconds
    {
        ui->statusBar->showMessage(
                    QString("%1 FPS, Total Data points: %2")
                    .arg(frameCount/(key-lastFpsKey), 0, 'f', 0)
                    .arg(ui->Phase_Plot->graph(0)->data()->size()+ui->Phase_Plot->graph(1)->data()->size())
                    , 0);

        lastFpsKey = key;
        frameCount = 0;
    }
}

/*--------------------(Input) Slot & Signal-----------------*/

void MainWindow::on_HTL_textChanged(const QString &arg1)
{
    int TL = ui->HTL->text().toInt();
    if(TL > 0)
        this->Xrange = TL;
    else
        return;
}

void MainWindow::on_cbox_I_stateChanged(int arg1)
{
    this->If_scaleI = arg1;
}

void MainWindow::on_cbox_Q_stateChanged(int arg1)
{
    this->If_scaleQ = arg1;
}

void MainWindow::on_cbox_IQ_stateChanged(int arg1)
{
    this->If_scaleIQ = arg1;
}

void MainWindow::on_IMax_textChanged(const QString &arg1)
{
    ui->cbox_I->setChecked(false);
    double val = ui->IMax->text().toDouble();
    this->IMax = val;
}

void MainWindow::on_IMin_textChanged(const QString &arg1)
{
    ui->cbox_I->setChecked(false);
    double val = ui->IMin->text().toDouble();
    this->Imin = val;
}

void MainWindow::on_QMax_textChanged(const QString &arg1)
{
    ui->cbox_Q->setChecked(false);
    double val = ui->QMax->text().toDouble();
    this->QMax = val;
}

void MainWindow::on_QMin_textChanged(const QString &arg1)
{
    ui->cbox_Q->setChecked(false);
    double val = ui->QMin->text().toDouble();
    this->Qmin = val;
}












