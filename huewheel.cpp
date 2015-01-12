/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the documentation of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/
#include "huewheel.h"
#include "template.h"
#include <QPainter>
#include <QtCore/qmath.h>
#include "math.h"
#include "huewheelthread.h"

extern GlobalTemplate GT;

HueWheel::HueWheel(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    reset();
}

QString HueWheel::name() const
{
    return m_name;
}

void HueWheel::setName(const QString &name)
{
    m_name = name;
}

QImage HueWheel::image() const
{
    return m_image;
}

void HueWheel::setImage(const QImage &image)
{
    o_image = image;
}

QString HueWheel::fileName() const
{
    return m_fileName;
}

QVariant HueWheel::TV() const {
    QVariant var;
    var.setValue(m_TV);
    return var;
}

void HueWheel::setTV(const QVariant& TV) {
    m_TV = TV.value<TemplateValue>();
    qDebug() << "Name = " << m_name << "ID = " << m_TV.id << "arc = " << m_TV.arc << "distance" << m_TV.distance;
}


void HueWheel::setFileName(const QString &fileName)
{
    reset();
    m_fileName = fileName;
    o_image = QImage(fileName);
    o_image = fit500(&o_image);
    m_image = o_image;
    qDebug() << m_name << "file set" << m_fileName << "format = " << m_image.format();
    computeHueHistogram();
    update();

}

void HueWheel::shiftImage() {
    HueTemplate HT;
    m_image = o_image;
    for (int i = 0; i < m_image.width(); i++) {
        for (int j = 0; j < m_image.height(); j++) {
            QColor qColor = QColor::fromRgb(m_image.pixel(i, j));
            int hue = qColor.hsvHue();
            int targetHue = HT.targetHue(m_TV.arc, hue, m_TV.id);
            QColor targetColor = QColor::fromHsv(targetHue, qColor.hsvSaturation(), qColor.value(), qColor.alpha());
            m_image.setPixel(i, j, qRgb(targetColor.redF() * 255.0, targetColor.greenF() * 255.0, targetColor.blueF() * 255.0));
        }
    }
    computeHueHistogram();
    update();
}

void HueWheel::paint(QPainter *painter)
{
    QBrush background = QBrush(QColor("white"));
    qDebug()<< "Paint Again Name:" << m_name;
    qDebug()<< "Width = "<< painter->window().width() << "Height = " << painter->window().height();
    int r = qMin(painter->window().width(), painter->window().height());


    painter->setRenderHint(QPainter::Antialiasing);

    QConicalGradient conicalGradient(0, 0, 0);
    conicalGradient.setColorAt(0.0, Qt::red);
    conicalGradient.setColorAt(60.0/360.0, Qt::yellow);
    conicalGradient.setColorAt(120.0/360.0, Qt::green);
    conicalGradient.setColorAt(180.0/360.0, Qt::cyan);
    conicalGradient.setColorAt(240.0/360.0, Qt::blue);
    conicalGradient.setColorAt(300.0/360.0, Qt::magenta);
    conicalGradient.setColorAt(1.0, Qt::red);

    /* outer circle */
    painter->translate(r / 2, r / 2);

    QBrush brush(conicalGradient);
    painter->setPen(Qt::NoPen);
    painter->setBrush(brush);
    painter->drawEllipse(QPoint(0,0),r/2-m_margin,r/2-m_margin);
    /* inner circle */
    painter->setBrush(background);
    painter->drawEllipse(QPoint(0,0),r/2-m_margin-m_wheelWidth,r/2-m_margin-m_wheelWidth);
    // Draw hueHistorgram
    painter->setPen(QPen(conicalGradient, 1, Qt::SolidLine));
    HueTemplate HT;
    for (int i = 0; i < 360; i++) {
        float rInner = r/2-m_margin-m_wheelWidth;
        float per;
        if (maxHue != 0) {
            per = rInner - ((float)hueHistogram[i]/(float)maxHue) * rInner;
            painter->drawLine(QPoint(rInner * qCos((float)i/180.0 * M_PI), -rInner * qSin((float)i/180.0 * M_PI)), QPoint(per * qCos((float)i/180.0 * M_PI), -per * qSin((float)i/180.0* M_PI)));
        }
    }
    // Black circles
    painter->setPen(Qt::NoPen);
    painter->setBrush(QBrush(QColor("black")));
    painter->drawEllipse(QPoint(0,0), 3, 3);
    painter->setPen(QPen(QBrush(QColor("black")), 2, Qt::SolidLine));
    float rInner = r/2-m_margin-m_wheelWidth;
    painter->drawArc(-rInner, -rInner, rInner * 2, rInner * 2, 0, 16 * 360);
    float rOuter = r/2-m_margin;
    painter->drawArc(-rOuter, -rOuter, rOuter * 2, rOuter * 2, 0, 16 * 360);
    drawTemplate(painter, rInner);
}

void HueWheel::drawTemplate(QPainter *painter, int rInner) {

    if (m_TV.id == -1)
        return;
    HueTemplate HT;
    int border1;
    painter->setPen(Qt::NoPen);
    if (HT.region1Arcs[m_TV.id] != 0) {
        border1 = (m_TV.arc - HT.region1Arcs[m_TV.id]/2 + 360) % 360;
        //qDebug() << "b1 = " << border1;
        painter->setBrush(QBrush(QColor(0, 0, 0, 80)));
        painter->drawPie(-rInner, -rInner, rInner * 2, rInner * 2, border1 * 16, HT.region1Arcs[m_TV.id] * 16);
    }
    if (HT.region2Arcs[m_TV.id] != 0) {
        border1 = (m_TV.arc + HT.region2Shift[m_TV.id] - HT.region2Arcs[m_TV.id]/2 + 360) % 360;
        painter->setBrush(QBrush(QColor(0, 0, 0, 80)));
        painter->drawPie(-rInner, -rInner, rInner * 2, rInner * 2, border1 * 16, HT.region2Arcs[m_TV.id] * 16);
    }
}

