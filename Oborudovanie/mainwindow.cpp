#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <QString>
#include <QTextStream>
#include <QDate>
#include <QTime>
#include <time.h>
#include <QDateTime>
#include <windows.h>
#include <QVector>

using namespace std;

vector <QString> baza;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    MyTimer = new QTimer(this);
    connect(MyTimer, SIGNAL(timeout()),this,SLOT(on_pB_port1Send_clicked()));
    ui->setupUi(this);
    m_socket = new QUdpSocket(this);
    connect(m_socket,&QUdpSocket::readyRead,this,&MainWindow::ReadyRead);

    timer=new QTimer(this);
    tmr=new QTimer(this);
    connect(timer,&QTimer::timeout,this,&MainWindow::slotTimerAlarm);
    connect(tmr,&QTimer::timeout,this,&MainWindow::slotTmrAlarm);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pB_port1Open_clicked()
{
    QString Port;
     bool result;

    //считываем IP-адрес своего компьютера
     QHostAddress Address(QHostAddress::LocalHost);
    //пытаемся открыть сокет
     result=m_socket->bind(Address, 0001); // Порт оборудования
    //если открыли успешно, то вывод сообщения
     if(result)
     ui->lb_port1Status->setText("Открыт");
     else
     ui->lb_port1Status->setText("Ошибка!");
}


void MainWindow::on_pB_port1Close_clicked()
{
    m_socket->close();
     ui->lb_port1Status->setText("Закрыт");
}


