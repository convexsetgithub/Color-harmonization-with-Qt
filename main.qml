import QtQuick 2.4
import QtQuick.Controls 1.3
import QtQuick.Window 2.2
import QtQuick.Dialogs 1.2
import QtQuick.Layouts 1.1
import Images 1.0
import QtMultimedia 5.0
ApplicationWindow {
    title: qsTr("Color Hamonization")
    width: 1600
    height: 900
    visible: true

    FileDialog {
        id: fileDialog
        nameFilters: [ "Image files (*jpeg *.jpg *.png)", "All files (*)" ]
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
                text: qsTr("&DrawTemplateLocality")
                onTriggered: {
                    image2.setTV(image3.TV());
                    image4.setTV(image3.TV());
                    image2.shiftImageWithSpatialLocality();
                    image4.shiftImageWithSpatialLocality();
                }
            }
            MenuItem {
                text: qsTr("&DrawTemplateSimple")
                onTriggered: {
                    image2.setTV(image3.TV());
                    image4.setTV(image3.TV());
                    image2.shiftImage();
                    image4.shiftImage();
                }
            }
        }
    }

    toolBar: ToolBar{
        RowLayout {
            //anchors.fill: parent
            ToolButton{
                text: "ComputeTemplate"
                iconSource: "computeTemplate.png"
                onClicked: image3.computeMostFitTemplate();
            }
            //anchors.fill: parent
            ToolButton{
                text: "ComputeTemplate"
                iconSource: "computeTemplateX.png"
                onClicked: image3.computeMostFitTemplateX(50);
            }
            ToolButton{
                text: "DrawTemplate"
                iconSource: "drawTemplate.png"
                onClicked: {
                    image2.setTV(image3.TV());
                    image4.setTV(image3.TV());
                    image2.shiftImageWithSpatialLocality();
                    image4.shiftImageWithSpatialLocality();
                }
            }
            ToolButton{
                text: "drawTemplateNoLocality"
                iconSource: "drawTemplateNoLocality.png"
                onClicked: {
                    image2.setTV(image3.TV());
                    image4.setTV(image3.TV());
                    image2.shiftImage();
                    image4.shiftImage();
                }
            }
            ToolButton{
                text: "reload"
                iconSource: "reload.png"
                onClicked: {
                    image1.reload();
                }
            }
            ToolButton{
                iconSource: "littleI.png"
                onClicked: image3.fitTemplateX(0, 100);
            }
            ToolButton{
                iconSource: "V.png"
                onClicked: image3.fitTemplateX(1, 100);
            }
            ToolButton{
                iconSource: "L.png"
                onClicked: image3.fitTemplateX(2, 100);
            }
            ToolButton{
                iconSource: "I.png"
                onClicked: image3.fitTemplateX(3, 100);
            }
            ToolButton{
                iconSource: "T.png"
                onClicked: image3.fitTemplateX(4, 100);
            }
            ToolButton{
                iconSource: "Y.png"
                onClicked: image3.fitTemplateX(5, 100);
            }
            ToolButton{
                iconSource: "X.png"
                onClicked: image3.fitTemplateX(6, 100);
            }
            ToolButton{
                iconSource: "N.png"
                onClicked: image3.fitTemplateX(7, 100);
            }
            ToolButton{
                iconSource: "camera.png"
                onClicked: {
                    image5.setSurface();
                    image5.openCamera();
                    image6.updateByThread();
                    image2.updateByThreadShift();
                    image4.updateByThreadShift();
                }
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
            fileName: "//Users/momo/icg_final/testing3.png"
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
            width: 300; height: 300
            margin: 1; wheelWidth: 15
            fileName: image1.fileName;
        }

        HueWheel {
            id: image4
            name: "outputHue"
            x: 524
            y: 506
            width: 300; height: 300
            margin: 1; wheelWidth: 15
            fileName: image1.fileName;
        }

        MyImage {
            id: image5
            x: 1048
            y: 0
            width: 500
            height: 500
            name: "cameraImage"
            fileName: "";
        }

        HueWheel {
            id: image6
            name: "cameraHue"
            x: 1048
            y: 506
            width: 300; height: 300
            margin: 1; wheelWidth: 15
            fileName: "";
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
        /*
        Item {
            x: 1048
            y: 0
            width: 500
            height: 900
            Camera {
                id: camera

                imageProcessing.whiteBalanceMode: CameraImageProcessing.WhiteBalanceFlash

                exposure {
                    exposureCompensation: -1.0
                    exposureMode: Camera.ExposurePortrait
                }

                flash.mode: Camera.FlashRedEyeReduction

                imageCapture {
                    onImageCaptured: {
                        photoPreview.source = preview;  // Show the preview in an Image
                        //console.debug(camera.imageCapture.capturedImagePath);
                    }
                }
            }

            VideoOutput {
                id: video
                source: camera
                //anchors.fill: parent
                x: 0
                y: 0
                width: 500
                height: 281
                focus : visible // to receive focus and capture key events when visible
            }

            Image {
                id: photoPreview
                x: 0
                y: 324
                width: 500
                height: 281
            }
        }*/
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