void HueWheel::computeMostFitTemplate() {
    HueTemplate HT;
    m_image = o_image;
    m_TV = HT.computeDistance(m_image, 0);
    qDebug() << "Type = " << HT.names[0] << "Arc = " << m_TV.arc << "Distance" << m_TV.distance;
    for (int i = 1; i < 7; i++) {
        TemplateValue temp = HT.computeDistance(m_image, i);
        if (temp.distance < m_TV.distance)
            m_TV = temp;
        qDebug() << "Type = " << HT.names[i] << "Arc = " << temp.arc << "Distance" << temp.distance;
    }
    update();
}

void HueWheel::computeMostFitTemplateX(int X) {
    HueTemplate HT;
    QImage imageResize = QImage(m_fileName);
    imageResize = fitX(&imageResize, X);
    m_TV = HT.computeDistance(imageResize, 0);
    qDebug() << "Type = " << HT.names[0] << "Arc = " << m_TV.arc << "Distance" << m_TV.distance;
    for (int i = 1; i < 7; i++) {
        TemplateValue temp = HT.computeDistance(imageResize, i);
        if (temp.distance < m_TV.distance)
            m_TV = temp;
        qDebug() << "Type = " << HT.names[i] << "Arc = " << temp.arc << "Distance" << temp.distance;
    }
    update();
}

void HueWheel::fitTemplateX(int X, int scale) {
    HueTemplate HT;
    QImage imageResize = QImage(m_fileName);
    imageResize = fitX(&imageResize, scale);
    m_TV = HT.computeDistance(imageResize, X);
    update();
}

void HueWheel::computeHueHistogram() {
    maxHue = 0;
    for (int i = 0; i < 360; i++)
        hueHistogram[i] = 0;
    qDebug() << "Name" << m_name << "width :" << m_image.width() << "height :" << m_image.height();
    for (int i = 0; i < m_image.width(); i++) {
        for (int j = 0; j < m_image.height(); j++) {
            QRgb pColor = m_image.pixel(i, j);
            QColor qColor(pColor);
            int hue = qColor.hsvHue();
            if (hue >= 0) {
                hueHistogram[hue]++;
                if (hueHistogram[hue] > maxHue)
                   maxHue = hueHistogram[hue];
            }
        }
    }
}

QImage HueWheel::fit500(QImage * image) {
    qDebug() << "ImageWidth = " << image->width() << "ImageHeight = " << image->height();
    if (image->width() >= image->height() && image->width() > 500) {
        qDebug() << "fit500 Width";
        return image->scaledToWidth(500);
    }
    else if (image->height() >= image->width() && image->height() > 500) {
        qDebug() << "fit500 Height";
        return image->scaledToHeight(500);
    }
    else
        return *image;
}

QImage HueWheel::fitX(QImage * image, int X) {
    qDebug() << "ImageWidth = " << image->width() << "ImageHeight = " << image->height();
    if (image->width() >= image->height() && image->width() > X) {
        qDebug() << "fit Width" << X;
        return image->scaledToWidth(X);
    }
    else if (image->height() >= image->width() && image->height() > X) {
        qDebug() << "fit Height" << X;
        return image->scaledToHeight(X);
    }
    else
        return *image;
}

void HueWheel::shiftImageWithSpatialLocality() {
    HueTemplate HT;
    m_image = o_image;
    //QImage o_image = QImage(m_fileName);
    //o_image = fit500(&o_image);
    for (int i = 0; i < m_image.width(); i++) {
        for (int j = 0; j < m_image.height(); j++) {
            QColor qColor = QColor::fromRgb(m_image.pixel(i, j));
            //int hue = qColor.hsvHue();
            int targetHue;
            //long long int * labels = HT.computeArcDistanceLabel(m_TV.arc, hue, m_TV.id);
            //if (abs(labels[2]) != abs(labels[3]))
                targetHue = HT.targetHueWithSpatialLocality(i, j, m_image, m_TV);
            //else
                //targetHue =  HT.targetHue(m_TV.arc, hue, m_TV.id);
            //delete [] labels;
            QColor targetColor = QColor::fromHsv(targetHue, qColor.hsvSaturation(), qColor.value(), qColor.alpha());
            m_image.setPixel(i, j, qRgb(targetColor.redF() * 255.0, targetColor.greenF() * 255.0, targetColor.blueF() * 255.0));
        }
    }
    computeHueHistogram();
    update();
}

void HueWheel::updateWithFrame() {
    int counter = 0;
    while(1) {
        if (counter % 100000000 == 0){
            m_TV = GT.m_TV;
            m_image = GT.m_image;
            computeHueHistogram();
            update();
            counter = 0;

        }
    }
}

void HueWheel::updateByThread() {
    HWThread * hwT = new HWThread(this);
    hwT->start();
}


void HueWheel::updateWithFrameTV() {
    while(1) {
        int counter = 0;
        while(1) {
            counter++;
            if (counter % 100000000 == 0){
                m_TV = GT.m_TV;
                shiftImage();
                counter = 0;
            }
        }
    }
}

void HueWheel::updateByThreadShift() {
    HWThreadTV * hwT = new HWThreadTV(this);
    hwT->start();
}



/*
QVariant HueWheel::HW() const {
    QVariant var;
    var.setValue(this);
    return var;
} */


