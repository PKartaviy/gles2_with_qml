import QtQuick 2.0
import Shapes 1.0


Rectangle{
    id: page
    width: 360
    height: 360
    focus: true
    Keys.onPressed: {
        // Note: on my laptop Esc and Enter have strange and large codes,
        // which differs from Qt.Key_Enter and Qt.Key_Escape
        if (event.key === Qt.Key_Space) {
            Qt.quit();
        }
    }

    states: State{
        name: "down"; when: mouseArea.pressed === true
        PropertyChanges { target: helloText; y: 100; rotation: 180; color: "red" }
    }

    transitions: Transition{
        from: ""; to: "down"; reversible: true
        ParallelAnimation{
            NumberAnimation { properties: "y, rotation"; duration: 500; easing.type: Easing.InOutQuad }
            ColorAnimation { duration: 500 }
        }
    }


    MyTriangle {
        id: triangle
        anchors.fill: parent
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: { // do something
        }
    }

    Column {
        spacing: 10

        Text {
            id: helloText
            anchors.horizontalCenter: parent.horizontalCenter
            text: "Simple example which shows how opengl can work with QML.\n\
               Works on raspberry pi! \n\
               Press Space to exit."
        }

        Button{
            id: exampleButton
            label: "example"
            buttonColor: "darkgrey"

            //        onButtonClick: Qt.quit()
        }
    }
}
