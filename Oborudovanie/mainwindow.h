#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QUdpSocket>
#include <QTimer>
#include <QTime>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    QUdpSocket* m_socket;
    ~MainWindow();
    int t1=0,t2=0,t3=0,t4=0,HP=0;
    long long MinTime=99999999999999,Time, MinTime1=99999999999999, MinTime2=99999999999999;
    QString TimeMessage,dt,min;
    bool u1=false,u2=false,u3=false,u4=false;

private slots:
    void on_pB_port1Open_clicked();

    void on_pB_port1Close_clicked();

    void on_pB_port1Send_clicked();
    void ReadyRead();

    void slotTimerAlarm();
    void slotTmrAlarm();

    void on_pB_port1Stop_clicked();
    void on_pushButton_clicked();
    void on_pB_port1Send_2_clicked();

    void on_pB_port1Send_3_clicked();

private:
    Ui::MainWindow *ui;
    QTimer * MyTimer;
    QTimer *timer;
    QTimer *tmr;
};
#endif // MAINWINDOW_H
