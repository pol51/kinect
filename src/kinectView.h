#ifndef __KINECTVIEW_H__
#define __KINECTVIEW_H__

#include <QtOpenGL/QGLWidget>

#include <QtCore/QTimer>

#include <freenectDevice.h>

class KinectView : public QGLWidget
{
  Q_OBJECT

  public:
    KinectView(QWidget *parent = NULL);
    ~KinectView();

    QSize sizeHint() const;

    void setAngle(double angle) { _device->setTiltDegrees(angle); }

  protected:
    void initializeGL();
    void paintGL();
    void resizeGL(int width, int height);

  protected:
    GLuint _texDepth;
    FreenectDevice *_device;
    Freenect::Freenect _freenect;
    QTimer _refreshTimer;
};

#endif
