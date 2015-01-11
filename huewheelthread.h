#ifndef HUEWHEELTHREAD
#define HUEWHEELTHREAD
#include <Qthread>
#include "huewheel.h"
#include "myimage.h"
class HWThread : public QThread {
    Q_OBJECT
public:
    HWThread(HueWheel* HW) {
        m_HW = HW;
    }

protected:
    void run() {
        m_HW->updateWithFrame();
    }

private:
    HueWheel* m_HW;
};

class HWThreadTV : public QThread {
    Q_OBJECT
public:
    HWThreadTV(HueWheel* HW) {
        m_HW = HW;
    }

protected:
    void run() {
        m_HW->updateWithFrameTV();
    }

private:
    HueWheel* m_HW;
};

class MyImageTV : public QThread {
    Q_OBJECT
public:
    MyImageTV(MyImage* HW) {
        m_HW = HW;
    }

protected:
    void run() {
        m_HW->updateWithFrameTV();
    }

private:
    MyImage* m_HW;
};
#endif // HUEWHEELTHREAD

