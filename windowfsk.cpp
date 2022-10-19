#include "windowfsk.h"
#include "ui_windowfsk.h"
#include "fsk_tx.h"
#include "fsk_rx.h"
#include <QDebug>
#include "wake2.h"
#include <volk/volk.h>

WindowFSK::WindowFSK(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::WindowFSK)
{
    ui->setupUi(this);

    initGraphic(ui->widget_otputGraph);
    initGraphic(ui->widget_plotFreqDemod);
    initGraphic(ui->widget_plotPower);

    ui->comboBox_modulation->addItem("2FSK", 0);
    ui->comboBox_modulation->addItem("4FSK", 1);
//    ui->comboBox_modulation->addItem("4GFSK", 3);
//    ui->comboBox_modulation->addItem("8FSK", 4);
//    ui->comboBox_modulation->addItem("8GFSK", 5);
//    ui->comboBox_modulation->addItem("QPSK", QPSK);
//    ui->comboBox_modulation->addItem("QAM16", QAM16);
//    ui->comboBox_modulation->addItem("QAM64", QAM64);
//    ui->comboBox_modulation->addItem("QAM256", QAM256);

//    lv_32fc_t * signal;
//    unsigned int alignment = volk_get_alignment();
//    signal = (lv_32fc_t*)volk_malloc(sizeof(lv_32fc_t)*(10), alignment);
//    float* magnitude = (float*)volk_malloc(sizeof(float)*10, alignment);

//    signal[0] = lv_cmake(100, 100);
//    signal[1] = lv_cmake(10, 10);
//    signal[2] = lv_cmake(2, 4);
//    signal[3] = lv_cmake(8, 93);

//    volk_32fc_magnitude_squared_32f(magnitude, signal, 4);

//    for(int i = 0; i < 4; i ++)
//    {
//        qDebug() << magnitude[i];
//    }

//    volk_free(magnitude);
//    volk_free(signal);

//    qDebug() << sizeof(lv_16sc_t);

//    lv_16sc_t mass[4];

//    int16_t test[8] = {8,-15,
//                       64,93,
//                       -5,60,
//                      12,87};

//    memcpy(mass, test, sizeof(lv_16sc_t)*4);

//    for(int i = 0; i < 4; i++)
//        qDebug() << mass[i].real() << mass[i].imag();

//    qDebug() << sizeof(MyComplex);

}

WindowFSK::~WindowFSK()
{
    delete ui;
}

void WindowFSK::initGraphic(QWidget *widget)
{
    QCustomPlot *customPlot = (QCustomPlot *)widget;
    // set some pens, brushes and backgrounds:
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
//    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
//    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
//    QLinearGradient axisRectGradient;
//    axisRectGradient.setStart(0, 0);
//    axisRectGradient.setFinalStop(0, 350);
//    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
//    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
//    customPlot->axisRect()->setBackground(axisRectGradient);
    // Note: we could have also just called customPlot->rescaleAxes(); instead
    // Allow user to drag axis ranges with mouse, zoom with mouse wheel and select graphs by clicking:
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
}