void MainWindow::on_pB_port1Send_clicked()
{
    QString RemotePort;
     QByteArray mass;
     QString message, str;
    QString delay, delay2;

     //взяли IP-адрес компьютера, которому хотим отправить сообщение
      QHostAddress RemoteAddress(QHostAddress::LocalHost);

     using namespace std;

     const int N = 1; //Количество испытаний
     srand(time(0));
     QString text;

     double A = 1., B = 100.;
     double d[N];// диапазон

    for(int i = 0; i < N; i++)
    {

        d[i] = A + (double)rand() / RAND_MAX * (B - A);
         text += QString::number(d[i]) + " ";

    }

   // baza.pusk_back(text);

    ui->lE_sendingMessage->setText(text);

    message=ui->lE_sendingMessage->text();
    mass = message.toUtf8();

    delay = ui -> Delay -> text();
    delay2 = ui -> Delay_2 -> text();
    time_t now = time(0);
    // Преобразуем в структуру tm для локальной временной зоны
    tm* localtm = localtime(&now);
   // str = asctime(localtm);

   // ui->pTE_port1Recieved->appendPlainText(str);
    QString next, null;
    next = "----------------------------------------------------------------------------------------------------------";

    //str ="Sended to "+RemoteAddress.toString()+": "+message;
    //ui->pTE_port1Recieved->appendPlainText(str);
    //ui->pTE_port1Recieved->appendPlainText(next);;

// тут будет условие для выбора узла
    float D1, D2;
    int Port1 = 1010, Port2 = 2020;
    D1 = 1000*delay.toFloat();
    D2 = 1000*delay2.toFloat();
    if (HP==1)
    {
        if(u1 && u3)
        {
            Sleep(D1);
            m_socket->writeDatagram(mass, RemoteAddress, Port1);
            str = asctime(localtm);
            ui->pTE_port1Recieved->appendPlainText(str);
            str ="Sended to "+QString::number(Port1)+": "+message;
            ui->pTE_port1Recieved->appendPlainText(str);
            ui->pTE_port1Recieved->appendPlainText(next);;
        }
        else
        {
            Sleep(D2);
            m_socket->writeDatagram(mass, RemoteAddress, Port2);
            str = asctime(localtm);
            ui->pTE_port1Recieved->appendPlainText(str);
            str ="Sended to "+QString::number(Port2)+": "+message;
            ui->pTE_port1Recieved->appendPlainText(str);
            ui->pTE_port1Recieved->appendPlainText(next);;
        }
    }
    else if(HP==2)
    {
        if(u2 && u4)
        {
            Sleep(D1);
            m_socket->writeDatagram(mass, RemoteAddress, Port2);
            str = asctime(localtm);
            ui->pTE_port1Recieved->appendPlainText(str);
            str ="Sended to "+QString::number(Port2)+": "+message;
            ui->pTE_port1Recieved->appendPlainText(str);
            ui->pTE_port1Recieved->appendPlainText(next);;
        }
        else if(u1 && u3)
        {
            Sleep(D2);
            m_socket->writeDatagram(mass, RemoteAddress, Port1);
            str = asctime(localtm);
            ui->pTE_port1Recieved->appendPlainText(str);
            str ="Sended to "+QString::number(Port1)+": "+message;
            ui->pTE_port1Recieved->appendPlainText(str);
            ui->pTE_port1Recieved->appendPlainText(next);;
        }

        //Sleep(D2);
        //m_socket->writeDatagram(mass, RemoteAddress, Port2);
    }
    MyTimer -> start(3000);

}
void MainWindow::on_pB_port1Stop_clicked()
{
    MyTimer->stop();
}
void MainWindow::ReadyRead()
{
 QByteArray mass;
 unsigned short SenderPort;
 QHostAddress SenderAddress;
 QString ReadedData, str;
//узнать, сколько байт содержит принятый пакет и выделить под него место
 mass.resize(m_socket->pendingDatagramSize());
//считать содержимое пришедшего пакета в массив байтов
m_socket->readDatagram(mass.data(), mass.size(),&SenderAddress,&SenderPort);
if(mass.size()>0)
{
    ReadedData=QString::fromUtf8(mass);
    if(ReadedData[0]=='0' && ReadedData[1]=='x')
    {
        str = "Received from "+SenderAddress.toString()+":"+ReadedData;
        //ui->pTE_port1Recieved_2->appendPlainText(str);
        ReadedData.remove(0, 2);
        if(ReadedData[0]=='3')
        {
            ReadedData.remove(0, 1);
            if(ReadedData[0]=='t')
            {
                ReadedData.remove(0, 1);

                while(ReadedData[0]!='t')
                {
                    TimeMessage.append(ReadedData[0]);
                    ReadedData.remove(0, 1);
                }
                ReadedData.remove(0, 1);
               //ui-> Min_Time->setText(TimeMessage);
            }


            if(ReadedData[0]=='1')
            {
                ui->status_port_1->setText("OK");
                ui->status_port_3->setText("OK");
                u1=true;
                u3=true;
                t1=0;
                t3=0;
            }
            else if(ReadedData[0]=='2')
            {
                ui->status_port_2->setText("OK");
                ui->status_port_4->setText("OK");
                u2=true;
                u4=true;
                t2=0;
                t4=0;
            }
           // TimeMessage.toInt();
            //QDateTime dt=QDateTime::currentDateTime();
           // min=dt.toString();
            ui->Min_Time->setText(QString::number(QDateTime::currentMSecsSinceEpoch()));
            ui->Min_Time_3->setText(TimeMessage);

            Time=QDateTime::currentMSecsSinceEpoch()-TimeMessage.toLongLong();
            ui->Min_Time_4->setText(QString::number(MinTime));
            TimeMessage.clear();

            if(ReadedData[0]=='1')
            {
            MinTime1=Time;
            }
            else if(ReadedData[0]=='2')
            {
            MinTime2=Time;
            }
            if(MinTime1<MinTime2 && u1 && u3)
            {
            HP=1;
            ui->Opti_Time->setText(QString::number(HP));
            }
            else if(MinTime2<MinTime1 && u2 && u4)
            {
            HP=2;
            ui->Opti_Time->setText(QString::number(HP));
            }
            if(HP==1 && (!u1 || !u3))
            {
            HP=2;
            ui->Opti_Time->setText(QString::number(HP));
            }
            if(HP==2 && (!u2 || !u4))
            {
            HP=1;
            ui->Opti_Time->setText(QString::number(HP));
            }
            if(!u1 && !u2 && !u3 && !u4)
            {
            HP=0;
            }
            }
        else if(ReadedData[0]=='1')
        {
           ui->status_port_1->setText("OK");
           u1=true;
           t1=0;
        }
        else if(ReadedData[0]=='2')
        {
           ui->status_port_2->setText("OK");
           u2=true;
           t2=0;
        }
        }
    else
    {
        //преобразовать пришедшую последовательность байт в символы
         ReadedData=QString::fromUtf8(mass);
        //сформировать сообщение для показа в окне принятых и отправленных
        str = "Received from "+SenderAddress.toString()+":"+ReadedData;
        //показать отправленное сообщение в окне принятых и отправленных
    }
    }
}

void MainWindow::slotTimerAlarm()
{
    QString RemotePort;
     QByteArray mass;
     QString message, str;
    //взяли IP-адрес компьютера, которому хотим отправить сообщение
     QHostAddress RemoteAddress(QHostAddress::LocalHost);
    //взяли номер UDP-порта сокета на этом компьютере
    //преобразовали введенные пользователем символы в последовательность байт
    // message=ui->lE_sendingMessage->text();
     message="0x";
     mass = message.toUtf8();
    //отправили последовательность байт через сокет по адресу
     QString delay, delay2;
     delay = ui -> Delay -> text();
     delay2 = ui -> Delay_2 -> text();
     float D1, D2;
     int Port1 = 1010, Port2 = 2020;
     D1 = 1000*delay.toFloat();
     D2 = 1000*delay2.toFloat();
     if(D1>D2)
     {
         Sleep(D2);
         m_socket->writeDatagram(mass, RemoteAddress, Port2);
         Sleep(D1-D2);
         m_socket->writeDatagram(mass, RemoteAddress, Port1);
     }
     else
     {
         Sleep(D1);
         m_socket->writeDatagram(mass, RemoteAddress, Port1);
         Sleep(D2-D1);
         m_socket->writeDatagram(mass, RemoteAddress, Port2);
     }

    // m_socket->writeDatagram(mass, RemoteAddress,1010 );
    // m_socket->writeDatagram(mass, RemoteAddress,2020 ); //RemotePort.toInt()
    //сформировать сообщение для показа в окне принятых и отправленных
   // str = "Sended to "+RemoteAddress.toString()+":"+message;
    //показать отправленное сообщение в окне принятых и отправленных
     //ui->pTE_port1Recieved->appendPlainText(str);

}

