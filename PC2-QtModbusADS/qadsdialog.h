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
#ifndef QADSDIALOG_H
#define QADSDIALOG_H

#include <QDialog>
#include <QModbusDataUnit>
#include <QModbusServer>
namespace Ui {
class QAdsDialog;
}
class QADSENUM;
class QADSTIME;
class QADSLREAL;


class QAdsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QAdsDialog(QWidget *parent = 0);
    ~QAdsDialog();

private slots:
    void catchADSErrorChanged();

    void catchADSStateChanged();

    void catchAMSEventChanged();
    void tPassageOfTimeChanged();

    void on_pushButton_clicked();
    void updateWidgets(QModbusDataUnit::RegisterType table, int address, int size);
private:
    Ui::QAdsDialog *ui;
    unsigned int m_textEditMessageCount;
    QADSENUM* m_eExampleEnum;
    QADSENUM* m_eGlobalExampleEnum;
    QADSTIME* m_tPassageOfTime;
    QADSLREAL* velocity;
    QADSLREAL* depth;
    QModbusServer *modbusDevice;
};

#endif // QADSDIALOG_H
