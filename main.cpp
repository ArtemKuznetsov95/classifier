#include "classifier.h"

#include <QCoreApplication>
#include <QProcess>
#include <QDebug>
#include <QtXml>

int main(int argc, char *argv[])
{
    //Блок распаковки архива!!!!!!!!!!!!!!!!!!!!!!!
    {
    //Проверка на передачу архива параметром командной строки
    if(argc==1) {
        printf("Вы забыли указать имя архива.\n");
        exit(1);
    }

    //Создание процесса для распаковки архива
    QProcess *proc = new QProcess;
    QStringList argment;

    //Для выгрузки файлов архива в проект, требуется изменить каталог в КС
    QString adres = "tar fx " +QString(argv[1]); // tar - встроенный архиватор, fx - параметр распаковки архива, argv[1] - архив
    argment <<"-c" << adres;
    proc->start("bash", argment);   //Открытие КС с параметрами.Выполняется до завершения процесса
    proc->waitForFinished(); //Присылает ошибки по завершению процесса
    printf("Архив %s распакован!\n", argv[1]);
    }

    //Блок открытие файла .xml для дальнейшей работы. Работа с SAX!!!!!!!!!!!!
    {
    Classifier hander;  //Создание обьекта класса Classifier
    QFile file("./resource.xml");
    if (!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Файл не открыт!\n";
        return 1;
    }
    else {
        qDebug() << "Файл открыт!\n";

        QXmlInputSource source(&file);  //Обьект который принял файл xml
        QXmlSimpleReader reader;    //Обьект для анализа файла
        reader.setContentHandler(&hander); //устанавливаем обьект hander унаследованный от QXmlDefaultHandler, который выполняет анализ и отображение
        reader.parse(source); //Запуск процесса анализа файла
        file.close();
    }

    //Блок архивации данных в файл json
    QVector<QMap<QString, QString>> vec;
    QVector<QMap<QString, QString>>::iterator itVec = vec.begin();
    vec = hander.GetVector();           //Вектор - копия вектора из класса классификатора

    QMap<QString, QString> mapLander;   //Словарь для временного хранения данных по каждой стране
    QMap<QString,QString>::iterator it1 = mapLander.begin();
    QString uuidClassifier, classifier_name; //строки для хранения УИД и имени классификатора

    //Вытаскивание УИД и имени классификатора
    for(itVec = vec.begin(); itVec!=vec.end();++itVec) {
        mapLander = *itVec;
        for(it1=mapLander.begin(); it1!=mapLander.end();++it1) {
            if(it1.key()=="ir:uuid") {
                uuidClassifier = mapLander["ir:uuid"];

                //break;
            } else if (it1.key()=="ir:name") {
                classifier_name = mapLander["ir:name"];
            }
        }
    }

    QString name_file = "res_"+uuidClassifier+".json";

    QFile f("./"+name_file);
    if (f.open(QIODevice::ReadWrite)) {
        QJsonObject m_currentJsonObject;
        m_currentJsonObject["uuid"] = uuidClassifier;
        m_currentJsonObject["classifier_name"] = classifier_name;
        QJsonArray textsArray1; // для записи предыдущей страны

        for(itVec = vec.begin(); itVec!=vec.end(); ++itVec)
        {
            mapLander = *itVec;
            QJsonObject textObject;
            QJsonArray textsArray;
            textObject["ir:TK_KSMREG_KOD1"] = mapLander["ir:fc__uuid"];
            textObject["ir:TK_KSMREG_PR"] = mapLander["ir:TK_KSMREG_PR"];
            textObject["ir:TK_KSMREG_ENDDA"] = mapLander["ir:TK_KSMREG_ENDDA"];
            textObject["ir:TK_KSMREG_BEGDA"] = mapLander["ir:TK_KSMREG_BEGDA"];
            textObject["ir:TK_KSMREG_NAME"] = mapLander["ir:TK_KSMREG_NAME"];
            textObject["zhild"] = textsArray1;
            textObject["ir:TK_KSMREG_KOD2"] = mapLander["ir:TK_KSMREG_KOD2"];
            textObject["ir:fc__uuid"] = mapLander["ir:fc__uuid"];
            textsArray.append(textObject);
            m_currentJsonObject["zhild"] = textsArray;
            textsArray1 = textsArray;
        }f.write(QJsonDocument(m_currentJsonObject).toJson(QJsonDocument::Indented)); // Запись в файл
    }
    f.close();
    }

    return 0;
}