void MainWindow::on_pushButton_clicked()
{
     timer->start(1000);
     tmr->start(1000);
}
void MainWindow::slotTmrAlarm()
{
    int t=2;
 t1++;
 t2++;
 t3++;
 t4++;
 if(t1>=t)
 {
     ui->status_port_1->setText("ERROR");
     u1=false;
 }
 if(t2>=t)
 {
     ui->status_port_2->setText("ERROR");
     u2=false;
 }
 if(t3>=t)
 {
     ui->status_port_3->setText("ERROR");
     u3=false;
 }
 if(t4>=t)
 {
     ui->status_port_4->setText("ERROR");
     u4=false;
 }
}

void MainWindow::on_pB_port1Send_2_clicked()
{
    QString RemotePort;
     QByteArray mass;
     QString message, str;
    QString delay, delay2;

     //взяли IP-адрес компьютера, которому хотим отправить сообщение
      QHostAddress RemoteAddress(QHostAddress::LocalHost);

     using namespace std;

     const int N = 1; //Количество испытаний
     srand(time(0));
     QString text;

     double A = 1., B = 100.;
     double d[N];// диапазон

    for(int i = 0; i < N; i++)
    {

        d[i] = A + (double)rand() / RAND_MAX * (B - A);

    }

    for(int i = 0; i < N; i++)
    {
     text += QString::number(d[i]) + " ";
    }

    //ui->lE_sendingMessage->setText(text);

   // message=ui->lE_sendingMessage->text();
    mass = message.toUtf8();

    delay = ui -> Delay -> text();
    delay2 = ui -> Delay_2 -> text();
    time_t now = time(0);
    // Преобразуем в структуру tm для локальной временной зоны
    tm* localtm = localtime(&now);
    str = asctime(localtm);

   // ui->pTE_port1Recieved->appendPlainText(str);
    QString next, null;
    next = "----------------------------------------------------------------------------------------------------------";

   // str ="Sended to "+RemoteAddress.toString()+": "+message;
   // ui->pTE_port1Recieved->appendPlainText(str);
    //ui->pTE_port1Recieved->appendPlainText(next);;






    float D2;
    int  Port2 = 2020;
    D2 = 1000*delay2.toFloat();
    Sleep(D2);
    m_socket->writeDatagram(mass, RemoteAddress, Port2);

    ui->pTE_port1Recieved->appendPlainText(str);
    str ="Sended to "+QString::number(Port2)+": "+message;
    ui->pTE_port1Recieved->appendPlainText(str);
    ui->pTE_port1Recieved->appendPlainText(next);;

    MyTimer -> start(1000);
}


void MainWindow::on_pB_port1Send_3_clicked()
{
    QString RemotePort;
     QByteArray mass;
     QString message, str;
    QString delay, delay2;

     //взяли IP-адрес компьютера, которому хотим отправить сообщение
      QHostAddress RemoteAddress(QHostAddress::LocalHost);

     using namespace std;

     const int N = 1; //Количество испытаний
     srand(time(0));
     QString text;

     double A = 1., B = 100.;
     double d[N];// диапазон

    for(int i = 0; i < N; i++)
    {

        d[i] = A + (double)rand() / RAND_MAX * (B - A);

    }

    for(int i = 0; i < N; i++)
    {
     text += QString::number(d[i]) + " ";
    }

    ui->lE_sendingMessage->setText(text);

    message=ui->lE_sendingMessage->text();
    mass = message.toUtf8();

    delay = ui -> Delay -> text();
    delay2 = ui -> Delay_2 -> text();
    time_t now = time(0);
    // Преобразуем в структуру tm для локальной временной зоны
    tm* localtm = localtime(&now);
    str = asctime(localtm);

    //ui->pTE_port1Recieved->appendPlainText(str);
    QString next, null;
    next = "----------------------------------------------------------------------------------------------------------";

    //str ="Sended to "+RemoteAddress.toString()+": "+message;
    //ui->pTE_port1Recieved->appendPlainText(str);
    //ui->pTE_port1Recieved->appendPlainText(next);;

    float D1;
    int  Port1 = 1010;
    D1 = 1000*delay.toFloat();
    Sleep(D1);
    m_socket->writeDatagram(mass, RemoteAddress, Port1);
    ui->pTE_port1Recieved->appendPlainText(str);
    str ="Sended to "+QString::number(Port1)+": "+message;
    ui->pTE_port1Recieved->appendPlainText(str);
    ui->pTE_port1Recieved->appendPlainText(next);;
    MyTimer -> start(1000);
}

//------------------------------------------
