#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <QString>
#include <QImage>
#include <QObject>
class TemplateValue{
public:
    TemplateValue() : distance(0), arc(0), id(-1) { }
    TemplateValue(int d, int arc, int id) : distance(d), arc(arc), id(id) { }
    TemplateValue(const TemplateValue &other) { distance = other.distance; arc = other.arc; id = other.id; }
    ~TemplateValue() { }
    long long int distance;
    int arc;
    int id;
};

Q_DECLARE_METATYPE(TemplateValue)

class HueTemplate {
public:
    static int region1Arcs[8];
    static int region2Arcs[8];
    static int regionNums[8];
    static int region2Shift[8];
    static char names[8];
    HueTemplate();
    TemplateValue computeDistance(QImage & image, int id);
    long long int helpComputeDistance(int arc, QImage & image, int id);
    long long int computeArcDistance(int arc, int hue, int id);
    int nearestDistance(int hue1, int hue2);
};
#endif // TEMPLATE_H
