TEMPLATE = app

QT += qml quick widgets multimedia

SOURCES += main.cpp \
    myimage.cpp \
    huewheel.cpp \
    template.cpp \
    myvideosurface.cpp

RESOURCES += qml.qrc

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets multimedia multimediawidgets

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    myimage.h \
    huewheel.h \
    template.h \
    myvideosurface.h \
    huewheelthread.h

CONFIG += qt
