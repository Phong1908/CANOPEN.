#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h" // Thư viện QCustomPlot
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>
#include <QCanBus>
#include <QCanBusDevice>
#include <QCanBusFrame>
#include <QDebug>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);


    // Tạo đối tượng Serial
    Serial = new QSerialPort(this);
    // Kết nối tín hiệu với slot xử lý
    connect(ui->pushButton_Send, &QPushButton::clicked, this, &MainWindow::sendCanFrame);


    // Cài đặt cổng COM
    setupSerialPort();

    // Tạo đối tượng QCustomPlot và gắn nó vào widget customPlotWidget
    QCustomPlot *customPlot = new QCustomPlot(ui->customPlotWidget);
    customPlot->setGeometry(ui->customPlotWidget->rect()); // Đặt kích thước khớp với widget

    // Dữ liệu cho đồ thị
    QVector<double> x(101), y(101); // 101 điểm từ 0 đến 10
    for (int i = 0; i < 101; ++i)
    {
        x[i] = i / 10.0;        // Giá trị X: 0, 0.1, ..., 10
        y[i] = qSin(x[i]);      // Giá trị Y: sin(x)
    }

    // Thêm đồ thị
    customPlot->addGraph();
    customPlot->graph(0)->setData(x, y);

    // Đặt nhãn trục
    customPlot->xAxis->setLabel("X-Axis");
    customPlot->yAxis->setLabel("Y-Axis");

    // Cài đặt khoảng hiển thị
    customPlot->xAxis->setRange(0, 10);
    customPlot->yAxis->setRange(-1, 1);

    // Hiển thị đồ thị
    customPlot->replot();

}



void MainWindow::setupSerialPort()
{
    Serial->setPortName("COM3"); // Thay bằng cổng COM phù hợp
    Serial->setBaudRate(QSerialPort::Baud115200);
    Serial->setDataBits(QSerialPort::Data8);
    Serial->setParity(QSerialPort::NoParity);
    Serial->setStopBits(QSerialPort::OneStop);
    Serial->setFlowControl(QSerialPort::NoFlowControl);

    if (Serial->open(QIODevice::ReadWrite)) {
        qDebug() << "Serial port opened successfully!";
        connect(Serial, &QSerialPort::readyRead, this, &MainWindow::readCanData);
    } else {
        qDebug() << "Failed to open serial port: " << Serial->errorString();
        QMessageBox::critical(this, "Error", "Failed to open serial port!");
    }
}

void MainWindow::readCanData()
{
    QByteArray data = Serial->readAll(); // Đọc dữ liệu từ Serial
    QString receivedData = data.toHex(' ').toUpper(); // Chuyển thành chuỗi HEX

    // Hiển thị dữ liệu vào textBrowser
    updateTextBrowser(receivedData);

    qDebug() << "Received Data: " << receivedData;
}

void MainWindow::updateTextBrowser(const QString &data)
{
    QString displayText = ui->textBrowser->toPlainText();
    displayText.append(data + "  |  " + QTime::currentTime().toString("hh:mm:ss") + "\n");
    ui->textBrowser->setPlainText(displayText);

    // Tự động cuộn xuống cuối
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
}

void MainWindow::on_pushButton_readData_clicked()
{
    if (Serial->isOpen()) {
        QByteArray data = Serial->readAll(); // Đọc dữ liệu từ Serial
        QString receivedData = data.toHex(' ').toUpper(); // Chuyển thành chuỗi HEX

        // Hiển thị dữ liệu vào textBrowser
        updateTextBrowser(receivedData);

        qDebug() << "Received Data: " << receivedData;
    } else {
        QMessageBox::warning(this, "Serial Port", "Please open the connection first!");
    }
}

void MainWindow::on_pushButton_clear_clicked()
{
    ui->textBrowser->clear(); // Xóa toàn bộ dữ liệu
    qDebug() << "TextBrowser cleared!";
}

