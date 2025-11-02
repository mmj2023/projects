import QtQuick
import Quickshell
import Quickshell.Wayland
import Quickshell.Hyprland

Scope {
    Variants {
        model: Quickshell.screens
        LazyLoader {
            required property ShellScreen modelData
            active: true
            component: PanelWindow {
                screen: modelData
                anchors.top: true
                anchors.right: true
                implicitWidth: 200
                implicitHeight: 100
                color: "transparent"
                WlrLayershell.layer: WlrLayer.Top   // instead of Overlay
                exclusiveZone: implicitHeight       // reserve space so windows avoid it


                Text {
                    anchors.centerIn: parent
                    text: "Hello"
                    color: "white"
                }
            }
        }
    }
}
