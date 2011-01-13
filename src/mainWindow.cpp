#include "mainWindow.h"

#include <QHBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent)
{
  mWidget = new QWidget();
  setCentralWidget(mWidget);

  angle = new QSlider(Qt::Vertical, this);
  angle->setMinimum(-45);
  angle->setMaximum(45);

  view = new KinectView(this);

  QHBoxLayout *MainLayout = new QHBoxLayout(mWidget);
  MainLayout->addWidget(view);
  MainLayout->addWidget(angle);

  connect(angle, SIGNAL(valueChanged(int)), this, SLOT(onAngle(int)));

  angle->setValue(10);
}

void MainWindow::onAngle(int val)
{
  view->setAngle(val);
}
