#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QSerialPortInfo>

#include "parameters.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , mSerialPort(nullptr)
    , mBaudrate(115200)
    , mIsGuiPortOpen(false)
{
    ui->setupUi(this);
    setWindowTitle( QCoreApplication::applicationName() );
    setWindowIcon(QIcon(":/images/fterm.png"));

    const auto infos = QSerialPortInfo::availablePorts();
    for (const QSerialPortInfo &info : infos)
    {
        if(CParameters::IsUSBFilterEnabled)
        {
            if(info.vendorIdentifier() == CParameters::FTDI_VENDOR_ID)
                ui->cbPort->addItem(info.portName());
        }
        else
        {
            ui->cbPort->addItem(info.portName());
        }
    }
    static const int bauds[] = {
        9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600
    };

    int i=0;
    for (const auto &n : bauds)
    {
        ui->cbBaudrate->addItem(QString::number(n));
        if (n == mBaudrate)
            ui->cbBaudrate->setCurrentIndex(i);

        i++;
    }

    connect(ui->pbOpen, &QPushButton::clicked, this, &MainWindow::OnSerialOpen);
    connect(ui->pbClose, &QPushButton::clicked, this, &MainWindow::OnSerialClose);
    connect(ui->pbClear, &QPushButton::clicked, this, &MainWindow::OnWindowClear);

    mEtLogsFont = ui->etLogs->currentFont ();
    ui->etLogs->setReadOnly(true);
    mEtLogsfw = ui->etLogs->fontWeight();
    mEtLogstc = ui->etLogs->textColor();
    mEtLogsFont = ui->etLogs->currentFont ();

    QPalette p = ui->etLogs->palette();
    p.setColor(QPalette::Base, Qt::black);
    ui->etLogs->setPalette(p);
    Ui(false);
}

MainWindow::~MainWindow()
{
    OnSerialClose();
    delete ui;
}

void MainWindow::OnSerialOpen()
{
    QString port = ui->cbPort->currentText();
    ui->etLogs->clear();
    ui->etLogs->setText("");
    ui->etLogs->setCurrentFont (mEtLogsFont);
    mSerialPort = new QSerialPort();
    mSerialPort->setPortName(port);
    mBaudrate = ui->cbBaudrate->currentText().toInt();
    mSerialPort->setBaudRate(mBaudrate);
    mSerialPort->setDataBits(QSerialPort::DataBits::Data8);
    mSerialPort->setParity(QSerialPort::Parity::NoParity);
    mSerialPort->setStopBits(QSerialPort::StopBits::OneStop);
    mSerialPort->setFlowControl(QSerialPort::FlowControl::NoFlowControl);

    connect(mSerialPort, &QSerialPort::readyRead, this, &MainWindow::OnSerialReady);

    ClearColor();

    if(OpenSerialPort())
    {
        Ui(true);
    }
}

void MainWindow::OnSerialClose()
{
    CloseSerialPort ();
    Ui(false);
}

void MainWindow::OnSerialReady()
{
    readSerialArray.clear();
    readSerialArray.append(mSerialPort->readAll());
    uint8_t * byteArray = reinterpret_cast<uint8_t *>(readSerialArray.data());
    for(int i=0; i<readSerialArray.length(); i++)
    {
        if(mPacket.size() > 0 && (byteArray[i] == 0x0a || 0x0d == byteArray[i]))
        {
            mPacket.append("\r\n");

            ParsePacket();
            Log(mVt100Packet);

            mPacket.clear();
        }
        else
        {
            if(byteArray[i] == 0)
            {
                if(mPacket.size() > 0)
                {
                    mPacket.append("\r\n");
                    ParsePacket();

                    Log(mVt100Packet);

                    mPacket.clear();
                }
            }
            else
            {
                mPacket.append(byteArray[i]);
            }
        }
    }
}

void MainWindow::OnWindowClear()
{
    ui->etLogs->clear();
}

void MainWindow::Ui(const bool _enable)
{
    ui->pbOpen->setEnabled(ui->cbPort->count() > 0 ? !_enable : false);
    ui->cbPort->setEnabled(!_enable);
    ui->pbClose->setEnabled(_enable);
}

bool MainWindow::OpenSerialPort()
{
    if(mIsGuiPortOpen)
        return false;

    if(mSerialPort == nullptr)
        return false;

    bool status = false;
    mPacket.clear();

    if( !mSerialPort->open(QIODevice::ReadWrite) )
    {
        Log("Fail: Can't open port " + ui->cbPort->currentText());
    }
    else
    {
        status = true;
    }
    mSerialPort->setFlowControl (QSerialPort::NoFlowControl);
    mIsGuiPortOpen = status;
    return status;
}

bool MainWindow::CloseSerialPort()
{
    if(mIsGuiPortOpen)
    {
        if(mSerialPort != nullptr)
        {
            if (mSerialPort->isOpen())
            {
                mSerialPort->close();
                mIsGuiPortOpen = false;
            }
        }
    }

    return true;
}

void MainWindow::LogD(const QString _message)
{
    qDebug() << ": "+ _message;
}

void MainWindow::Logc(const QString _str, const int _color_id)
{
    SetLogColor (CParameters::GetColor (_color_id));
    ui->etLogs->append(_str);
    ui->etLogs->show ();

    ClearColor();
}

void MainWindow::Log(const QString _str)
{
    if(_str.size () == 0)
        return;

    QString filter = ui->etFilter->text();

    if (filter.size() >0)
    {
        if (filter.contains("&"))
        {
            QStringList fs = filter.split("&");
            for (const auto &s : fs)
            {
                if (_str.contains(s, Qt::CaseInsensitive))
                {
                    ui->etLogs->append(_str);
                    break;
                }
            }
        }
        else
        {
            if(_str.contains(filter, Qt::CaseInsensitive))
                ui->etLogs->append(_str);
        }
    }
    else
        ui->etLogs->append(_str);
}

void MainWindow::SetLogColor(const QColor _color)
{
    ui->etLogs->setFontWeight( QFont::DemiBold );
    ui->etLogs->setTextColor( _color );
}

void MainWindow::ClearColor()
{
    ui->etLogs->setFontWeight( mEtLogsfw );
    ui->etLogs->setTextColor( mEtLogstc );
}

void MainWindow::ParsePacket()
{
    int start = 0;
    int stop = 0;
    int j=0;
    uint8_t * byteArray = reinterpret_cast<uint8_t *>(mPacket.data());
    size_t psize = mPacket.size();
    mVt100Packet.clear();
    startColor.clear();
    stopColor.clear();

    for(int i=0; i<psize; i++)
    {
        if(i == 0 && byteArray[0] == 0x0a && byteArray[1] == 0x44) // \nD
        {
            startColor = "[0;30m";
            start = 3;
            i+=2;
        }
        else if(byteArray[i] == 0x1b && (psize - i) > 3)
        {
            if(start == 0)
            {
                j++;
            }
            else
            {
                stop = i - 1;
                j++;
            }
        }
        else if(j > 0)
        {
            if(start == 0)
                startColor.append(mPacket[i]);
            else
                stopColor.append(mPacket[i]);

            if(byteArray[i] == 0x20)
            {
                if(start == 0)
                    start = i + 1;

                j=0;
            }
        }
    }
    if(stop > start)
    {
        mVt100Packet.append(mPacket.mid(start, stop - start + 1));
        LogD(mVt100Packet);
        SetLogColor (CParameters::GetColor (startColor));
    }
}