void WindowFSK::testGenData()
{
    QVector<MyComplex> vcoData, _2FSK_data;

    for(int i = 0; i < 1000; i++)
    {
        if(i < 200)
            vcoData.append(vco(3.6));
        else if(i < 500)
            vcoData.append(vco(7.2));
        else if(i < 700)
            vcoData.append(vco(-5.3));
        else
            vcoData.append(vco(3.0));
    }

    uint8_t tmpData = 0x7A;

    _2FSK_data.resize(getTx2FSK_size(1, USE_SPS));
    tx2FSK(&tmpData, 1, USE_SPS, _2FSK_data.data());

    QVector<double> real, image, x;

    vcoData.append(_2FSK_data);

    for(int i = 0; i < vcoData.length(); i++)
    {
        x.append(i*1.0);
        real.append(vcoData[i].real);
        image.append(vcoData[i].image);
    }

    uint8_t dataRes[256] = {0};

    MyComplex tmp = {183, 691};
    for(int i = 0; i < 100; i++)
        vcoData.prepend(tmp);
#if USE_VOLK
    FSK_RxStruct rxData;
    rxData.sizeSignal = vcoData.size();
    unsigned int alignment = volk_get_alignment();
    rxData.signal = (lv_32fc_t*)volk_malloc(sizeof(lv_32fc_t)*(rxData.sizeSignal + 1), alignment);
    rxData.signal_xConj = (lv_32fc_t*)volk_malloc(sizeof(lv_32fc_t)*(rxData.sizeSignal), alignment);
    rxData.freq = (float*)volk_malloc(sizeof(float)*(rxData.sizeSignal), alignment);
    rxData.power = (float*)volk_malloc(sizeof(float)*(rxData.sizeSignal), alignment);
    rxData.signal_i16 = (lv_16sc_t*)volk_malloc(sizeof(lv_16sc_t)*(rxData.sizeSignal + 1), alignment);

    rxData.oldData = lv_cmake(0, 0);

    rxData.signal_i16[0] = rxData.oldData;
    rxData.signal[0] = rxData.oldData;
    memcpy(&rxData.signal_i16[1], vcoData.data(), sizeof(lv_16sc_t)*rxData.sizeSignal);
    volk_16ic_convert_32fc(rxData.signal, rxData.signal_i16, rxData.sizeSignal + 1);


//    for(uint32_t i = 0; i < rxData.sizeSignal; i++)
//    {
//        rxData.signal[i + 1] = lv_cmake(vcoData[i].real, vcoData[i].image);
//    }
    uint32_t resSize = rx2FSK(&rxData, USE_SPS, dataRes);
#else
    uint32_t resSize = rx2FSK(vcoData.data(), vcoData.size(), USE_SPS, dataRes);
#endif

    ui->textEdit->append("testGenData");
    ui->textEdit->append("Tx data = 0x" + QString::number(tmpData, 16).rightJustified(2, '0'));
    ui->textEdit->append("Rx size = 0x" + QString::number(resSize, 16).rightJustified(2, '0'));
    ui->textEdit->append("Rx data = 0x" + QString::number(dataRes[0], 16).rightJustified(2, '0'));
    ui->textEdit->append("\n");


    ui->widget_otputGraph->clearGraphs();

    ui->widget_otputGraph->addGraph();
    ui->widget_otputGraph->addGraph();
    ui->widget_otputGraph->graph(0)->setData(x, real);
    ui->widget_otputGraph->graph(1)->setData(x, image);

    ui->widget_otputGraph->graph(0)->rescaleAxes();
    ui->widget_otputGraph->graph(1)->rescaleAxes(true);
    ui->widget_otputGraph->graph(1)->setPen(QPen(QColor(255, 110, 40)));

    ui->widget_otputGraph->replot();

    testFreqDetector(vcoData);

#if USE_VOLK
    volk_free(rxData.freq);
    volk_free(rxData.power);
    volk_free(rxData.signal);
    volk_free(rxData.signal_xConj);
    volk_free(rxData.signal_i16);
#endif
}

void WindowFSK::testGenFrame()
{
//    wakePacket.resize(ui->spinBox_symbolsData->value()*1.1 + 100);
//    uint16_t sizePacket = genPaccketWake(0x55, testData.data(), ui->spinBox_symbolsData->value(), wakePacket.data());

//    wakePacket.resize(sizePacket);

    //no wake2
    wakePacket = testData.mid(0, ui->spinBox_symbolsData->value());
    uint16_t sizePacket = wakePacket.size();

    if(mod == _2FSK)
    {
        txFrame.resize(getTx2FSK_size(sizePacket, USE_SPS));

        tx2FSK(wakePacket.data(), sizePacket, USE_SPS, txFrame.data());
    }
    else if(mod == _4FSK)
    {
        txFrame.resize(getTx4FSK_size(sizePacket, USE_SPS));

        tx4FSK(wakePacket.data(), sizePacket, USE_SPS, txFrame.data());
    }

//    txFrame.append(txFrame);

    MyComplex tmp = {8192, 0};

    for(int i = 0 ; i < 100; i++)
    {
        txFrame.prepend(tmp);
        txFrame.append(tmp);
    }

    qDebug() << "Tx frame size:" << txFrame.size();

}

