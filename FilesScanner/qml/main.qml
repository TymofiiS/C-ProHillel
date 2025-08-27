import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Dialogs   // FolderDialog

ApplicationWindow {
    visible: true
    width: 760
    height: 560
    title: "Files Table"

    function humanSize(bytes) {
        if (bytes >= 1024*1024*1024) return (bytes / (1024*1024*1024)).toFixed(1) + " GB"
        if (bytes >= 1024*1024) return (bytes / (1024*1024)).toFixed(1) + " MB"
        if (bytes >= 1024) return Math.round(bytes / 1024) + " KB"
        return bytes + " B"
    }

    // QML-native folder picker
    FolderDialog {
        id: folderDlg
        title: "Select folder"
        onAccepted: controller.setSelectedFolderFromQml(selectedFolder)
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 8

        // Actions
        Frame {
            Layout.fillWidth: true
            padding: 10
            background: Rectangle { color: "transparent"; border.color: "#E5E8EC"; radius: 8 }

            RowLayout {
                anchors.fill: parent
                spacing: 12
                Button { text: "📁 Choose folder"; onClicked: folderDlg.open() }
                Button { text: "🔎 Scan"; enabled: fileModel.selectedFolder !== ""; onClicked: controller.scan() }
                Button { text: "■ Stop"; enabled: controller.scanning; onClicked: controller.stop() }
                Item { Layout.fillWidth: true }
            }
        }

        // Progress
        Frame {
            Layout.fillWidth: true
            padding: 12
            background: Rectangle { color: "transparent"; border.color: "#E5E8EC"; radius: 8 }

            ColumnLayout {
                anchors.fill: parent
                spacing: 8

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Progress:"; Layout.fillWidth: true }
                    Label { text: "0%"; horizontalAlignment: Text.AlignRight }
                }

                ProgressBar { Layout.fillWidth: true; from: 0; to: 100; value: 0 }

                RowLayout {
                    Layout.fillWidth: true
                    Label { text: "Found: 0 files"; Layout.fillWidth: true }
                    Label { text: "Size: 0 B"; horizontalAlignment: Text.AlignRight }
                }
            }
        }

        GroupBox {
            title: "Files Table"
            Layout.fillWidth: true
            Layout.fillHeight: true

            ColumnLayout {
                anchors.fill: parent
                spacing: 0

                HorizontalHeaderView {
                    id: header
                    Layout.fillWidth: true
                    height: 34
                    syncView: table
                    model: [ "NAME", "SIZE", "TYPE", "DATE" ]

                    delegate: Rectangle {
                        color: "#F5F6F8"
                        border.color: "#E0E0E0"
                        Text {
                            anchors.fill: parent
                            anchors.margins: 8
                            text: modelData
                            font.bold: true
                            horizontalAlignment: Text.AlignHCenter
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }
                    }
                }

                TableView {
                    id: table
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    model: fileModel
                    clip: true

                    columnWidthProvider: function(c) {
                        if (c === 1 || c === 2 || c === 3) return 120
                        return Math.max(240, table.width - (120 * 3))
                    }
                    rowHeightProvider: function() { return 34 }

                    delegate: Rectangle {
                        implicitHeight: 34
                        color: row % 2 === 0 ? "#FFFFFF" : "#FAFAFA"
                        border.color: "#EFEFEF"
                        border.width: 1

                        Label {
                            visible: column === 0
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.left: parent.left; anchors.leftMargin: 12
                            width: parent.width - 24
                            text: model.name
                            elide: Text.ElideRight
                        }
                        Label {
                            visible: column === 1
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.right: parent.right; anchors.rightMargin: 12
                            text: humanSize(model.sizeBytes)
                            horizontalAlignment: Text.AlignRight
                        }
                        Label {
                            visible: column === 2
                            anchors.centerIn: parent
                            text: model.type
                        }
                        Label {
                            visible: column === 3
                            anchors.centerIn: parent
                            text: model.dateDisplay
                        }
                    }

                    ScrollBar.vertical: ScrollBar {}
                }
            }
        }

        GroupBox {
            title: "Basic Statistics"
            Layout.fillWidth: true
            Layout.bottomMargin: 5
            bottomPadding: 30

            GridLayout {
                anchors.fill: parent
                anchors.margins: 12
                columns: 2
                rowSpacing: 10
                columnSpacing: 32

                Label { text: "Total files:"; font.bold: true }
                Label { text: fileModel.totalCount }

                Label { text: "Total size:"; font.bold: true }
                Label { text: humanSize(fileModel.totalSizeBytes) }

                Label { text: "Largest file:"; font.bold: true }
                Label { text: fileModel.largestFileName + (fileModel.largestFileName !== "" ? " (" + humanSize(fileModel.largestFileSize) + ")" : "") }

                Label { text: "Most common type:"; font.bold: true }
                Label { text: (fileModel.mostCommonType !== "" ? fileModel.mostCommonType + " (" + fileModel.mostCommonTypePercent + "%)" : "-") }
            }
        }
    }
}