///gửi dữ liệu
void MainWindow::sendCanFrame()
{
    if (!Serial->isOpen()) {
        QMessageBox::warning(this, "Error", "Open serial port first!");
        return;
    }

    // Lấy dữ liệu từ UI
    bool ok;
    quint32 canId = ui->lineEdit_CAN_ID->text().toUInt(&ok, 16);
    quint8 dlc = static_cast<quint8>(ui->spinBox_DLC->value());
    QByteArray data = QByteArray::fromHex(ui->lineEdit_CAN_Data->text().toLatin1());

    // Kiểm tra dữ liệu hợp lệ
    if (!ok || data.size() != dlc) {
        QMessageBox::warning(this, "Error", "Invalid CAN parameters!");
        return;
    }

    // Tạo khung CAN
    QByteArray frame;
    // Thêm byte Header (ví dụ 0xAA để báo hiệu bắt đầu CAN frame)
    frame.append(0xAA);
    frame.append(static_cast<char>((canId >> 24) & 0xFF));
    frame.append(static_cast<char>((canId >> 16) & 0xFF));
    frame.append(static_cast<char>((canId >> 8) & 0xFF));
    frame.append(static_cast<char>(canId & 0xFF));
    frame.append(static_cast<char>(dlc));
    frame.append(data);
    // Thêm checksum (tổng XOR của tất cả byte trước đó)
    char checksum = 0;
    for (char byte : frame) {
        checksum ^= byte;
    }
    frame.append(checksum);

    // Gửi qua Serial
    Serial->write("hello");
    // Hiển thị dữ liệu gửi vào textBrowser_sent
    updateTextBrowser1(canId, dlc, data);
    qDebug() << "Sent CAN Frame:" << frame.toHex(' ');


}
void MainWindow::updateTextBrowser1(quint32 canId, quint8 dlc, const QByteArray &data)
{
    QString displayText = ui->textBrowser_sent->toPlainText();

    // Chuyển đổi dữ liệu sang chuỗi HEX
    QString canIdStr = QString("%1").arg(canId, 8, 16, QChar('0')).toUpper(); // ID 8 ký tự HEX
    QString dlcStr = QString::number(dlc);
    QString dataStr = data.toHex(' ').toUpper();

    // Thêm vào textBrowser_sent
    displayText.append("ID: " + canIdStr + " | DLC: " + dlcStr + " | DATA: " + dataStr + " | " +
    QTime::currentTime().toString("hh:mm:ss") + "\n");

    ui->textBrowser_sent->setPlainText(displayText);

    // Tự động cuộn xuống cuối
    QTextCursor cursor = ui->textBrowser_sent->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser_sent->setTextCursor(cursor);
}

// void MainWindow::sendCanData(const QByteArray &data)
// {
//     if (Serial->isOpen()) {
//         // Gửi trực tiếp dữ liệu 8 byte
//         Serial->write(data);
//         qDebug() << "Sent Data: " << data.toHex(' ').toUpper();
//     } else {
//         QMessageBox::warning(this, "Serial Port", "Serial port is not open!");
//     }
// }

// void MainWindow::on_pushButton_SEND_clicked()
// {
//     // Dữ liệu nhập vào là một chuỗi hex từ một QLineEdit
//     QString hexData = ui->lineEdit_Data->text().trimmed();

//     if (hexData.isEmpty())
//     {
//         QMessageBox::warning(this, "Input Error", "Please enter data to send!");
//         return;
//     }

//     // Chuyển chuỗi hex thành QByteArray cho phần data
//     QByteArray dataField = QByteArray::fromHex(hexData.toUtf8());

//     // Kiểm tra độ dài dữ liệu (8 byte)
//     if (dataField.size() != 8)
//     {
//         QMessageBox::warning(this, "Input Error", "Data field must be exactly 8 bytes in HEX format!");
//         return;
//     }

//     // Gửi dữ liệu 8 byte trực tiếp
//     sendCanData(dataField);
// }


// // Hàm tính CRC (giống như trong phần trước)
// quint8 calculateCRC(const QByteArray &data) {
//     quint8 crc = 0;
//     for (int i = 0; i < data.size(); i++) {
//         crc += data[i];
//     }
//     return crc;
// }

// // Hàm xây dựng khung CAN
// QByteArray buildCanFrame(quint32 id, const QByteArray &data, bool isCanMessage = true) {
//     QByteArray frame;

//     // 1. Start mark (2 byte 0xAA)
//     frame.append((char)0xAA);
//     frame.append((char)0xAA);

//     // 2. CAN ID (4 byte)
//     frame.append((char)((id >> 24) & 0xFF));
//     frame.append((char)((id >> 16) & 0xFF));
//     frame.append((char)((id >> 8) & 0xFF));
//     frame.append((char)(id & 0xFF));

//     // 3. Data (8 byte)
//     QByteArray dataToSend = data;
//     for (int i = 0; i < dataToSend.size(); i++) {
//         frame.append(dataToSend[i]);
//     }

//     // 4. DLC (1 byte, giả sử 8 byte cho dữ liệu)
//     frame.append((char)8);

//     // 5. Message Type (00: CAN)
//     frame.append((char)(isCanMessage ? 0x00 : 0xFF));

//     // 6. IDE Flag (1 byte)
//     frame.append((char)0x00);

//     // 7. Request Flag (1 byte)
//     frame.append((char)0x00);

//     // 8. CRC (1 byte)
//     quint8 crc = calculateCRC(frame);
//     frame.append((char)crc);

//     // 9. End mark (2 byte 0xFF)
//     frame.append((char)0xFF);
//     frame.append((char)0xFF);

//     return frame;
// }

// // Hàm gửi khung CAN qua cổng serial
// void sendCanFrame(QSerialPort *Serial, const QByteArray &frame) {
//     if (Serial && Serial->isOpen()) {
//         Serial->write(frame);
//         qDebug() << "CAN frame sent: " << frame.toHex();
//     } else {
//         qDebug() << "Serial port is not open!";
//     }
// }

// void MainWindow::on_pushButton_SEND_clicked()

//     {
//     QString inputData = ui->lineEdit_Data->text();  // Lấy dữ liệu từ QLineEdit

//     if (inputData.isEmpty()) {
//         QMessageBox::warning(this, "Input Error", "Please enter data to send!");
//         return;
//     }

