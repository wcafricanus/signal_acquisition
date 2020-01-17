#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "qcustomplot.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setupRealtimeDataDemo(QCustomPlot *customPlot ,QCustomPlot *histPlot);


signals:
    void valueChanged(double);

private slots:
  void realtimeDataSlot();
  void Dataslot(double ,double ,double ,double ,double);
  void on_HTL_textChanged(const QString &arg1);
  void on_cbox_I_stateChanged(int arg1);
  void on_cbox_Q_stateChanged(int arg1);
  void on_cbox_IQ_stateChanged(int arg1);
  void on_IMax_textChanged(const QString &arg1);
  void on_IMin_textChanged(const QString &arg1);
  void on_QMax_textChanged(const QString &arg1);
  void on_QMin_textChanged(const QString &arg1);




private:
    Ui::MainWindow *ui;
    QTimer dataTimer;

    double Iin , Qin ,key;
    int Xrange ,max_HTL;
    int If_scaleI,If_scaleQ,If_scaleIQ;
    double IMax,Imin,QMax,Qmin;

};

#endif // MAINWINDOW_H
