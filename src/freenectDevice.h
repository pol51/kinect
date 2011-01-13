#include <QtCore/QVector>
#include <QtCore/QMutex>

#include <cmath>

#include <libfreenect.hpp>

class FreenectDevice : public Freenect::FreenectDevice
{
  public:
    FreenectDevice(freenect_context *_ctx, int _index);
    virtual ~FreenectDevice() {}

    virtual void DepthCallback(void* _depth, uint32_t timestamp);
    virtual void VideoCallback(void*, uint32_t) {}

    bool getDepth(QVector<uint8_t> &buffer);

  protected:
    QVector<uint8_t>    _bufferDepth;
    QVector<uint16_t>   _gamma;
    QMutex              _depthMutex;
    bool                _newDepthFrame;
    bool                _reverseMode;
};
