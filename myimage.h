#ifndef MYIMAGE
#define MYIMAGE

#include <QtQuick/QQuickPaintedItem>
#include <QColor>
#include <QImage>
#include "template.h"
//![0]
class MyImage : public QQuickPaintedItem
{
//![0]
    Q_OBJECT
    Q_PROPERTY(QString name READ name WRITE setName)
    Q_PROPERTY(QString fileName READ fileName WRITE setFileName NOTIFY fileNameChanged)
    //Q_PROPERTY(TemplateValue TV READ TV WRITE setTV)
//![1]
public:
//![1]

    MyImage(QQuickItem *parent = 0);

    QString name() const;
    void setName(const QString &name);
    QString fileName() const;
    void setFileName(const QString &fileName);
    QImage image() const;
    void setImage(const QImage &image);
    void reset() {
        m_TV.id = -1;
    }
    void paint(QPainter *painter);

    Q_INVOKABLE QVariant TV() const;
    Q_INVOKABLE void setTV(const QVariant &TV);
//![2]
    Q_INVOKABLE void changeFileName(QString fileName);
signals:
    void fileNameChanged();
//![2]

private:
    QImage fit500(QImage * image);
    QString m_name;
    QString m_fileName;
    QImage m_image;
    TemplateValue m_TV;
//![3]
};
//![3]

#endif // MYIMAGE

