#ifndef HUEWHEEL_H
#define HUEWHEEL_H
#include "myimage.h"
#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QImage>
#include <QPainter>
#include <QVector>
#include <QStyleOption>
#include "template.h"
#include "myimage.h"
//![0]
class HueWheel : public QQuickPaintedItem
{
//![0]
    Q_OBJECT
    Q_PROPERTY(int margin READ margin WRITE setMargin)
    Q_PROPERTY(int wheelWidth READ wheelWidth WRITE setWheelWidth)
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    //Q_PROPERTY(TemplateValue TV READ TV WRITE setTV)
//![1]
public:
//![1]

    HueWheel(QQuickItem *parent = 0);
    /*
    HueWheel() : m_margin(1), m_wheelWidth(15), m_name(""), m_fileName("")  { reset(); }
    HueWheel(int m, int ww, int name, int filename, TemplateValue TV) : m_margin(m), m_wheelWidth(ww), m_name(name), m_fileName(filename), m_TV(TV) {}
    HueWheel(const HueWheel &other) { m_margin = other.m_margin; m_wheelWidth = other.m_wheelWidth; m_name = other.m_name; m_fileName = other.m_fileName; m_TV = other.m_TV;  }
    //~HueWheel() { } */

    void paint(QPainter *painter);
    int margin() const {
        return m_margin;
    }
    void setMargin(const int &margin) {
        m_margin = margin;
    }

    int wheelWidth() const {
        return m_wheelWidth;
    }
    void setWheelWidth(const int &wheelWidth) {
        m_wheelWidth = wheelWidth;
    }

    void computeHueHistogram();
    Q_INVOKABLE void shiftImageWithSpatialLocality();
    int targetHueWithSpatialLocality(int i, int j);
    QString name() const;
    void setName(const QString &name);
    QString fileName() const;
    void setFileName(const QString &fileName);
    QImage image() const;
    void setImage(const QImage &image);
    void drawTemplate(QPainter *painter, int rInner);
    void reset() {
        for (int i = 0; i < 360; i++)
            hueHistogram[i] = 0;
        maxHue = 0;
        m_TV.arc = 300;
        m_TV.distance = 0;
        m_TV.id = 3;
    }
    QImage fit500(QImage * image);
    QImage fitX(QImage * image, int X);
    Q_INVOKABLE void shiftImage();
    Q_INVOKABLE QVariant TV() const;
    Q_INVOKABLE void setTV(const QVariant &TV);
    Q_INVOKABLE void computeMostFitTemplate();
    Q_INVOKABLE void computeMostFitTemplateX(int X);
    Q_INVOKABLE void fitTemplateX(int X, int scale);
    Q_INVOKABLE void updateWithFrame();
    Q_INVOKABLE void updateByThread();
    //Q_INVOKABLE QVariant HW() const;
//![2]

signals:
    void fileNameChanged();
//![2]

private:
    int m_margin;
    int m_wheelWidth;
    QString m_name;
    QString m_fileName;
    QImage m_image;
    int hueHistogram[360];
    int maxHue;
    TemplateValue m_TV;
//![3]
};
//![3]

//Q_DECLARE_METATYPE(HueWheel*)
#endif // HUEWHEEL_H

