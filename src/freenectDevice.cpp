#include "freenectDevice.h"

FreenectDevice::FreenectDevice(freenect_context *_ctx, int _index) :
  Freenect::FreenectDevice(_ctx, _index),
  _bufferDepth(FREENECT_FRAME_PIX<<2), _gamma(2048),
  _newDepthFrame(false), _reverseMode(false)
{
  for (int i = 2048; --i >= 0; )
    _gamma[i] = pow(i / 2048.f, 3) * 6 * (6<<8);
  memset(&_bufferDepth[0], 0xff, FREENECT_FRAME_PIX<<2);
}

void FreenectDevice::DepthCallback(void* _depth, uint32_t timestamp)
{
  Q_UNUSED(timestamp);

  QMutexLocker Locker(&_depthMutex);
  uint16_t* depth = static_cast<uint16_t*>(_depth);

  unsigned char CMin = 0xff;
  for( unsigned int i = 0 ; i < FREENECT_FRAME_PIX; ++i)
  {
    unsigned char C = (_gamma[depth[i]]>>8 <= 1?_gamma[depth[i]]>>1:0xff);

    if ((_gamma[depth[i]]>>7) < 2 && C < CMin && C > 0)
      CMin = C;
  }

  for( unsigned int i = 0 ; i < FREENECT_FRAME_PIX; ++i)
  {
    unsigned char C = (_gamma[depth[i]]>>8 <= 1?_gamma[depth[i]]>>1:0xff);

    if ((_gamma[depth[i]]>>7) >= 2)
    {
      if (_reverseMode) C ^= 0xff;
      memset(&_bufferDepth[i<<2], C, 3);
    }
    else
    {
      if (C-16 < CMin)
      {
        _bufferDepth[(i<<2) + 1] = 0;
        _bufferDepth[(i<<2) + 2] = 0xff;
      }
      else
        memset(&_bufferDepth[i<<2]+1, C, 2);
      _bufferDepth[i<<2] = 0xff;
    }
  }

  _newDepthFrame = true;
}

bool FreenectDevice::getDepth(QVector<uint8_t> &buffer)
{
  QMutexLocker Locker(&_depthMutex);
  if(_newDepthFrame)
  {
    buffer = _bufferDepth;
    _newDepthFrame = false;
    return true;
  }
  else
    return false;
}
