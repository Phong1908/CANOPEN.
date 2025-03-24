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
#include <QMap>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);


    // Tạo đối tượng Serial
    Serial = new QSerialPort(this);
    // Kết nối tín hiệu với slot xử lý

    connect(ui->pushButton_Send, &QPushButton::clicked, this, &MainWindow::sendCanData);



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
    // updateTextBrowser(data);

    // // Hiển thị dữ liệu thô trong qDebug
    // qDebug() << "Received Raw Data: " << data;
    parseCanData(data);
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

///gửi dữ liệu qua cổng com
void MainWindow::sendCanData()
{


    QString idHex = ui->lineEdit_CAN_ID->text();     // Lấy ID từ QLineEdit
    QString dataHex = ui->lineEdit_CAN_Data->text(); // Lấy Data từ QLineEdit

    QByteArray idData = hexStringToByteArray(idHex);  // Chuyển ID sang QByteArray
    QByteArray frameData = hexStringToByteArray(dataHex); // Chuyển Data sang QByteArray

    if (idData.size() != 4 || frameData.size() != 8) // Kiểm tra đúng kích thước
    {
        qDebug() << "Loi: ID phai có 4 byte và Data phai có 8 byte!";
        return;
    }

    QByteArray frame;
    frame.append((char)0xAA); // Start Mark 1
    frame.append((char)0xAA); // Start Mark 2
    frame.append(idData); // Thêm 4 byte ID vào Frame
    frame.append(frameData); // Thêm 8 byte Data vào Frame

    // **Thêm các trường bổ sung**
    frame.append((char)0x08); // Frame Data Length = 8 byte
    frame.append((char)0x00); // Message Type = 0x00 (CAN Message)
    frame.append((char)0x00); // CAN Frame Type = 0x01 (Extended)
    frame.append((char)0x00); // CAN Request Type = 0x01 (Remote Request Frame)

    // **Tính toán CRC**
    QByteArray crcData = idData + frameData;
    crcData.append((char)0x08); // Thêm Frame Data Length vào CRC
    crcData.append((char)0x00); // Thêm Message Type vào CRC
    crcData.append((char)0x00); // Thêm CAN Frame Type vào CRC
    crcData.append((char)0x00); // Thêm CAN Request Type vào CRC

    QByteArray processedData;
    for (char byte : crcData)
    {
        if (byte == (char)0xA5 || byte == (char)0xAA || byte == (char)0x55)
        {
            processedData.append((char)0xA5); // Thêm FrameCtrl nếu gặp ký tự đặc biệt
        }
        processedData.append(byte);
    }

    uchar crc = 0;
    for (char byte : processedData)
    {
        crc += (uchar)byte; // Tổng đơn giản
    }
    crc &= 0xFF;

    if (crc == 0xA5 || crc == 0xAA || crc == 0x55)
    {
        frame.append((char)0xA5); // Chống trùng CRC với ký tự đặc biệt
    }
    frame.append(crc); // Thêm CRC vào Frame

    frame.append((char)0x55); // End
    frame.append((char)0x55); // End

    // **Gửi dữ liệu qua COM**
    Serial->write(frame);

    if (Serial->waitForBytesWritten(1000)) // Chờ gửi xong
    {
        QString sentData = frame.toHex(' ').toUpper();
        qDebug() << "Sent Data (HEX): " << frame.toHex(' ').toUpper();
        updateTextBrowserSent(sentData);
    }
    else
    {
        qDebug() << "Loi khi gui du lieu!";
    }
}

QByteArray MainWindow::hexStringToByteArray(const QString &hex)
{
    QByteArray byteArray;
    QStringList hexList = hex.split(' ', Qt::SkipEmptyParts); // Tách từng byte
    for (const QString &byte : hexList)
    {
        bool ok;
        byteArray.append(static_cast<char>(byte.toInt(&ok, 16))); // Chuyển từ HEX sang Byte
        if (!ok)
        {
            qDebug() << "Loi chuyen đoi du lieu HEX!";
            return QByteArray(); // Trả về rỗng nếu có lỗi
        }
    }
    return byteArray;
}

