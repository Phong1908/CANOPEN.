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

private slots:
    void on_pushButton_STOP_released();

    void on_pushButton_RUN_released();
    void on_pushButton_readData_clicked(); // Đọc dữ liệu
    void on_pushButton_clear_clicked();   // Xóa dữ liệu
    void readCanData();                    // Đọc dữ liệu từ Serial

private:
    Ui::MainWindow *ui;
    QSerialPort *Serialll;                 // Serial port object
    QTimer *timer;                         // Đọc dữ liệu theo chu kỳ
    void setupSerialPort();             // Cài đặt cổng COM
    void updateTextBrowser(const QString &data); // Hiển thị dữ liệu vào textBrowser
};
#endif // MAINWINDOW_H

