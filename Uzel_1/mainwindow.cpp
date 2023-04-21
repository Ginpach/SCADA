#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <Windows.h>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    MyTimer = new QTimer(this);
    m_socket = new QUdpSocket(this);
    connect(m_socket,&QUdpSocket::readyRead,this,&MainWindow::ReadyRead);

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pB_port1Open_clicked()
{
     bool result;

    //пытаемся открыть сокет
     result=m_socket->bind(QHostAddress::LocalHost, 1010); // Порт узла 1
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

void MainWindow::ReadyRead()
{
    QByteArray mass;
    unsigned short SenderPort;
    QHostAddress SenderAddress;
    QString ReadedData, str,log;

    QString RemotePort, message;



    //узнать, сколько байт содержит принятый пакет и выделить под него место
    mass.resize(m_socket->pendingDatagramSize());
    //считать содержимое пришедшего пакета в массив байтов
    m_socket->readDatagram(mass.data(), mass.size(),&SenderAddress,&SenderPort);
    if(mass.size()>0)
    {
        ReadedData=QString::fromUtf8(mass);
        if(ReadedData[0]=='0' && ReadedData[1]=='x')
        {
            ui->status_port->setText("OK");
            QHostAddress RemoteAddress(QHostAddress::LocalHost);
            log = ReadedData.toUtf8();
            mass = log.toUtf8();
            if(ReadedData[2]!='3')
            {
                QString delay;
                delay = ui -> Delay -> text();
                Sleep(1000*delay.toInt());
                m_socket->writeDatagram(mass, RemoteAddress, 7070);
            }
            else
            {
                log.append('1');
                mass = log.toUtf8();
                QString delay_2;
                delay_2 = ui -> Delay_2 -> text();
                Sleep(1000*delay_2.toInt());
                m_socket->writeDatagram(mass, RemoteAddress, 0001);
            }

        }
        else
        {

    QString delay;
    delay = ui -> Delay -> text();


    time_t now = time(0);
    // Преобразуем в структуру tm для локальной временной зоны
    tm* localtm = localtime(&now);
    str = asctime(localtm);

    ui->pTE_port1Recieved->appendPlainText(str);

    //преобразовать пришедшую последовательность байт в символы
    ReadedData=QString::fromUtf8(mass);
    //сформировать сообщение для показа в окне принятых и отправленных



     str ="Received from "+SenderAddress.toString()+":"+ReadedData;
    //показать отправленное сообщение в окне принятых и отправленных


     QString next, null;
     next = "----------------------------------------------------------------------------------------------------------";
     null = " ";

    ui->pTE_port1Recieved->appendPlainText(str);
    ui->pTE_port1Recieved->appendPlainText(null);




    //ui->lE_sendingMessage->setText(ReadedData);

    //взяли IP-адрес компьютера, которому хотим отправить сообщение
    QHostAddress RemoteAddress(QHostAddress::LocalHost);
    //взяли номер UDP-порта сокета на этом компьютере
    //RemotePort =ui->lE_port2Port->text();
    //преобразовали введенные пользователем символы в последовательность байт
    // message=ui->lE_sendingMessage->text();



    Sleep(1000*delay.toInt());

    message=ReadedData;
    mass = ReadedData.toUtf8();
    //отправили последовательность байт через сокет по адресу
    m_socket->writeDatagram(mass, RemoteAddress, 7070); // порт SCADA
    //сформировать сообщение для показа в окне принятых и отправленных



    str = asctime(localtm);
    ui->pTE_port1Recieved->appendPlainText(str);


    str ="Sended to "+RemoteAddress.toString()+": "+message;
    //показать отправленное сообщение в окне принятых и отправленных
    ui->pTE_port1Recieved->appendPlainText(str);


     ui->pTE_port1Recieved->appendPlainText(next);


    }
}
}
//-------------------------