inline void repeater(MyComplex *vcoData, MyComplex *repeatData, int sizeData, int repaatTime)
{
    for(int i = 0; i < sizeData; i++)
    {
        repeatData[i*repaatTime] = vcoData[i];
    }
}

void WindowFSK::testSpeed()
{
    QVector<MyComplex> vcoData, vcoDataRepat;
    uint8_t dataRess[4096];

    MyComplex tmp;
    tmp.image = 153;
    tmp.real = 268;

    ui->textEdit->append("testSpeed");

    QTime timer(QTime::currentTime());
    if(mod == _2FSK)
    {
        vcoData.resize(getTx2FSK_size(ui->spinBox_symbolsData->value(), USE_SPS)*4);
        vcoDataRepat.resize(getTx2FSK_size(ui->spinBox_symbolsData->value(), USE_SPS)*4);

        memset(vcoDataRepat.data(), 0, vcoDataRepat.size()*sizeof(MyComplex));
        for(int i = 0; i < 100; i++)
        {
            tx2FSK(testData.data(), ui->spinBox_symbolsData->value(), USE_SPS, vcoData.data(), 3);
//            repeater(vcoData.data(), vcoDataRepat.data(), vcoData.size(), 4);
        }

        vcoData.resize(getTx2FSK_size(ui->spinBox_symbolsData->value(), USE_SPS));
        tx2FSK(testData.data(), ui->spinBox_symbolsData->value(), USE_SPS, vcoData.data(), 1);
    }
    else if(mod == _4FSK)
    {
        vcoData.resize(getTx4FSK_size(ui->spinBox_symbolsData->value(), USE_SPS));
        for(int i = 0; i < 100; i++)
        {
            tx4FSK(testData.data(), ui->spinBox_symbolsData->value(), USE_SPS, vcoData.data());
        }
    }

    int ms = timer.elapsed();
    ui->textEdit->append(QString("Time use for 100 transmitte %1 ms").arg(ms));
//    qDebug() << "Time use for 100 transmitte " << ms << "ms";
    for(int i = 0; i < 1000; i++)
        vcoData.prepend(tmp);

    int ms_1000, ms_11000;

#if USE_VOLK
    FSK_RxStruct rxData;
    rxData.sizeSignal = vcoData.size();
    unsigned int alignment = volk_get_alignment();
    rxData.signal = (lv_32fc_t*)volk_malloc(sizeof(lv_32fc_t)*(rxData.sizeSignal + 1), alignment);
    rxData.signal_xConj = (lv_32fc_t*)volk_malloc(sizeof(lv_32fc_t)*(rxData.sizeSignal), alignment);
    rxData.freq = (float*)volk_malloc(sizeof(float)*(rxData.sizeSignal), alignment);
    rxData.power = (float*)volk_malloc(sizeof(float)*(rxData.sizeSignal), alignment);
    rxData.signal_i16 = (lv_16sc_t*)volk_malloc(sizeof(lv_16sc_t)*(rxData.sizeSignal + 1), alignment);
#endif

    for(int it = 0; it < 2; it++)
    {

#if USE_VOLK
    rxData.oldData = lv_cmake(0, 0);

    rxData.signal_i16[0] = rxData.oldData;
    rxData.signal[0] = rxData.oldData;
    memcpy(&rxData.signal_i16[1], vcoData.data(), sizeof(lv_16sc_t)*rxData.sizeSignal);
    volk_16ic_convert_32fc(rxData.signal, rxData.signal_i16, rxData.sizeSignal + 1);


//    for(uint32_t i = 0; i < rxData.sizeSignal; i++)
//    {
//        rxData.signal[i + 1] = lv_cmake(vcoData[i].real, vcoData[i].image);
//    }
#endif

    ms = timer.elapsed();

    if(mod == _2FSK)
    {
        for(int i = 0; i < 100; i++)
        {
#if USE_VOLK
            rx2FSK(&rxData, USE_SPS, dataRess);
#else
            rx2FSK(vcoData.data(), vcoData.size(), USE_SPS, dataRess);
#endif
        }
    }
    else if(mod == _4FSK)
    {
        for(int i = 0; i < 100; i++)
        {
            rx4FSK(vcoData.data(), vcoData.size(), USE_SPS, dataRess);
        }
    }
    ms = timer.elapsed() - ms;

    ui->textEdit->append(QString("Time use for 100 recive %1 ms").arg(ms));

    if(it == 0)
    {
        ms_1000 = ms;
        for(int i = 0; i < 10000; i++)
            vcoData.prepend(tmp);
    }
    ms_11000 = ms;
    }
    ui->textEdit->append(QString("Time use for 100 sinhrinizations %1 ms").arg(ms_11000 - ms_1000));
//    qDebug() << "Time use for 100 recive " << ms << "ms";
    ui->textEdit->append("\n");

#if USE_VOLK
    volk_free(rxData.freq);
    volk_free(rxData.power);
    volk_free(rxData.signal);
    volk_free(rxData.signal_xConj);
    volk_free(rxData.signal_i16);
#endif

}

