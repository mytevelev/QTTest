#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->setWindowTitle("Films");

    //Исходное состояние виджетов
    ui->setupUi(this);
    ui->lb_statusConnect->setStyleSheet("color:red");
    ui->pb_request->setEnabled(false);

    /*
     * Выделим память под необходимые объекты. Все они наследники
     * QObject, поэтому воспользуемся иерархией.
    */

    dataDb = new DbData(this);
    dataBase = new DataBase(this);
    msg = new QMessageBox(this);

    //Установим размер вектора данных для подключения к БД
    dataForConnect.resize(NUM_DATA_FOR_CONNECT_TO_DB);

    /*
     * Добавим БД используя стандартный драйвер PSQL и зададим имя.
    */
    dataBase->AddDataBase(POSTGRE_DRIVER, DB_NAME);

    /*
     * Устанавливаем данные для подключениея к БД.
     * Поскольку метод небольшой используем лямбда-функцию.
     */
    connect(dataDb, &DbData::sig_sendData, this, [&](QVector<QString> receivData){
        dataForConnect = receivData;
    });

    /*
     * Соединяем сигнал, который передает ответ от БД с методом, который отображает ответ в ПИ
     */
     connect(dataBase, &DataBase::sig_SendDataFromDB, this, &MainWindow::ScreenDataFromDB);

    /*
     *  Сигнал для подключения к БД
     */
    connect(dataBase, &DataBase::sig_SendStatusConnection, this, &MainWindow::ReceiveStatusConnectionToDB);
    connect(dataBase, &DataBase::sig_SendStatusRequest, this, &MainWindow::ReceiveStatusRequestToDB);

    dataForConnect[hostName] = "981757-ca08998.tmweb.ru";
    dataForConnect[dbName] = "netology_cpp";
    dataForConnect[login] = "netology_usr_cpp";
    dataForConnect[pass] = "CppNeto3";
    dataForConnect[port] = "5432";
}

MainWindow::~MainWindow()
{
    ///Отключиться от БД
    dataBase->DisconnectFromDataBase(DB_NAME);
    if(tbWid){
        delete tbWid;
    }

    delete ui;
}

/*!
 * @brief Слот отображает форму для ввода данных подключения к БД
 */
void MainWindow::on_act_addData_triggered( )
{
    //Отобразим диалоговое окно. Какой метод нужно использовать?
    dataDb->show( );
}

/*!
 * @brief Слот выполняет подключение к БД. И отображает ошибки.
 */

void MainWindow::on_act_connect_triggered( )
{
    /*
     * Обработчик кнопки у нас должен подключаться и отключаться от БД.
     * Можно привязаться к надписи лейбла статуса. Если он равен
     * "Отключено" мы осуществляем подключение, если "Подключено" то
     * отключаемся
    */

    if(ui->lb_statusConnect->text() == "Отключено"){

       ui->lb_statusConnect->setText("Подключение");
       ui->lb_statusConnect->setStyleSheet("color : black");

       ///Подключение к базе данных
       auto connectToDb = [this]{ dataBase->ConnectToDataBase(dataForConnect); };
       QFuture<void> futConn = QtConcurrent::run(connectToDb);

    }
    else{

       ///Отключение от базы данных
        dataBase->DisconnectFromDataBase(DB_NAME);
        ui->lb_statusConnect->setText("Отключено");
        ui->act_connect->setText("Подключиться");
        ui->lb_statusConnect->setStyleSheet("color:red");
        ui->pb_request->setEnabled(false);
    }
}

/*!
 * \brief Обработчик кнопки "Получить"
 */
void MainWindow::on_pb_request_clicked()
{
/*
 * В случае если ответ получен, то мы его читаем
 * в противном случае выводим ошибку. Сейчас мы разберем
 * получение всех фильмов. А дома вы получите отдельно комедии
 * и ужасы
 *     QString request = "SELECT title, release_year, c.name  FROM film f "
                      "JOIN film_category fc on f.film_id = fc.film_id "
                      "JOIN category c on c.category_id  = fc.category_id";
*/

    switch ( ui->cb_category->currentIndex( ) + 1 )
    {
    case requestAllFilms :
        request = "SELECT title, release_year, c.name  FROM film f "
                  "JOIN film_category fc on f.film_id = fc.film_id "
                  "JOIN category c on c.category_id  = fc.category_id";

        break;
    case    requestComedy:
        request = "SELECT title, release_year, c.name  FROM film f "
                  "JOIN film_category fc on f.film_id = fc.film_id "
                  "JOIN category c on c.category_id  = fc.category_id where c.name = 'Comedy' ";


        break;

    case    requestHorrors:
        request = "SELECT title, release_year, c.name  FROM film f "
                  "JOIN film_category fc on f.film_id = fc.film_id "
                  "JOIN category c on c.category_id  = fc.category_id where c.name = 'Horror' ";

        break;

    }

    ///Запрос к БД
    auto reqToDb = [this]{dataBase->RequestToDB(request);};
    QFuture<void> futRcvData = QtConcurrent::run(reqToDb);
}

/*!
 * \brief Слот отображает значение в QTableWidget
 * \param widget
 * \param typeRequest
 */
void MainWindow::ScreenDataFromDB(QTableWidget *widget, int typeRequest)
{
    /*
     * По аналогии с формированием таблицы заполним таблицу
     * которая распаложена на главной форме. Необходимо пройтись
     * по всем элементам и приравнять их.
     * Также привяжем ширину заголовка к ширене окна.
    */
    switch (typeRequest) {

    case requestAllFilms:
    case requestHorrors:
    case requestComedy:{

    ///Отобразить данные

        tbWid = widget;
        ui->TableWid->layout( )->addWidget(tbWid);
        break;

    }
    default:
        break;
    }

}
/*!
 * \brief Метод изменяет стотояние формы в зависимости от статуса подключения к БД
 * \param status
 */
void MainWindow::ReceiveStatusConnectionToDB(bool status)
{
    if(status){
        ui->act_connect->setText("Отключиться");
        ui->lb_statusConnect->setText("Подключено к БД");
        ui->lb_statusConnect->setStyleSheet("color:green");
        ui->pb_request->setEnabled(true);
    }
    else{
        ///Отключиться  от БД
        dataBase->DisconnectFromDataBase(DB_NAME);
        msg->setIcon(QMessageBox::Critical);
        msg->setText(dataBase->GetLastError( ).text( ));
        ui->lb_statusConnect->setText("Отключено");
        ui->lb_statusConnect->setStyleSheet("color:red");
        msg->exec( );
    }

}

/*!
 * \brief Метод обрабатывает ответ БД на поступивший запрос
 * \param err
 */
void MainWindow::ReceiveStatusRequestToDB(QSqlError err)
{

    if(err.isValid( )){
        msg->setText(err.text());
        msg->show( );
    }
    else{
        dataBase->ReadAnswerFromDB(ui->cb_category->currentIndex( ) + 1);
    }

}






void MainWindow::on_pb_clear_clicked()
{

    tbWid->clear() ;

}

