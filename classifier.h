#ifndef CLASSIFIER_H
#define CLASSIFIER_H
#include <QXmlDefaultHandler>

#include <QVector>
#include <QMap>

class Classifier : public QXmlDefaultHandler
{ 
private:
    QString m_strText;

   // int k=0; //подсчет стран
    bool metk = false; // метка на обработку тега

public:

    Classifier();

    QMap<QString, QString> m_map;   //Словарь для записи данных каждой страны
    QMap<QString,QString>::iterator it = m_map.begin();
    QVector<QMap<QString, QString>> my_vector;  //Вектор, хранящий данные всех стран
    QVector<QMap<QString, QString>>::iterator vec = my_vector.begin();

    bool startElement(const QString &,
                      const QString &,
                      const QString &tag,
                      const QXmlAttributes  &);


    bool characters(const QString& strText);

    bool endElement(const QString&, const QString&, const QString& str);

    bool fatalError(const QXmlParseException& exception);

    QVector<QMap<QString, QString>> GetVector() {return my_vector;}


};

#endif // CLASSIFIER_H
