#include <QApplication>
#include <QQmlApplicationEngine>
#include "colorwheel.h"
#include "huewheel.h"
#include "wheelcontroller.h"
#include "template.h"
#include "myimage.h"

GlobalTemplate GT;
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    qmlRegisterType<MyImage>("Images", 1, 0, "MyImage");
    qmlRegisterType<HueWheel>("Images", 1, 0, "HueWheel");
    //qmlRegisterType<MyVideoProbe>("Images", 1, 0, "MyVideoProbe");
    QQmlApplicationEngine engine;
    engine.addImageProvider(QLatin1String("wheelcontroller"), new WheelController);
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    //ColorWheel colorwheel;
    //colorwheel.show();
    return app.exec();
}