void MainWindow::updateTextBrowserSent(const QString &data)
{
    QString displayText = ui->textBrowser_sent->toPlainText();
    displayText.append(data + "  |  " + QTime::currentTime().toString("hh:mm:ss") + "\n");
    ui->textBrowser_sent->setPlainText(displayText);

    // **Tự động cuộn xuống cuối**
    QTextCursor cursor = ui->textBrowser_sent->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser_sent->setTextCursor(cursor);
}


void MainWindow::parseCanData(const QByteArray &data) {
    if (data.size() < 20) { // Kiểm tra độ dài hợp lệ
        qDebug() << "Dữ liệu nhận được không hợp lệ!";
        return;
    }

    // Bỏ qua 2 byte đầu (AA AA)
    int index = (uchar)data[3] << 8 | (uchar)data[2]; // ID (Little Endian)
    int subindex = (uchar)data[5] << 8 | (uchar)data[4]; // Subindex (Byte thứ 6)

    // Lấy dữ liệu 8 byte tiếp theo
    QByteArray valueData = data.mid(6, 8);
    QString dataHex = valueData.toHex(' ').toUpper(); // Chuyển thành chuỗi HEX

    // Chuyển dữ liệu sang số nguyên (Big Endian)
    qint64 valueDecimal = 0;
    for (int i = 0; i < 4; i++) {
        valueDecimal = (valueDecimal << 8) | (uchar)valueData[i];
    }

    // Cập nhật vào bảng QTableWidget
    updateTableValue(index, subindex, dataHex);
    updateTableValue1(index, subindex, dataHex, QString::number(valueDecimal));
}

void MainWindow::updateTableValue(int index, int subindex, const QString &dataHex) {
    int rowCount = ui->tableWidget->rowCount();

    for (int row = 0; row < rowCount; row++) {
        if (ui->tableWidget->item(row, 0) && ui->tableWidget->item(row, 1)) {
            int existingIndex = ui->tableWidget->item(row, 0)->text().toInt(nullptr, 16);
            int existingSubindex = ui->tableWidget->item(row, 1)->text().toInt(nullptr, 16);

            if (existingIndex == index && existingSubindex == subindex) {
                ui->tableWidget->setItem(row, 3, new QTableWidgetItem(dataHex)); // Cập nhật ô "Value"
                return;
            }
        }
    }

    qDebug() << "Không tìm thấy ID" << QString::number(index, 16).toUpper()
             << "Subindex" << QString::number(subindex, 16).toUpper();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// Lưu trữ vị trí của từng index, subindex
QMap<QPair<int, int>, int> rowMap;

void MainWindow::updateTableValue1(int index, int subindex, const QString &dataHex, const QString &valueDecimal) {
    QPair<int, int> key = qMakePair(index, subindex);

    // Kiểm tra nếu ID đã tồn tại trong bảng
    if (rowMap.contains(key)) {
        int row = rowMap[key];
        ui->tableWidget_2->setItem(row, 2, new QTableWidgetItem(dataHex));   // Cập nhật cột "Data" (cột 2)
        ui->tableWidget_2->setItem(row, 3, new QTableWidgetItem(valueDecimal)); //Cập nhật cột "Value" (cột 3)
    } else {
        // Thêm dòng mới
        int newRow = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->insertRow(newRow);

        // Gán giá trị vào từng cột
        ui->tableWidget_2->setItem(newRow, 0, new QTableWidgetItem(QString::number(index, 16).toUpper())); // Index
        ui->tableWidget_2->setItem(newRow, 1, new QTableWidgetItem(QString::number(subindex, 16).toUpper())); // Subindex
        ui->tableWidget_2->setItem(newRow, 2, new QTableWidgetItem(dataHex));  // Tạm thời để trống
        ui->tableWidget_2->setItem(newRow, 3, new QTableWidgetItem(valueDecimal));   // Cột "Value"

        // Lưu vị trí vào map để cập nhật nhanh hơn
        rowMap[key] = newRow;
    }
}




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

