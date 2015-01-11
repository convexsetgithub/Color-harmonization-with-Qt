

#include "myvideosurface.h"
#include "myimage.h"
//MyVideoSurface::MyVideoSurface(QObject * parent=NULL) : QAbstractVideoSurface(parent)
//{}


bool MyVideoSurface::present(const QVideoFrame& frame){
    if (frame.isValid() ) {
        QVideoFrame cloneFrame(frame);
        cloneFrame.map(QAbstractVideoBuffer::ReadOnly);
        img = QImage(cloneFrame.bits(),
                     cloneFrame.width(),
                     cloneFrame.height(),
                     QVideoFrame::imageFormatFromPixelFormat(cloneFrame.pixelFormat()));

       // do something with the image ...
      //img = &img1;
        show->update();

        cloneFrame.unmap();
        return true;
    }
    return false;
}