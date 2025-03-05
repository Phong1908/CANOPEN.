#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "qcustomplot.h" // Thêm thư viện QCustomPlot
#include <QSerialPort>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void writeCanData(const QByteArray &data); //

private slots:
    void on_pushButton_STOP_released();
    void on_pushButton_RUN_released();
    void on_pushButton_readData_clicked(); // Đọc dữ liệu
    void on_pushButton_clear_clicked();   // Xóa dữ liệu
    void readCanData();                    // Đọc dữ liệu từ Serial

    void on_pushButton_SEND_clicked();

    void on_pushButton_Send_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *Serial;                 // Serial port object
    QTimer *timer;                         // Đọc dữ liệu theo chu kỳ
    void setupSerialPort();             // Cài đặt cổng COM
    void updateTextBrowser(const QString &data); // Hiển thị dữ liệu vào textBrowser
    void updateTextBrowser1(quint32 canId, quint8 dlc, const QByteArray &data); // Hiển thị dữ liệu vào textBrowser
    void sendCanData(const QByteArray &data);
    void sendCanFrame();

};
#endif // MAINWINDOW_H

