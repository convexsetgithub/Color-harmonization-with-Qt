#include "template.h"
#include <QApplication>
#include <QDebug>
#include <QColor>
#include <math.h>
int HueTemplate::region1Arcs[8] = {18, 94, 18, 18, 180, 94, 94, 0};
int HueTemplate::region2Arcs[8] = { 0,  0, 80, 18,   0, 18, 94, 0};
int HueTemplate::region2Shift[8] = {0, 0, 270, 180, 0, 180, 180, 0};
char HueTemplate::names[8] = {'i', 'V', 'L', 'I', 'T', 'Y', 'X', 'N'};
int HueTemplate::regionNums[8] = {1, 1, 2, 2, 1, 2, 2, 0};
HueTemplate::HueTemplate() {

}

TemplateValue HueTemplate::computeDistance(QImage & image, int id) {
    TemplateValue result;
    result.arc = 0;
    result.distance = helpComputeDistance(0, image, id);
    result.id = id;
    for (int i = 1; i < 360; i++) {
        long long int d = helpComputeDistance(i, image, id);
        if (d < result.distance) {
            result.arc = i;
            result.distance = d;
        }
    }
    return result;
}

long long int HueTemplate::helpComputeDistance(int arc, QImage &image, int id) {
    long long int dis = 0;
    for (int i = 0; i < image.width(); i++) {
        for (int j = 0; j < image.height(); j++) {
            QRgb pColor = image.pixel(i, j);
            QColor qColor(pColor);
            int hue = qColor.hsvHue();
            int s = qColor.hsvSaturation();
            dis += computeArcDistance(arc, hue, id) * s;
        }
    }
    return dis;
}

int HueTemplate::nearestDistance(int hue1, int hue2) {
    int d = (hue1 - hue2 + 360) % 360;
    if (d > 180)
        d = 360 - d;
    return d;
}

long long int HueTemplate::computeArcDistance(int arc, int hue, int id) {
    long long int dis = 0;
    // use border1 as zero degree;
    if (region1Arcs[id] != 0) {
        int border1 = (arc - region1Arcs[id]/2 + 360) % 360;
        int border2 = region1Arcs[id];
        int shiftHue = (hue - border1 + 360) % 360;
        if (shiftHue < border2)
            return 0;
        border1 = 0;
        int d1 = nearestDistance(border1, shiftHue);
        int d2 = nearestDistance(border2, shiftHue);
        dis = d1 < d2 ? d1 : d2;
    }
    if (region2Arcs[id] != 0) {
        int border1 = (arc + region2Shift[id] - region2Arcs[id]/2 + 360) % 360;
        int border2 = region2Arcs[id];
        int shiftHue = (hue - border1 + 360) % 360;
        if (shiftHue < border2)
            return 0;
        border1 = 0;
        int d1 = nearestDistance(border1, shiftHue);
        int d2 = nearestDistance(border2, shiftHue);
        int dis2 = d1 < d2 ? d1 : d2;
        if (dis2 < dis)
            dis = dis2;
    }
    return dis;
}

int HueTemplate::computeNearRegion(int arc, int hue, int id) {
    long long int dis = 0;
    int region = 1;
    // use border1 as zero degree;
    if (region1Arcs[id] != 0) {
        int border1 = (arc - region1Arcs[id]/2 + 360) % 360;
        int border2 = region1Arcs[id];
        int shiftHue = (hue - border1 + 360) % 360;
        border1 = 0;
        int d1 = nearestDistance(border1, shiftHue);
        int d2 = nearestDistance(border2, shiftHue);
        dis = d1 < d2 ? d1 : d2;
    }
    if (region2Arcs[id] != 0) {
        int border1 = (arc + region2Shift[id] - region2Arcs[id]/2 + 360) % 360;
        int border2 = region2Arcs[id];
        int shiftHue = (hue - border1 + 360) % 360;
        border1 = 0;
        int d1 = nearestDistance(border1, shiftHue);
        int d2 = nearestDistance(border2, shiftHue);
        int dis2 = d1 < d2 ? d1 : d2;
        if (dis2 < dis) {
            dis = dis2;
            region = 2;
        }
    }
    return region;
}

float HueTemplate::counterOrcouterWise(int hue, int center) {
    int d = (hue - center + 360) % 360;
    if (d > 180)
        return -1;
    return 1;
}

int HueTemplate::targetHue(int arc, int hue, int id) {
    if (id == 7)
        return -1;
    if (hue == -1)
        return hue;
    int region = computeNearRegion(arc, hue, id);
    int targetHue = hue;
    float delta;
    float center;
    float distanceToCenter;
    // use border1 as zero degree;
    if (region == 1) {
        delta = region1Arcs[id]/2.0;
        center = arc;
        distanceToCenter = nearestDistance(hue, (int)center);
        targetHue = center + counterOrcouterWise(hue, (int)center) * (((float)region1Arcs[id])/2.0) * ( 1.0 - (1.0) * exp(- (distanceToCenter * distanceToCenter)/ (2.0 * delta * delta)));
        targetHue = (targetHue + 360) % 360;
    }
    else if (region == 2) {
        delta = region2Arcs[id]/2.0;
        center = (arc + region2Shift[id] + 360) % 360;;
        distanceToCenter = nearestDistance(hue, center);
        targetHue = center + counterOrcouterWise(hue, (int)center) * (((float)region2Arcs[id])/2.0) * ( 1.0 - (1.0/((delta/180 * M_PI) * sqrt(2.0 * M_PI))) * exp(- (distanceToCenter * distanceToCenter)/ (2.0 * delta * delta)));
        targetHue = (targetHue + 360) % 360;
    }
    return targetHue;
}

