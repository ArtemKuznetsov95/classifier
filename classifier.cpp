#include "classifier.h"
#include <QDebug>

Classifier::Classifier(){}
QSet<QString> set1 = {"ir:row",};
QSet<QString>::iterator itSet;

//Начало работы с элементом. При открытии тега
//Третий параметр тег
//Четвертый атрибут тега
bool Classifier::startElement(const QString&,
                      const QString&,
                      const QString& tag,
                      const QXmlAttributes&){

    if(tag == "ir:row")
    {
        metk = true; // Разрешение на запись данных
    }
    return true;  //Ошибок нет, анализ продолжается
}

//Вызывается при чтении данных элемента. Записывает текущее содержимое элемента
bool Classifier::characters(const QString& strText){

    m_strText = strText;
    return true;    //Сообщаем, что все хорошо. Продолжать анализ
}

//Вызывается при завершении обработки элемента
//Третий параметр имя тега. Если не совпадает с тегом, то данные выводятся на экран
bool Classifier::endElement(const QString&, const QString&, const QString& str){


    if (metk == true && str != "ir:row") //Если разрешено читать
    {     
        m_map.insert(str,m_strText);    //Записываем данные тега в словарь
    }
    else if(str == "ir:row")
    {
        metk = false;   //Сообщает, что данные страны закончились, и что записывать больше не нужно
        my_vector.append(m_map);    //Добавляем словарь с данными страны в вектор
        m_map.clear();
    }
    else if (str == "ir:uuid" || str == "ir:name" ) {
        m_map.insert(str,m_strText);
        my_vector.append(m_map);
        m_map.clear();
    }
    return true;    //Сообщаем, что все хорошо. Продолжать анализ
}

//Вызывается при наличии ошибки
bool Classifier::fatalError(const QXmlParseException& exception) {

    qDebug() << "Line:" << exception.lineNumber()
                 << ", Column:" <<exception.columnNumber()
                 << ", Message:"    << exception.message();
    return false;   //Сообщаем ошибки.
}



