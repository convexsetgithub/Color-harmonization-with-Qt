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

long long int * HueTemplate::computeArcDistanceLabel(int arc, int hue, int id) {
    long long int * labelAndDis = new long long int[5];
    for (int i = 0; i < 5; i++)
        labelAndDis[i] = -1; // 0 for nearest one, 1 for second nearest one
    // use border1 as zero degree;
    if (region1Arcs[id] != 0) {
        int border1 = (arc - region1Arcs[id]/2 + 360) % 360;
        int border2 = region1Arcs[id];
        int shiftHue = (hue - border1 + 360) % 360;
        border1 = 0;
        int d1 = nearestDistance(border1, shiftHue);
        int d2 = nearestDistance(border2, shiftHue);
        if (d1 < d2) {
            labelAndDis[0] = d1;
            labelAndDis[1] = d2;
            labelAndDis[2] = 1;
            labelAndDis[3] = -1;
        }
        else {
            labelAndDis[0] = d2;
            labelAndDis[1] = d1;
            labelAndDis[2] = -1;
            labelAndDis[3] = 1;
        }
        if (shiftHue < border2) {
            labelAndDis[4] = 1;
        }
    }
    if (region2Arcs[id] != 0) {
        int border1 = (arc + region2Shift[id] - region2Arcs[id]/2 + 360) % 360;
        int border2 = region2Arcs[id];
        int shiftHue = (hue - border1 + 360) % 360;
        border1 = 0;
        int d1 = nearestDistance(border1, shiftHue);
        int d2 = nearestDistance(border2, shiftHue);
        if (d1 < labelAndDis[0]) {
            labelAndDis[1] = labelAndDis[0];
            labelAndDis[3] = labelAndDis[2];
            labelAndDis[0] = d1;
            labelAndDis[2] = 2;
        }
        if (d2 < labelAndDis[0]) {
            labelAndDis[1] = labelAndDis[0];
            labelAndDis[3] = labelAndDis[2];
            labelAndDis[0] = d2;
            labelAndDis[2] = -2;
        }
        if (shiftHue < border2) {
            labelAndDis[4] = 1;
        }
    }
    return labelAndDis;
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

int HueTemplate::distance(int hue1, int hue2, int direction) {
    int d = (hue1 - hue2 + 360) % 360;
    if (direction < 0) {
        return d;
    }
    else {
        return 360 - d;
    }
}

int HueTemplate::targetHue(int arc, int hue, int id, int border) {
    if (id == 7)
        return -1;
    if (hue == -1)
        return hue;
    int targetHue = hue;
    float delta;
    float center;
    float distanceToCenter;
    // use border1 as zero degree;
    if (border == 1 || border == -1) {
        delta = region1Arcs[id]/2.0;
        center = arc;
        distanceToCenter = distance(hue, (int)center, border);
        targetHue = center - ((float)border) * (((float)region1Arcs[id])/2.0) * ( 1.0 - (1.0) * exp(- (distanceToCenter * distanceToCenter)/ (2.0 * delta * delta)));
        targetHue = (targetHue + 360) % 360;
    }
    else if (border == 2 || border == -2) {
        delta = region2Arcs[id]/2.0;
        center = (arc + region2Shift[id] + 360) % 360;
        distanceToCenter = distance(hue, (int)center, border);
        targetHue = center - ((float)border/2.0) * (((float)region2Arcs[id])/2.0) * ( 1.0 - (1.0) * exp(- (distanceToCenter * distanceToCenter)/ (2.0 * delta * delta)));
        targetHue = (targetHue + 360) % 360;
    }
    return targetHue;
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
        targetHue = center + counterOrcouterWise(hue, (int)center) * (((float)region2Arcs[id])/2.0) * ( 1.0 - (1.0) * exp(- (distanceToCenter * distanceToCenter)/ (2.0 * delta * delta)));
        targetHue = (targetHue + 360) % 360;
    }
    return targetHue;
}