void WindowFSK::testRecive(QVector<MyComplex> data)
{
    uint8_t dataRess[8192];
    uint32_t resSize = 0;

#if USE_VOLK
    FSK_RxStruct rxData;
    rxData.sizeSignal = data.size();
    unsigned int alignment = volk_get_alignment();
    rxData.signal = (lv_32fc_t*)volk_malloc(sizeof(lv_32fc_t)*(rxData.sizeSignal + 1), alignment);
    rxData.signal_xConj = (lv_32fc_t*)volk_malloc(sizeof(lv_32fc_t)*(rxData.sizeSignal), alignment);
    rxData.freq = (float*)volk_malloc(sizeof(float)*(rxData.sizeSignal), alignment);
    rxData.power = (float*)volk_malloc(sizeof(float)*(rxData.sizeSignal), alignment);
    rxData.signal_i16 = (lv_16sc_t*)volk_malloc(sizeof(lv_16sc_t)*(rxData.sizeSignal + 1), alignment);

    rxData.oldData = lv_cmake(0, 0);

    rxData.signal_i16[0] = rxData.oldData;
    rxData.signal[0] = rxData.oldData;
    memcpy(&rxData.signal_i16[1], data.data(), sizeof(lv_16sc_t)*rxData.sizeSignal);
    volk_16ic_convert_32fc(rxData.signal, rxData.signal_i16, rxData.sizeSignal + 1);


//    for(uint32_t i = 0; i < rxData.sizeSignal; i++)
//    {
//        rxData.signal[i + 1] = lv_cmake(vcoData[i].real, vcoData[i].image);
//    }
#endif

    if(mod == _2FSK)
    {
#if USE_VOLK
        resSize = rx2FSK(&rxData, USE_SPS, dataRess);
#else
        resSize = rx2FSK(data.data(), data.size(), USE_SPS, dataRess);
#endif
    }
    else if(mod == _4FSK)
    {
        resSize = rx4FSK(data.data(), data.size(), USE_SPS, dataRess);
    }

    int errors = 0;
    uint8_t err;
    for(int i = 0; i < resSize; i++)
    {
        if(i == wakePacket.size())
            break;

        err = wakePacket.at(i) ^ dataRess[i];
        for(int j = 0; j < 8; j++)
            errors += (err >> j) & 0x01;

    }


    ui->textEdit->append("testRecive");
    ui->textEdit->append("Tx size = 0x" + QString::number(ui->spinBox_symbolsData->value(), 16).rightJustified(4, '0') + \
                         "/" + QString::number(ui->spinBox_symbolsData->value(), 10));
    ui->textEdit->append("Rx size = 0x" + QString::number(resSize, 16).rightJustified(4, '0') + \
                         "/" + QString::number(resSize, 10));
    ui->textEdit->append(QString::asprintf("Error bits %d/%d", errors, resSize*8));
    ui->textEdit->append("\n");

#if USE_VOLK
    volk_free(rxData.freq);
    volk_free(rxData.power);
    volk_free(rxData.signal);
    volk_free(rxData.signal_xConj);
    volk_free(rxData.signal_i16);
#endif
}

