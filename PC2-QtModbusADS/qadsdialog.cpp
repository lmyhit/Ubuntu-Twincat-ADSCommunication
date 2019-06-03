/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * QtADS Library
 * Copyright (C) 2012-2018   Framatome, Inc.
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *****************************************************************************/
#include "qadsdialog.h"
#include "ui_qadsdialog.h"
#include <QtTest>
#include <QADSENUM>
#include <QADSTIME>
#include <QADSLREAL>
#include <QModbusTcpServer>
#include <QModbusDevice>
#include <QUrl>
#include <QDebug>
#define MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT  {\
    QTextCursor cursor = ui->textEdit->textCursor(); \
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor, 1); \
    }

#define ENTER_DIAGNOSTIC_ERROR(input) {\
    ui->textEdit->setTextColor(Qt::red); \
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT; \
    ui->textEdit->insertPlainText(QString("%1. %2\n").arg(m_textEditMessageCount++) \
                                                .arg(input->adsErrorString())); \
    ui->textEdit->setTextColor(Qt::black); \
    }

QAdsDialog::QAdsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::QAdsDialog),
    m_textEditMessageCount(0),
    m_eExampleEnum(new QADSENUM(this,851, "local", "localhost","MAIN.eExampleEnum",QADSPLCVariable::ON_DEMAND,0,"EXAMPLE_ENUM")),
    m_eGlobalExampleEnum(new QADSENUM(this,851, "local", "localhost","GVL.eGlobalExampleEnum",QADSPLCVariable::ON_DEMAND,0,"EXAMPLE_ENUM")),
    m_tPassageOfTime(new QADSTIME(this,851, "local", "localhost","GVL.tPassageOfTime",QADSPLCVariable::CYCLICAL,2000000)),

    velocity(new QADSLREAL(this,851, "local", "localhost","MAIN.velocity",QADSPLCVariable::SYNCHRONIZED,0)),
    depth(new QADSLREAL(this,851, "local", "localhost","MAIN.depth",QADSPLCVariable::SYNCHRONIZED,0))

{
    ui->setupUi(this);

    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Dll Version: %2\n").arg(m_textEditMessageCount++)
                                  .arg(static_cast<int>(m_eExampleEnum->adsDllVersion())));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Dll Revision: %2\n").arg(m_textEditMessageCount++)
                                  .arg(static_cast<int>(m_eExampleEnum->adsDllRevision())));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Dll Build: %2\n").arg(m_textEditMessageCount++)
                                  .arg(m_eExampleEnum->adsDllBuild()));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. AMS Net ID: %2\n").arg(m_textEditMessageCount++)
                                  .arg(m_eExampleEnum->amsNetId()));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Port: %2\n").arg(m_textEditMessageCount++)
                                  .arg(m_eExampleEnum->adsPort()));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Device Name: %2\n").arg(m_textEditMessageCount++)
                                  .arg(m_eExampleEnum->adsDeviceName()));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Device Version: %2\n").arg(m_textEditMessageCount++)
                                  .arg(static_cast<int>(m_eExampleEnum->adsDeviceVersion())));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Device Revision: %2\n").arg(m_textEditMessageCount++)
                                  .arg(static_cast<int>(m_eExampleEnum->adsDeviceRevision())));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS Device Build: %2\n").arg(m_textEditMessageCount++)
                                  .arg(static_cast<int>(m_eExampleEnum->adsDeviceBuild())));
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS PLC State: %2\n").arg(m_textEditMessageCount++)
                                  .arg(m_eExampleEnum->adsStateAsString()));
    if( m_eExampleEnum->adsError() )
    {
        ENTER_DIAGNOSTIC_ERROR(m_eExampleEnum);
    }
    if( m_tPassageOfTime->adsError() )
    {
        ENTER_DIAGNOSTIC_ERROR(m_tPassageOfTime);
    }

    if( velocity->adsError() )
    {
        ENTER_DIAGNOSTIC_ERROR(velocity);
    }
    if( depth->adsError() )
    {
        ENTER_DIAGNOSTIC_ERROR(depth);
    }

    // Monitor state changes (only need to connect this signal once).
    connect(m_eExampleEnum, SIGNAL(adsStateChanged()),
            this, SLOT(catchADSStateChanged()));
    connect(m_eExampleEnum, SIGNAL(amsEventChanged()),
            this, SLOT(catchAMSEventChanged()));

    // Connect the error signal to the slot on this class.
    connect(m_eExampleEnum, SIGNAL(adsErrorChanged()),
            this, SLOT(catchADSErrorChanged()));
    connect(velocity, SIGNAL(adsErrorChanged()),
            this, SLOT(catchADSErrorChanged()));

    connect(depth, SIGNAL(adsErrorChanged()),
            this, SLOT(catchADSErrorChanged()));


    // Variable connections.
    connect(m_tPassageOfTime, SIGNAL(valueChanged()),
            this, SLOT(tPassageOfTimeChanged()));


    modbusDevice = new QModbusTcpServer(this);
    QModbusDataUnitMap reg;

    reg.insert(QModbusDataUnit::HoldingRegisters, { QModbusDataUnit::HoldingRegisters, 0, 10 });

    modbusDevice->setMap(reg);//把reg设置给了全局的m_modbusDataUnitMap


    const QUrl url = QUrl::fromUserInput("192.168.0.12:502");
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkPortParameter, url.port());
    modbusDevice->setConnectionParameter(QModbusDevice::NetworkAddressParameter, url.host());
    modbusDevice->setServerAddress(1);
    modbusDevice->connectDevice();
    connect(modbusDevice, &QModbusServer::dataWritten,this, &QAdsDialog::updateWidgets);
}

