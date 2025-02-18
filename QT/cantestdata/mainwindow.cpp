#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "qcustomplot.h" // Thư viện QCustomPlot
#include <QSerialPortInfo>
#include <QDebug>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)

{
    ui->setupUi(this);

    // Tạo đối tượng Serial
    Serialll = new QSerialPort(this);

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
    Serialll->setPortName("COM3"); // Thay bằng cổng COM phù hợp
    Serialll->setBaudRate(QSerialPort::Baud115200);
    Serialll->setDataBits(QSerialPort::Data8);
    Serialll->setParity(QSerialPort::NoParity);
    Serialll->setStopBits(QSerialPort::OneStop);
    Serialll->setFlowControl(QSerialPort::NoFlowControl);

    if (Serialll->open(QIODevice::ReadOnly)) {
        qDebug() << "Serial port opened successfully!";
        connect(Serialll, &QSerialPort::readyRead, this, &MainWindow::readCanData);
    } else {
        qDebug() << "Failed to open serial port: " << Serialll->errorString();
        QMessageBox::critical(this, "Error", "Failed to open serial port!");
    }
}

void MainWindow::readCanData()
{
    QByteArray data = Serialll->readAll(); // Đọc dữ liệu từ Serial
    QString receivedData = data.toHex(' ').toUpper(); // Chuyển thành chuỗi HEX

    // Hiển thị dữ liệu vào textBrowser
    updateTextBrowser(receivedData);

    qDebug() << "Received Data: " << receivedData;
}

void MainWindow::updateTextBrowser(const QString &data)
{
    QString displayText = ui->textBrowser->toPlainText();
    displayText.append(data + "  |  " + QTime::currentTime().toString("hh:mm:ss.zzz") + "\n");
    ui->textBrowser->setPlainText(displayText);

    // Tự động cuộn xuống cuối
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->textBrowser->setTextCursor(cursor);
}

void MainWindow::on_pushButton_readData_clicked()
{
    if (Serialll->isOpen()) {
        QByteArray data = Serialll->readAll(); // Đọc dữ liệu từ Serial
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


