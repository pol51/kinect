#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtGui/QMainWindow>
#include <QtGui/QSlider>

#include <kinectView.h>

class MainWindow : public QMainWindow
{
  Q_OBJECT

  public:
    MainWindow(QWidget *parent = NULL);

  protected slots:
    void onAngle(int val);

  protected:
    QWidget *mWidget;
    KinectView *view;
    QSlider *angle;
};

#endif