QAdsDialog::~QAdsDialog()
{
    delete ui;
    if (modbusDevice)
        modbusDevice->disconnectDevice();//清除
    delete modbusDevice;
}

void QAdsDialog::catchADSErrorChanged()
{
    QADSObject *temp
#ifndef Q_WS_WINCE
            = dynamic_cast<QADSObject *>(sender());
#else
            = static_cast<QADSObject *>(sender());
#endif
    ENTER_DIAGNOSTIC_ERROR(temp);
}

void QAdsDialog::catchADSStateChanged()
{
    QADSObject *temp
#ifndef Q_WS_WINCE
            = dynamic_cast<QADSObject *>(sender());
#else
            = static_cast<QADSObject *>(sender());
#endif
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. ADS PLC State Changed to: %2\n").arg(m_textEditMessageCount++)
                                  .arg(temp->adsStateAsString()));
}

void QAdsDialog::catchAMSEventChanged()
{
    QADSObject *temp
#ifndef Q_WS_WINCE
            = dynamic_cast<QADSObject *>(sender());
#else
            = static_cast<QADSObject *>(sender());
#endif
    MOVE_TO_END_OF_DIAGNOSTICS_TAB_DOCUMENT;
    ui->textEdit->insertPlainText(QString("%1. AMS Router State Changed to: %2\n").arg(m_textEditMessageCount++)
                                  .arg(temp->amsEventAsString()));
}


void QAdsDialog::tPassageOfTimeChanged()
{
    ui->timeEdit->setTime(m_tPassageOfTime->value());
}


void QAdsDialog::on_pushButton_clicked()
{
    for(int i=1;i<100;i++)
    {

        QTest::qSleep(100);
    }
}
void QAdsDialog::updateWidgets(QModbusDataUnit::RegisterType table, int address, int size)
{
    qDebug()<< "table is "<< table<< "size is "<< size<< "address is "<< address;
    for (int i = 0; i < size; ++i) {
        quint16 value;
        switch (table) {
        case QModbusDataUnit::Coils:
            modbusDevice->data(QModbusDataUnit::Coils, address + i, &value);
            qDebug()<<"Table is Coils, and the value is "<<value;
            break;
        case QModbusDataUnit::HoldingRegisters:
            modbusDevice->data(QModbusDataUnit::HoldingRegisters, address + i, &value);//得到数据后做出显示
            qDebug()<<"Table is HoldingRegisters, and the value is"<<value;
            break;
        default:
            break;
        }
        QString s = QString::number(value, 16);
       // qDebug("%d",value);
        velocity->setValue(value);
        depth->setValue(2*value);
    }
}
