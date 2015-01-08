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
#include "myimage.h"
#include <QPainter>
#include <math.h>
MyImage::MyImage(QQuickItem *parent)
    : QQuickPaintedItem(parent)
{
    m_TV.id = -1;
}

QString MyImage::name() const
{
    return m_name;
}

void MyImage::setName(const QString &name)
{
    m_name = name;
}

QString MyImage::fileName() const
{
    return m_fileName;
}

void MyImage::setFileName(const QString &fileName)
{
    if (m_fileName != fileName) {
        m_fileName = fileName;
        qDebug() << m_name << "file set" << m_fileName;
        update();
    }
}

QImage MyImage::image() const
{
    return m_image;
}

void MyImage::setImage(const QImage &image)
{
    m_image = image;
}


void MyImage::paint(QPainter *painter)
{
    qDebug() << "FileName = " << m_fileName;
    m_image = QImage(m_fileName);
    m_image = fit500(&m_image);
    if (m_TV.id == -1) {
        painter->drawImage(QPoint(0,0), m_image);
    }
    else {
        HueTemplate HT;
        for (int i = 0; i < m_image.width(); i++) {
            for (int j = 0; j < m_image.height(); j++) {
                QRgb pColor = m_image.pixel(i, j);
                QColor qColor(pColor);
                int hue = qColor.hsvHue();
                int targetHue = HT.targetHue(m_TV.arc, hue, m_TV.id);
                qColor.setHsv(targetHue, qColor.hsvSaturation(), qColor.value(), qColor.alpha());
                m_image.setPixel(i, j, qColor.rgb());
            }
        }
        painter->drawImage(QPoint(0,0), m_image);
    }
}

QVariant MyImage::TV() const {
    QVariant var;
    var.setValue(m_TV);
    return var;
}
void MyImage::setTV(const QVariant& TV) {
    m_TV = TV.value<TemplateValue>();
    qDebug() << "Name = " << m_name << "ID = " << m_TV.id << "arc = " << m_TV.arc << "distance" << m_TV.distance;
    update();
}

QImage MyImage::fit500(QImage * image) {
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

//![0]

void MyImage::changeFileName(QString fileName) {
    m_fileName = fileName.remove(0, 8);
    update();
    emit fileNameChanged();
}
//![0]

