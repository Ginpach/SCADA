#include "mainwindow.h"

#include <QApplication>

#include <QCoreApplication>
#include <QFile> // Подключаем класс QFile
#include <QTextStream> // Подключаем класс QTextStream

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    /*
    QFile fileOut("fileout.txt"); // Связываем объект с файлом fileout.txt
        if(fileOut.open(QIODevice::WriteOnly | QIODevice::Text))
        { // Если файл успешно открыт для записи в текстовом режиме
            QTextStream writeStream(&fileOut); // Создаем объект класса QTextStream
    // и передаем ему адрес объекта fileOut
            writeStream << "Text, text, text."; // Посылаем строку в поток для записи
            fileOut.close(); // Закрываем файл
        }
*/
    return a.exec();
}
