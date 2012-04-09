/****************************************************************************
** Copyright (c) 2012 Debao Zhang
** All right reserved.
** Web: http://code.google.com/p/qextserialport/
**
** Permission is hereby granted, free of charge, to any person obtaining
** a copy of this software and associated documentation files (the
** "Software"), to deal in the Software without restriction, including
** without limitation the rights to use, copy, modify, merge, publish,
** distribute, sublicense, and/or sell copies of the Software, and to
** permit persons to whom the Software is furnished to do so, subject to
** the following conditions:
**
** The above copyright notice and this permission notice shall be
** included in all copies or substantial portions of the Software.
**
** THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
** EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
** MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
** NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
** LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
** OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
** WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
**
****************************************************************************/

import QtQuick 2.0
import QtExtSerialPort 2.0

Item {
    id: top
    ExtSerialPort
    {
        id: port
        // portName: "COM3"
        baudRate: 19200
        parity: ExtSerialPort.PAR_EVEN
        dataBits: ExtSerialPort.DATA_6
        connected: true

        onDataAvailable: recvText.text = recvText.text + port.stringData
        onConnectedChanged: status.text = port.connected ? "Connected" : "Disconnected"
    }

    Column {
        spacing: 4
        Row {
            Text {
                text : "Port Name:"
            }
            Rectangle {
                border.width: 1
                width: 300
                height: 20
                TextInput {
                    id: portedit
                    text : port.portName
                    anchors.fill: parent
                    onAccepted: {
                        port.connected = false
                        port.portName = portedit.text;
                        port.connected = true;
                    }
                }
            }
        }

        Text {
            id:status
            text : "Disconnected"
        }

        Text {
            color:"blue"
            text:"Receive area:"
        }

        Rectangle {
            border.width: 1
            width: 400
            height: 200
            TextEdit {
                id: recvText
                anchors.fill: parent
                focus: false
                wrapMode: TextEdit.WrapAnywhere
            }
        }

        Text {
            color:"blue"
            text:"Send area:"
        }

        Rectangle {
            border.width: 1
            width: 400
            height: 200
            TextEdit {
                id: sendText
                anchors.fill: parent
                focus: true
                wrapMode: TextEdit.WrapAnywhere
            }
        }

        Rectangle {
            border.width: 1
            height: text.height + 10
            width: text.width + 20;
            radius: 5
            color: "lightblue"
            Text {
                id: text
                anchors.centerIn: parent
                text: "Send"
            }
            MouseArea {
                id: button
                anchors.fill: parent
                onClicked: port.stringData = sendText.text
            }
        }
    }
}
