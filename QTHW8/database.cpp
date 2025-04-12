#include "database.h"
#include <cstdint>
#include <qcontainerfwd.h>
#include <qsqldatabase.h>
#include <qsqlquery.h>
#include <qtablewidget.h>

DataBase::DataBase(QObject *parent)
    : QObject{parent}
{

    /*Выделяем память под объекты классов.
     *Объект QSqlDatabase является основным классом низкого уровня,
     *в котором настраивается подключение к БД.
    */
    dataBase = new QSqlDatabase( );

    //Объект QSqlQuery отвечает за формирование запросов к БД
    simpleQuery = new QSqlQuery( );

    //QTableWidget необходим для формирования отображения ответа и передачи его в форму.
    tableWidget = new QTableWidget( );

}

DataBase::~DataBase()
{
    delete dataBase;
    delete simpleQuery;
}

/*!
 * \brief Метод добавляет БД к экземпляру класса QSqlDataBase
 * \param driver драйвер БД
 * \param nameDB имя БД (Если отсутствует Qt задает имя по умолчанию)
 */
void DataBase::AddDataBase(QString driver, QString nameDB)
{
    /*  Класс QSqlDatabase представляет из себя интерфейс между
        кодом и БД. Он организует соединение с БД. Для подключения
        в конструктор класса необходимо передать драйвер БД и имя.
        В объекте может храниться несколько подключений, они различаются именами.
    */
    *dataBase = QSqlDatabase::addDatabase(driver, nameDB);


}

/*!
 * \brief Метод подключается к БД
 * \param для удобства передаем контейнер с данными необходимыми для подключения
 * \return возвращает тип ошибки
 */
void DataBase::ConnectToDataBase(QVector<QString> data)
{
    ///Для подключения необходимо передать параметры БД в объект класса.

    dataBase->setHostName(data[hostName]);
    dataBase->setDatabaseName(data[dbName]);
    dataBase->setUserName(data[login]);
    dataBase->setPassword(data[pass]);
    dataBase->setPort(data[port].toInt());

    bool statusConnect = dataBase->open( );
    emit sig_SendStatusConnection(statusConnect);
}

/*!
 * \brief Метод производит отключение от БД
 * \param Имя БД
 */
void DataBase::DisconnectFromDataBase(QString nameDb)
{

    //Сначала выбираем имя БД
    dataBase->database(nameDb);

    //Метод close() закрывает соединение с БД
    dataBase->close( );

}

/*!
 * \brief Метод формирует запрос к БД.
 * \param request - SQL запрос
 * \return
 */
void DataBase::RequestToDB(QString request)
{
    /*  Для фоомирования запроса будем использовать
     *  объект QSqlQuery.
     *  В конструктор передадим адрес объекта QSqlDatabase
    */
    ///Связать экземпляр QSqlQuery с QSqlDataBase
    *simpleQuery = QSqlQuery(*dataBase);

    /*
       Выполнение запроса выполняется при помощи
       метода exec. В случае успешного запроса он вернет true.
       Если возникает какая либо ошибка, ее можно посмотреть
       при помощи метода lastError. Этот метод возвращает
       экземпляр класса QSqlError.
    */
    ///Выполним запрос
    bool statusRequest = simpleQuery->exec(request);
    if(statusRequest){
       emit sig_SendStatusRequest(simpleQuery->lastError());
    }
    else{
        qDebug() << "Error";
        emit sig_SendStatusRequest(simpleQuery->lastError());
    }

}

/*!
 * \brief Метод читае ответ, полученный от БД при помощи QSqlQuery
 * \param answerType - тип ответа
 */
void DataBase::ReadAnswerFromDB(int requestType)
{
    /*
     * Используем оператор switch для разделения запросов
    */
    switch (requestType) {
    //Для наших запросов вид таблицы не поменяетя. Поэтому бужет единый обработчик.
    case requestAllFilms:
    case requestComedy:
    case requestHorrors:
    {

        /*
         * Объект tableWidget представляет из себя таблицу, которую можно отобразить
         * в ПИ. Зададим ее параметры и заголовки. Мы получаем из БД название фильма, год выпуска и жанр
        */
        const int columnCount = 3;

        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(columnCount);

        QStringList headers;
        headers << "Название" << "Год выпуска" << "Жанр";
        tableWidget->setHorizontalHeaderLabels(headers);

        uint32_t rowCounter = 0;

        /* QSqlQuery
         * После запроса к БД ответ помещается в объект QSqlQuery.
         * Его необходимо считвать построчно. Для чтения ответа используется метод
         * next, он работает следующим образом: при вызове метода указатель перемещается
         * на следующую запись, в ответе. После получения ответа он указывает, если можно так
         * выразится на -1 запись. Т.е. при первом вызове метода next указатель переместится
         * на первую запись, а сам метод вернет true. В случае если указатель переместится
         * за диапазон записей метод вернет false. Также он вернет false в случае отсутствия
         * записей.
         * Каждая запись является стройкой прочитанной из БД. Для перемещения по столбцам
         * используется метод QSqlQuery value(индекс столбца).
        */
        /* QTableWidget
         * Необходимо сформировать таблицу, которую потом передадим в MainWindow для отображения
         * Для каждой записи прочитанной при помощи метода next будем добавлять строку в tableWidget
         * и заполнять значениями из ответа. Каждая ячейка таблицы является объектом класса
         * QTableWidgetItem. Т.е. для заполнения каждой ячеки мы должны создать экземпляр этого метода.
         * Значение ячейки в таблицк устанавливается при помощи метода setItem.
        */


        ///Заполнить таблицу
        //Каждый вызов next смещает итератор построчно по запрошенной таблице
        while(simpleQuery->next( )){

            tableWidget->insertRow(rowCounter);

            for(int currentColumn = 0; currentColumn < columnCount; ++currentColumn){

                QString text = simpleQuery->value(currentColumn).toString( );
                QTableWidgetItem* item = new QTableWidgetItem(text);
                tableWidget->setItem(rowCounter, currentColumn, item);

            }
            ++rowCounter;
        }


        /*
         * После заполнения всей таблицы мы ее передаем в MainWindow при помощи сигнала.
        */
        sig_SendDataFromDB(tableWidget, requestType);

        break;

    }
    default:
        break;
    }
}


/*!
 * @brief Метод возвращает последнюю ошибку БД
 */
QSqlError DataBase::GetLastError()
{
    return dataBase->lastError( );
}
