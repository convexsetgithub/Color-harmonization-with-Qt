#include <QAbstractVideoSurface>
#include <QVideoFrame>
#include <QCamera>
#include <QVideoWidget>
#include <QVideoSurfaceFormat>
#ifndef B_H
#define B_H
class MyImage;
class MyVideoSurface: public QAbstractVideoSurface{
    Q_OBJECT
public:
    MyVideoSurface(QObject * parent=NULL) : QAbstractVideoSurface(parent)
    {}

    QList<QVideoFrame::PixelFormat>
    supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const{
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB24 ; // here return whatever formats you will handle
    }
    QImage img;
    MyImage * show;
    bool present(const QVideoFrame& frame);
    /*bool start(const QVideoSurfaceFormat & format){}
    void stop(){}
    QVideoSurfaceFormat nearestFormat(const QVideoSurfaceFormat & format) const{}*/
};
#endif
