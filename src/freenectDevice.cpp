#include "freenectDevice.h"

FreenectDevice::FreenectDevice(freenect_context *_ctx, int _index, QObject *parent) :
  Freenect::FreenectDevice(_ctx, _index),
  QObject(parent),
  _bufferDepth(FREENECT_VIDEO_RGB_SIZE), _gamma(2048),
  _newDepthFrame(false), _reverseMode(false)
{

  for (int i = 2048; --i >= 0; )
  {
    float v = i / 2048.0;
    v = pow(v, 3)* 6;
    _gamma[i] = v * 6 * 256;
  }
}

void FreenectDevice::DepthCallback(void* _depth, uint32_t timestamp)
{
  Q_UNUSED(timestamp);

  _depthMutex.lock();
  uint16_t* depth = static_cast<uint16_t*>(_depth);
  if (_reverseMode)
    for( unsigned int i = 0 ; i < FREENECT_FRAME_PIX; ++i)
      memset(&_bufferDepth[3*i], 0xff - (_gamma[depth[i]]>>8 <= 1?_gamma[depth[i]]>>1:0xff), 3);
  else
    for( unsigned int i = 0 ; i < FREENECT_FRAME_PIX; ++i)
      memset(&_bufferDepth[3*i], (_gamma[depth[i]]>>8 <= 1?_gamma[depth[i]]>>1:0xff), 3);

  _newDepthFrame = true;
  _depthMutex.unlock();
}

bool FreenectDevice::getDepth(QVector<uint8_t> &buffer)
{
  _depthMutex.lock();
  if(_newDepthFrame)
  {
    buffer = _bufferDepth;
    _newDepthFrame = false;
    _depthMutex.unlock();
    return true;
  }
  else
  {
    _depthMutex.unlock();
    return false;
  }
}
