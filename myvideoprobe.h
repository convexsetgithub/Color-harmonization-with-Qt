#ifndef MYVIDEOPROBE
#define MYVIDEOPROBE
#include <QDebug>
#include <QVideoFrame>
#include <QMediaPlayer>
#include <QVideoProbe>
#include <QAbstractVideoSurface>
#include <QImage>
class MyVideoProbe : public QAbstractVideoSurface
{
    Q_OBJECT
    QObject * m_source;
    Q_PROPERTY(QObject* source READ source WRITE setSource)
    bool m_active;
    Q_PROPERTY(bool active READ isActive WRITE setActive)
    QImage m_image;
public:
    MyVideoProbe(QObject *parent = NULL);
    QList<QVideoFrame::PixelFormat>
    supportedPixelFormats(QAbstractVideoBuffer::HandleType type) const{
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB24; // here return whatever formats you will handle
    }
    bool present(const QVideoFrame& frame){
        if (frame.isValid()) {
            QVideoFrame cloneFrame(frame);
            cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
            m_image = QImage(cloneFrame.bits(),
                             cloneFrame.width(),
                             cloneFrame.height(),
                             QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));



            cloneFrame.unmap();
            return true;
        }
        return false;
    }
    bool isActive();
    void setActive(bool);
    QObject* source();
    bool setSource(QObject *);

signals:
    void frame(QVideoFrame videoFrame);

public slots:
    void emitFrame(QVideoFrame videoFrame);

};
#endif // MYVIDEOPROBE

