#include "myvideoprobe.h"
#include <QCamera>
#include <QVideoFrame>
#include <QVideoRendererControl>
MyVideoProbe::MyVideoProbe(QObject * parent)
    : QAbstractVideoSurface(parent) {

}

bool MyVideoProbe::setSource(QObject* sourceObj)
{
    m_source = sourceObj;
    bool ret = true;
    QCamera * player = qvariant_cast<QCamera*>(sourceObj->property("mediaObject"));
    qDebug() << "VideoProbe::setSource() player" << player << "sourceObj" << sourceObj;
    //QVideoRendererControl* rendererControl = player->service()->requestControl<QVideoRendererControl*>();
    //rendererControl->setSurface(this);
    //ret = QVideoProbe::setSource(player); -> windows always false
    //player->setViewfinder(this);
    return ret;
}

bool MyVideoProbe::isActive() {
    return QAbstractVideoSurface::isActive();
}

void MyVideoProbe::setActive(bool active) {
    m_active = active;
}

QObject* MyVideoProbe::source() {
    return m_source;
}

void MyVideoProbe::emitFrame(QVideoFrame videoFrame) {
    emit frame(videoFrame);
}
