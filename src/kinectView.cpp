#include "kinectView.h"

KinectView::KinectView(QWidget *parent) :
  QGLWidget(QGLFormat(QGL::SampleBuffers | QGL::DoubleBuffer), parent),
  _texDepth(0), _device(NULL)
{
  _device = &_freenect.createDevice<FreenectDevice>(0);
  _device->setTiltDegrees(10);
  _device->startDepth();

  setAttribute(Qt::WA_NoSystemBackground);
  setMinimumSize(640, 480);
  setMouseTracking(true);
  setFocus();
  grabKeyboard();

  _refreshTimer.start(10);
  connect(&_refreshTimer, SIGNAL(timeout()), this, SLOT(repaint()));
}

KinectView::~KinectView()
{
  _device->stopDepth();
  _device->setLed(LED_OFF);
  _device->setTiltDegrees(0);
  _freenect.deleteDevice(0);
}

QSize KinectView::sizeHint() const
{
  return QSize(640, 480);
}

void KinectView::initializeGL()
{
  glClearColor(0.f, 0.f, 0.f, 0.f);
  glClearDepth(1.f);
  glDepthFunc(GL_LESS);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glShadeModel(GL_SMOOTH);
  glGenTextures(1, &_texDepth);
  glBindTexture(GL_TEXTURE_2D, _texDepth);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.f, 640.f, 480.f, 0.f, 0.f, 1.f);
  glMatrixMode(GL_MODELVIEW);
}

void KinectView::paintGL()
{
  static QVector<uint8_t> depth(640*480*4);

  _device->getDepth(depth);

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);

  glBindTexture(GL_TEXTURE_2D, _texDepth);
  glTexImage2D(GL_TEXTURE_2D, 0, 4, 640, 480, 0, GL_RGB, GL_UNSIGNED_BYTE, &depth[0]);

  glBegin(GL_TRIANGLE_FAN);
  glColor4f(255.0f, 255.0f, 255.0f, 255.0f);
  glTexCoord2f(1, 0); glVertex3f(0,0,0);
  glTexCoord2f(0, 0); glVertex3f(640,0,0);
  glTexCoord2f(0, 1); glVertex3f(640,480,0);
  glTexCoord2f(1, 1); glVertex3f(0,480,0);
  glEnd();
}

void KinectView::resizeGL(int width, int height)
{
  glViewport(0, 0, width, height);

  float x = 640;
  float y = 480;

  if (x * 480 > y * 640)
    x = y * 640 / 480;
  else
    y = x * 480 / 640;

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.f, +x, +y, 0.f, 0.f, 1.f);
  glMatrixMode(GL_MODELVIEW);
}
