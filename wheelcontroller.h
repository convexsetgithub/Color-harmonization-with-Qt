#ifndef WHEELCONTROLLER_H
#define WHEELCONTROLLER_H
#include <QQuickImageProvider>
#include <QImage>
#include <QPainter>
#include <QStyleOption>
class WheelController : public QQuickImageProvider {
private:
    int margin;
    int wheelWidth;
    QImage input;
    QImage output;
public:

    WheelController() : QQuickImageProvider(QQuickImageProvider::Image) {
        margin = 0;
        wheelWidth = 30;
    }

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize) {
        //int width = 100;
        //int height = 50;

        //if (size)
        //    *size = QSize(width, height);
        QImage image;
        if (id == "input") {
            image = QImage("C://Users/user/Pictures/Cube.jpg");
            image = fit500(&image);
        }
        else if (id == "output") {
            image = QImage("C://Users/user/Pictures/Cube.jpg");
            image = fit500(&image);
        }

        if (id == "left" || id == "right") {
            image = QImage(255, 255, QImage::Format_ARGB32_Premultiplied);
            image.fill(QColor("white").rgba());
            drawWheel (&image);
        }
        return image;
    }

    QImage fit500(QImage * image) {
        if (image->width() > image->height() && image->width() > 500)
            return image->scaledToWidth(500);
        else if (image->height() > image->width() && image->height() > 500)
            return image->scaledToHeight(500);
        else
            return *image;
    }

    void drawWheel (QImage * wheelImage) {

        QBrush background = QBrush(QColor("white"));
        int r = qMin(wheelImage->width(), wheelImage->height());

        QPainter painter(wheelImage);
        painter.setRenderHint(QPainter::Antialiasing);

        QConicalGradient conicalGradient(0, 0, 0);
        conicalGradient.setColorAt(0.0, Qt::red);
        conicalGradient.setColorAt(60.0/360.0, Qt::yellow);
        conicalGradient.setColorAt(120.0/360.0, Qt::green);
        conicalGradient.setColorAt(180.0/360.0, Qt::cyan);
        conicalGradient.setColorAt(240.0/360.0, Qt::blue);
        conicalGradient.setColorAt(300.0/360.0, Qt::magenta);
        conicalGradient.setColorAt(1.0, Qt::red);

        /* outer circle */
        painter.translate(r / 2, r / 2);

        QBrush brush(conicalGradient);
        painter.setPen(Qt::NoPen);
        painter.setBrush(brush);
        painter.drawEllipse(QPoint(0,0),r/2-margin,r/2-margin);
        /* inner circle */
        painter.setBrush(background);
        painter.drawEllipse(QPoint(0,0),r/2-margin-wheelWidth,r/2-margin-wheelWidth);
    }
};
#endif // WHEELCONTROLLER_H