void WindowFSK::testFreqDetector(QVector<MyComplex> data)
{
    QVector<float> resPhase;
    for(int i = 0; i < data.size(); i++)
    {
        resPhase.append(freqDetector(data[i]));
//        resPhase.append(data[i].real*data[i].real + data[i].image*data[i].image);
    }

    QVector<double> X, Y;
    for(int i = 0; i < data.size(); i++)
    {
        X.append(i);
        Y.append(resPhase[i]);
    }


    ui->widget_plotFreqDemod->clearGraphs();

    ui->widget_plotFreqDemod->addGraph();
    ui->widget_plotFreqDemod->graph(0)->setData(X, Y);

    ui->widget_plotFreqDemod->graph(0)->rescaleAxes();

    ui->widget_plotFreqDemod->replot();
}

void WindowFSK::testPower(QVector<MyComplex> data)
{
    QVector<float> resPower;
    for(int i = 0; i < data.size(); i++)
    {
        resPower.append(data[i].real*data[i].real + data[i].image*data[i].image);
    }

    QVector<double> X, Y;
    for(int i = 0; i < data.size(); i++)
    {
        X.append(i);
        Y.append(resPower[i]);
    }


    ui->widget_plotPower->clearGraphs();

    ui->widget_plotPower->addGraph();
    ui->widget_plotPower->graph(0)->setData(X, Y);

    ui->widget_plotPower->graph(0)->rescaleAxes();

    ui->widget_plotPower->replot();
}

void WindowFSK::on_pushButton_modulate_clicked()
{
//    testGenData();
    if(ui->comboBox_modulation->currentIndex() == 0)
        mod = _2FSK;
    else
        mod = _4FSK;
    testGenFrame();
}

void WindowFSK::on_pushButton_tests_clicked()
{
    // /home/user/Projects//FPGA/Lib_myOFDM/testBanch/testDataOFDM.txt
    testRecive(txFrame);

    testFreqDetector(txFrame);
    testPower(txFrame);
    testSpeed();
}

void WindowFSK::on_pushButton_openData_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "Data file", ui->lineEdit_fileData->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileData->setText(file);
    }
}

void WindowFSK::on_pushButton_readData_clicked()
{
    QFile fileRealData(ui->lineEdit_fileData->text());

    if(fileRealData.open(QIODevice::ReadOnly))
    {
        testData.clear();
        while(!fileRealData.atEnd())
        {
            QByteArray readData = fileRealData.readLine();

            readData = readData.mid(0, readData.length() - 1);//remove \n
            testData.append(readData.toUInt());
        }

        fileRealData.close();
        qDebug() << "read done" << testData.size();

        uint32_t sizeSymbols = testData.size();
        ui->spinBox_symbolsData->setMaximum(sizeSymbols);
    }
    else
    {
        qDebug() << "couldn't open file" << fileRealData.fileName();
        QMessageBox::critical(this, "Open file", "Can't open file " + fileRealData.fileName());
    }
}

