#ifndef WINDOWFSK_H
#define WINDOWFSK_H

#include <QMainWindow>
#include "common_fsk.h"

#define USE_SPS     4

namespace Ui {
class WindowFSK;
}

class WindowFSK : public QMainWindow
{
    Q_OBJECT

public:
    explicit WindowFSK(QWidget *parent = 0);
    ~WindowFSK();

private slots:
    void on_pushButton_modulate_clicked();

    void on_pushButton_openData_clicked();

    void on_pushButton_readData_clicked();

    void on_pushButton_openSaveFile_i_clicked();

    void on_pushButton_openSaveFile_q_clicked();

    void on_pushButton_saveModData_clicked();

    void on_pushButton_openResiveFile_i_clicked();

    void on_pushButton_openResiveFile_q_clicked();

    void on_pushButton_testReciveData_clicked();

    void on_pushButton_tests_clicked();

private:
    void initGraphic(QWidget *widget);
    void testGenData();
    void testGenFrame();
    void testSpeed();
    void testRecive(QVector<MyComplex> data);
    void testFreqDetector(QVector<MyComplex> data);
    void testPower(QVector<MyComplex> data);

private:
    Ui::WindowFSK *ui;
    QVector <uint8_t> testData;
    QVector<MyComplex> txFrame, rxFrame;
    QVector <uint8_t> wakePacket;
    Modulation mod;
};

#endif // WINDOWFSK_H
