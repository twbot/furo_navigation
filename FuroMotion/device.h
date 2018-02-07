#ifndef DEVICE
#define DEVICE
class Device
{
public:
    virtual bool Init();
    virtual bool Release() = 0;

};
#endif // DEVICE

