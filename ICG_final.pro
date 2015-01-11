TEMPLATE = app

QT += qml quick widgets multimedia

SOURCES += main.cpp \
    myimage.cpp \
    huewheel.cpp \
    template.cpp \
    myvideoprobe.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    myimage.h \
    huewheel.h \
    template.h \
    myvideoprobe.h
