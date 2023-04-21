#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDateTime>
#include <windows.h>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    m_socket = new QUdpSocket(this);
    connect(m_socket,&QUdpSocket::readyRead,this,&MainWindow::ReadyRead);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pB_port1Open_clicked()
{
    QString Port;
     bool result;

    //считываем номер UDP-порта, введенный пользователем
    // Port=ui->lE_port1Port->text();
    //считываем IP-адрес своего компьютера
    // QHostAddress Address(ui->lE_port1IP->text());
    //пытаемся открыть сокет
     result=m_socket->bind(QHostAddress::LocalHost, 7070);
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

//преобразовать пришедшую последовательность байт в символы
 ReadedData=QString::fromUtf8(mass);
//сформировать сообщение для показа в окне принятых и отправленных
 if(ReadedData[0]=='0' && ReadedData[1]=='x')
 {
     ui->  lineEdit->setText("OK");

    QHostAddress RemoteAddress(QHostAddress::LocalHost);
    log = ReadedData.toUtf8();

    log.append('3');
    log.append('t');
    long long dt = QDateTime::currentMSecsSinceEpoch();
    log.append(QString::number(dt));
    log.append('t');


//ui->pTE_port1Recieved_2->appendPlainText(log);

    mass = log.toUtf8();
    QString delay, delay2;
    delay = ui -> lineEdit_2 -> text();
    delay2 = ui -> lineEdit_3 -> text();
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


   // m_socket->writeDatagram(mass, RemoteAddress, 1010);
    //m_socket->writeDatagram(mass, RemoteAddress, 2020);
 }

 else
  {

     QString null = "----------------------------------------------------------------------------------------------------------"; //линия
     time_t now = time(0); //фигня для времени
    // Преобразуем в структуру tm для локальной временной зоны
     tm* localtm = localtime(&now); //фигня для времени




 str = asctime(localtm); //запихиваем туда время
 ui->pTE_port1Recieved->appendPlainText(str); //печатаем





str = "Received from "+SenderAddress.toString()+": "+ReadedData;
//показать отправленное сообщение в окне принятых и отправленных
 ui->pTE_port1Recieved->appendPlainText(str);



ui->pTE_port1Recieved->appendPlainText(null); //полоска



 QFile fileOut("fileout.txt"); // Связываем объект с файлом fileout.txt
     if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
     { // Если файл успешно открыт для записи в текстовом режиме
         QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
 // и передаем ему адрес объекта fileOut
         writeStream << str; // Посылаем строку в поток для записи
         fileOut.close(); // Закрываем файл
     }
}
}
}
//-----------------------------------
