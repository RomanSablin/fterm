#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QAction>
#include <QMainWindow>
#include <QPointer>
#include <QSerialPort>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    void Ui(const bool _enable);
    bool OpenSerialPort();
    bool CloseSerialPort();
    void LogD(const QString _message);
    void Logc(const QString _str, const int _color_id);
    void Log(const QString _str);

    void SetLogColor(const QColor _color);
    void ClearColor();

    void ParsePacket();

    void UpdateAvaliablePorts();

private slots:
    void SettingsChecked();
    void OnSerialOpen();
    void OnSerialClose();
    void OnSerialReady();
    void OnWindowClear();

private:
    Ui::MainWindow *ui;
    QPointer<QAction> mBtnFtdi;
    QSerialPort *mSerialPort;
    int mBaudrate;
    bool mIsGuiPortOpen;
    QColor mEtLogstc;
    int mEtLogsfw;
    QFont mEtLogsFont;
    QByteArray readSerialArray;
    QByteArray mPacket;
    QByteArray mVt100Packet;
    QByteArray startColor;
    QByteArray stopColor;
};
#endif // MAINWINDOW_H
