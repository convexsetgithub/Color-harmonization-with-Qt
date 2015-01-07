import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Images 1.0

ApplicationWindow {
    title: qsTr("Color Hamonization")
    width: 1024
    height: 768
    visible: true

    FileDialog {
        id: fileDialog
        nameFilters: [ "Image files (*.jpg *.png)", "All files (*)" ]
        title: "Choose a image"
        onAccepted: {
            //messageDialog.show(qsTr("" + fileUrl));
            mainform.changeInput(fileUrl);
        }
    }

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            MenuItem {
                text: qsTr("&Open")
                onTriggered: fileDialog.open();
            }
            MenuItem {
                text: qsTr("E&xit")
                onTriggered: Qt.quit();
            }
        }
        Menu {
            title: qsTr("&Tools")
            MenuItem {
                text: qsTr("&ComputeTemplate")
                onTriggered: image3.computeMostFitTemplate();
            }
            MenuItem {
                text: qsTr("&DrawTemplate")
                onTriggered: {
                    image2.setTV(image3.TV());
                    image4.setTV(image3.TV());
                }
            }
        }
    }

    toolBar: ToolBar{
        RowLayout {
            anchors.fill: parent
            ToolButton{
                text: "ComputeTemplate"
                iconSource: "computeTemplate.png"
                onClicked: image3.computeMostFitTemplate();
            }
        }
    }

    MainForm {
        id: mainform
        anchors.fill: parent
        /*
        button1.onClicked: messageDialog.show(qsTr("Button 1 pressed"))
        button2.onClicked: messageDialog.show(qsTr("Button 2 pressed"))
        button3.onClicked: messageDialog.show(qsTr("Button 3 pressed"))
        */
        MyImage {
            id: image1
            x: 0
            y: 0
            width: 500
            height: 500
            name: "inputImage"
            fileName: "C://Users/user/Pictures/Flower.jpg"
        }

        MyImage {
            id: image2
            x: 524
            y: 0
            width: 500
            height: 500
            name: "output"
            fileName: image1.fileName;
        }
        HueWheel {
            id: image3
            name: "inputHue"
            x: 0
            y: 506
            width: 150; height: 150
            margin: 1; wheelWidth: 15
            fileName: image1.fileName;
        }

        HueWheel {
            id: image4
            name: "outputHue"
            x: 524
            y: 506
            width: 150; height: 150
            margin: 1; wheelWidth: 15
            fileName: image1.fileName;
        }
        MouseArea {
            anchors.fill: parent
            onClicked: messageDialog.show(qsTr("Got Mouse"));
        }

        Text {
            anchors { bottom: parent.bottom; horizontalCenter: parent.horizontalCenter; bottomMargin: 10 }
            text: "Color Harmoziation. Laisky. Immomo808"
        }
        function changeInput (fileUrl) {
            image1.changeFileName (fileUrl);
        }
    }

    MessageDialog {
        id: messageDialog
        title: qsTr("MessageDialog")

        function show(caption) {
            messageDialog.text = caption;
            messageDialog.open();
        }
    }



}