//     // Chuyển đổi dữ liệu từ chuỗi nhập vào thành QByteArray (dữ liệu phải là chuỗi hex)
//     QByteArray data = QByteArray::fromHex(inputData.toUtf8());

//     // Giả sử ID CAN là 0x123
//     QByteArray canFrame = buildCanFrame(0x123, data);

//     // Gửi dữ liệu qua cổng serial
//     sendCanFrame(Serial, canFrame);

//     }


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// // Hàm gửi dữ liệu CAN
// void MainWindow::sendCanData(const QByteArray &data)
// {
//     if (Serial->isOpen()) {
//         Serial->write(data);
//         qDebug() << "Sent Data: " << data.toHex(' ').toUpper();
//     } else {
//         QMessageBox::warning(this, "Serial Port", "Serial port is not open!");
//     }
// }


// // Nút pushButton_sendData_clicked: Gửi dữ liệu CAN từ Qt tới STM32
//     void MainWindow::on_pushButton_SEND_clicked()
// {
//     // Dữ liệu nhập vào là một chuỗi hex từ  QLineEdit
//     QString hexData = ui->lineEdit_Data->text().trimmed();

//     if (hexData.isEmpty())
//     {
//         QMessageBox::warning(this, "Input Error", "Please enter data to send!");
//         return;
//     }

//     // Chuyển chuỗi hex thành QByteArray cho phần data
//     QByteArray dataField = QByteArray::fromHex(hexData.toUtf8());

//     // Kiểm tra độ dài dữ liệu (8 byte)
//     if(dataField.size() != 8)
//     {
//         QMessageBox::warning(this, "Input Error", "Data field must be exactly 8 bytes in HEX format!");
//         return;
//     }

//     // Tạo frame CAN gồm:
//     // 1. Identifier (11-bit), trong ví dụ sử dụng 0x446 (lưu vào 16 bit, giá trị cao không sử dụng)
//     quint16 stdId = 0x103;
//     QByteArray frame;
//     frame.append(reinterpret_cast<const char*>(&stdId), sizeof(stdId));

//     // 2. Byte chứa RTR, IDE và DLC
//     // Bit7-4: DLC (8), bit1: IDE (0, chuẩn), bit0: RTR (0, data frame)
//     quint8 controlByte = (8 << 4) | (0 << 1) | (0);
//     frame.append(controlByte);

//     // 3. Data field: 8 byte dữ liệu
//     frame.append(dataField);

//     // Gửi frame qua serial
//     sendCanData(frame);
// }

// {
//     if (Serial->isOpen()) {
//         QMessageBox::warning(this, "Serial Port", "Please open the connection first!");
//         return;
//     }

//     // Lấy dữ liệu từ QLineEdit (giả sử dữ liệu nhập vào là chuỗi hex)
//     QString dataStr = ui->lineEdit_Data->text();
//     if (dataStr.isEmpty() || dataStr.length() != 16) { // 16 ký tự hex = 8 byte
//         QMessageBox::warning(this, "Data", "Please enter 8 bytes of data in hex format (16 characters)!");
//         return;
//     }

//     // Chuyển đổi chuỗi hex thành QByteArray
//     QByteArray data = QByteArray::fromHex(dataStr.toUtf8());


//     quint16 id = 0x103; // ID 11 bit (giá trị từ 0 đến 2047)

//     // Đóng gói ID vào 2 byte
//     quint8 idByte1 = (id >> 3) & 0xFF; // 8 bit cao của ID
//     quint8 idByte2 = (id << 5) & 0xFF; // 3 bit thấp của ID, dịch sang trái 5 bit

//     // Tạo gói dữ liệu
//     QByteArray sendData;
//     sendData.append(reinterpret_cast<const char*>(&idByte1), 1); // Thêm byte ID đầu tiên
//     sendData.append(reinterpret_cast<const char*>(&idByte2), 1); // Thêm byte ID thứ hai
//     sendData.append(data); // Thêm 8 byte dữ liệu

//     // In ra console để kiểm tra
//     qDebug() << "Data to be sent (Hex):" << sendData.toHex();

//     // Gửi dữ liệu qua cổng Serial
//     qint64 bytesWritten = Serial->write(sendData);
//     if (bytesWritten == -1) {
//         qDebug() << "Failed to write data to serial port.";
//         QMessageBox::warning(this, "Serial Port", "Failed to write data to serial port.");
//         return;
//     }

//     // Đảm bảo rằng dữ liệu đã được gửi đi
//     if (Serial->waitForBytesWritten(1000)) {
//         qDebug() << "Data sent with ID:" << id << ", Data:" << sendData.toHex();
//     } else {
//         qDebug() << "Failed to send data.";
//         QMessageBox::warning(this, "Serial Port", "Failed to send data.");
//     }
// }

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_STOP_released()
{
    ui->label_DK->setText("NGAT KET NOI");
}


void MainWindow::on_pushButton_RUN_released()
{
    ui->label_DK->setText("DA KET NOI");
}



void MainWindow::on_pushButton_Send_clicked()
{

}