int HueTemplate::targetHueWithSpatialLocality(int i, int j, QImage &image, TemplateValue m_TV) {
    float hues[9], s[9], targetDistances[18];
    int targetBorders[18];
    int insideImage[9];
    int insideSector[9];
    insideImage[0] = 1;
    QColor qColor = QColor::fromRgb(image.pixel(i, j));
    hues[0] = qColor.hsvHue();
    s[0]= qColor.hsvSaturation();
    int locality = 4;
    int x[8] = {0, 1, 0, -1, 1, -1, 1, -1};
    int y[8] = {1, 0, -1, 0, 1, -1, -1, 1};
    for (int m = 0; m < locality; m++) {
        int targetX = i + x[m];
        int targetY = j + y[m];
        if ( targetX >= 0 && targetX < image.width() && targetY >= 0 && targetY < image.height() ) {
            QColor tempQ = QColor::fromRgb(image.pixel(targetX, targetY));
            hues[m + 1] = tempQ.hsvHue();
            s[m + 1] = tempQ.hsvSaturation();
            insideImage[m + 1] = 1;
        }
        else {
            insideImage[m + 1] = 0;
        }
    }
    for (int m = 0; m < locality + 1; m++) {
        //Get TargetBorders;
        if (insideImage[m]) {
            long long int * labelAndDis = computeArcDistanceLabel(m_TV.arc, hues[m], m_TV.id);
            targetDistances[m] = labelAndDis[0];
            targetDistances[m + locality + 1] = labelAndDis[1];
            targetBorders[m] = labelAndDis[2];
            targetBorders[m + locality + 1] = labelAndDis[3];
            insideSector[m] = labelAndDis[4];
            delete [] labelAndDis;
        }
    }
    float lamda = 1, E1, E2, targetE;
    int targetCombination = 0;
    E1 = 0, E2 = 0;
    int mychoice = ((1 << 0) & targetCombination) > 0 ? 1 : 0;
    for (int k = 0; k < locality + 1; k++) {
        if (insideImage[k]) {
            int m = ((1 << k) & targetCombination) > 0 ? 1 : 0;
            E1 += (targetDistances[k + m * (locality + 1)]  / 180.0 * M_PI) * s[k];
            if (targetBorders[mychoice * (locality + 1)] != targetBorders[k + m * (locality + 1)] && k != 0) {
                float biggerS = s[0] > s[k] ? s[0] : s[k];
                E2 += biggerS / ((float)nearestDistance(hues[k], hues[0]) / 180.0 * M_PI);
            }
        }
    }
    targetE = lamda * E1 + E2;
    for (int combination = 1; combination < 32; combination++) {
        E1 = 0, E2 = 0;
        mychoice = ((1 << 0) & combination) > 0 ? 1 : 0;
        int meetHard = 1;
        for (int k = 0; k < locality + 1; k++) {
            if (insideImage[k]) {
                //Check hard constraint
                int m = ((1 << k) & combination) > 0 ? 1 : 0;
                if (insideSector[k] == 1 && m != 0) {
                    meetHard = 0;
                    break;
                }
                //qDebug() << "m ==" << m;
                E1 += (targetDistances[k + m * (locality + 1)]  / 180.0 * M_PI) * s[k];
                if (targetBorders[mychoice * (locality + 1)] != targetBorders[k + m * (locality + 1)] && k != 0) {
                    float biggerS = s[0] > s[k] ? s[0] : s[k];
                    E2 += biggerS / ((float)nearestDistance(hues[k], hues[0]) / 180.0 * M_PI);
                }
            }
        }
        if(!meetHard)
            continue;
        float newTargetE = lamda * E1 + E2;
        if (newTargetE < targetE) {
            targetE = newTargetE;
            targetCombination = combination;
        }
    } 
    //Draw neighbors now

    for (int m = 0; m < locality; m++) {
        int targetX = i + x[m];
        int targetY = j + y[m];
        if ( targetX >= 0 && targetX < image.width() && targetY >= 0 && targetY < image.height() ) {
            QColor qColor = QColor::fromRgb(image.pixel(targetX, targetY));
            int choice = ((1 << (m + 1)) & targetCombination) > 0 ? 1 : 0;
            int targetHue = this->targetHue(m_TV.arc, hues[m + 1], m_TV.id, targetBorders[ m + 1 + choice * (locality + 1)]);
            QColor targetColor = QColor::fromHsv(targetHue, qColor.hsvSaturation(), qColor.value(), qColor.alpha());
            image.setPixel(targetX, targetY, qRgb(targetColor.redF() * 255.0, targetColor.greenF() * 255.0, targetColor.blueF() * 255.0));
        }
    }
    mychoice = 1 & targetCombination;
    return targetHue(m_TV.arc, hues[0], m_TV.id, targetBorders[mychoice * (locality + 1)]);
}