void WindowFSK::on_pushButton_openSaveFile_i_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "File data I", ui->lineEdit_fileSaveData_i->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileSaveData_i->setText(file);
    }
}

void WindowFSK::on_pushButton_openSaveFile_q_clicked()
{
    QString file = QFileDialog::getSaveFileName(this, "File data Q", ui->lineEdit_fileSaveData_q->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileSaveData_q->setText(file);
    }
}

void WindowFSK::on_pushButton_saveModData_clicked()
{
    QFile fileData_i(ui->lineEdit_fileSaveData_i->text());

    if(fileData_i.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < txFrame.size(); i++)
        {
            int32_t data = txFrame[i].real;
            uint8_t dara[4] = {data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF};
            fileData_i.write((char*)dara, 4);
        }
    }
    else
    {
        QMessageBox::critical(this, "File write I data", "Can't open file " + fileData_i.fileName());
    }

    QFile fileData_q(ui->lineEdit_fileSaveData_q->text());

    if(fileData_q.open(QIODevice::WriteOnly))
    {
        for(int i = 0; i < txFrame.size(); i++)
        {
            int32_t data = txFrame[i].image;
            uint8_t dara[4] = {data & 0xFF, (data >> 8) & 0xFF, (data >> 16) & 0xFF, (data >> 24) & 0xFF};
            fileData_q.write((char*)dara, 4);
        }
    }
    else
    {
        QMessageBox::critical(this, "File write Q data", "Can't open file " + fileData_q.fileName());
    }
}

void WindowFSK::on_pushButton_openResiveFile_i_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "File data I", ui->lineEdit_fileResiveData_i->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileResiveData_i->setText(file);
    }
}

void WindowFSK::on_pushButton_openResiveFile_q_clicked()
{
    QString file = QFileDialog::getOpenFileName(this, "File data Q", ui->lineEdit_fileResiveData_q->text(), tr("Text data (*.txt);;All file (*)"));
    if(!file.isEmpty())
    {
        ui->lineEdit_fileResiveData_q->setText(file);
    }
}

void WindowFSK::on_pushButton_testReciveData_clicked()
{
    QVector <MyComplex> readFrameFSK;
    MyComplex tmp;

    QFile fileData_i(ui->lineEdit_fileResiveData_i->text());
    QFile fileData_q(ui->lineEdit_fileResiveData_q->text());


    if(fileData_i.open(QIODevice::ReadOnly) && fileData_q.open(QIODevice::ReadOnly))
    {
        while((!fileData_i.atEnd()) && (!fileData_i.atEnd()))
        {
//            uint8_t dara[4] = {/*data & 0xFF, (data >> 8) & 0xFF*/0};
//            fileData_i.read((char*)dara, 4);
            QByteArray daraB = fileData_i.read(4);
            int32_t data = (uint8_t)daraB[0] | (((uint8_t)daraB[1]) << 8) | (((uint8_t)daraB[2]) << 16) | (((uint8_t)daraB[3]) << 24);
            tmp.real = data >= 0 ? data : data | 0xFFFFFFFF00000000;

//            fileData_q.read((char*)dara, 4);
            daraB = fileData_q.read(4);
            data = (uint8_t)daraB[0] | (((uint8_t)daraB[1]) << 8) | (((uint8_t)daraB[2]) << 16) | (((uint8_t)daraB[3]) << 24);
            tmp.image = data >= 0 ? data : data | 0xFFFFFFFF00000000;


            readFrameFSK.append(tmp);

            if(readFrameFSK.size() == 250000)
                break;
        }

        fileData_i.close();
        fileData_q.close();
    }
    else
    {
        QMessageBox::critical(this, "File read I Q data", "Can't open file");
        return ;
    }

    ui->textEdit->append("fromFile");
    testRecive(readFrameFSK);
    testFreqDetector(readFrameFSK);
    testPower(readFrameFSK);
}
