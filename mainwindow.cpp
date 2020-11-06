/// Se incluyen las librerias necesarias

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QTimer>
#include "unistd.h"
#include <limits>

/**
 * @brief MainWindow Ventana Principal del programa que contiene los componentes con los que funciona el programa
 * @param parent
 *
 */
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

/// Se inicializa la interfaz de usuario y se crea el modelo para crear la tabla
/// Se crea el manejador de memoria

    ui->setupUi(this);
    mManager = new MemoryManager(this);
    mModel = new QStandardItemModel(this);


/// Se establece el modelo para el tableView

    ui->tableView->setModel(mModel);
///Se editan los headears y se elimina la opción de editar las casillas de la tabla
    ui->tableView->setStyleSheet("QHeaderView::section { background-color:red }");
    ui->tableView->setEditTriggers(QAbstractItemView :: NoEditTriggers);
    ui->tableView->setSortingEnabled(true);


/// Se crea el player como un objeto de la clase QMediaPlayer

    player = new QMediaPlayer(this);

/// Se conecta el player con la posición y duración de las canciones a reproducir

    connect(player, &QMediaPlayer:: positionChanged, this, &MainWindow::on_positionChanged);
    connect(player, &QMediaPlayer:: durationChanged, this, &MainWindow::on_durationChanged);

/// Barra de progreso de la canción incializada en 0 para que incremente

    ui->progressBar->setValue(0);

/// Timer para que cada dos segundos se ejecute la funcion del uso de memoria
    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::runMemUsage);
    timer->start(2000);

/// Contador para pasar de pagina
    t_count = 1;

/// Modo paginacion
    paginate = true;
}

MainWindow::~MainWindow()
{
    delete ui;
}

/**
 * @brief MainWindow::on_actionAbrir_triggered
 * Al activarlo accede a los archivos del sistema, permitiendo cargar el archivo .csv
 * En la parte superior derecha, como sub-separator de "Archivo"
 */


void MainWindow::on_actionAbrir_triggered()
{

/// Accede a los archivos del sistema

    auto filename = QFileDialog:: getOpenFileName(this,
            "Abrir", "/home", "CSV File(*.csv)" );
    if(filename.isEmpty()){
        return;
    }

    file = new QFile(filename);

/// Carga la pagina
    loadPage(15);
}

void MainWindow::loadPage(int size)
{
/// Carga el archivo del nombre que sea seleccionado y si estos son de solo lectura o texto, son abiertos
    if (!file->open(QIODevice::ReadOnly|QIODevice::Text)){
            return;
    }
/// Se inicializa el contador para recorrer las filas del archivo
    QTextStream xin(file);
    int i = 0;
    int ix = 0;

/// Carga la primera pagina
    if(t_count==1){
    while(i<size){

        auto line =xin.readLine(); ///Lee la linea de la posición en la que se encuentre
        auto values = line.split(","); ///Si en dicha posició hay una "," se realiza un salto de linea

        const int colCount= values.size();

/// Se asigna el valor del contador a la cantidad de filas del .csv y se le asignan al model creado
        mModel->setRowCount(ix);
        mModel->setColumnCount(colCount); ///Asigna el valor de la cantida de columnas al modelo
        for (int jx=0;jx<colCount ;++jx ) {
            setValueAt(ix,jx,values.at(jx));
        }
        ++ix; /// al terminar con la columna actual pasa a la siguente fila
        ++i;
    }}

/// Carga la pagina dependiendo el numero de pagina
    if(t_count>1){
    while(i<size*t_count){
        auto line =xin.readLine();
        auto values = line.split(",");
        const int colCount= values.size();

        mModel->setRowCount(ix);
        mModel->setColumnCount(colCount);
        for (int jx=0;jx<colCount ;++jx ) {
            setValueAt(ix,jx,values.at(jx));
        }
        ++ix;
        ++i;
    }}

/// Carga la pagina dependiendo el numero de pagina

        if(!paginate){
        while(!xin.atEnd()){
            auto line =xin.readLine();
            auto values = line.split(",");
            const int colCount= values.size();

            mModel->setRowCount(ix);
            mModel->setColumnCount(colCount);
            for (int jx=0;jx<colCount ;++jx ) {
                setValueAt(ix,jx,values.at(jx));
            }
            ++ix;
            ++i;
        }}
    file->close();

}

/**
 * @brief MainWindow::setValueAt
 * Asigna valores a las casillas al leer el archivo .csv
 * @param ix
 * @param jx
 * @param value
 */

void MainWindow::setValueAt(int ix,int jx, const QString &value )
{

    if(!mModel->item(ix, jx)){
        mModel->setItem(ix, jx, new QStandardItem(value)); ///Crea el item y le asigna el valor a la posicion ix y jx
    }else{
        mModel->item(ix,jx)->setText(value); /// EScribe el valor asignado a cada casilla

    }

}

/**
 * @brief MainWindow::on_pushButton_clicked
 * Función del botón play para reproducir la canción seleccionada
 */
void MainWindow::on_pushButton_clicked()
{
    QString val = ui->tableView->currentIndex().data().toString(); /// Reconoce la casilla seleccionada y pasa el valor de esta a String y lo almacena en una variable
    QString userName = getlogin(); /// Obtiene el login del PC para que funcione en muchos equipos


    player ->setMedia(QUrl::fromLocalFile("/home/"+userName+"/Proyecto1/Canciones/" + val+".mp3")); /// Obtiene la dirección del archivo .mp3 y reproduce el archivo del nombre igual a la variable val
    //URL deseada
    //player ->setMedia(QUrl::fromLocalFile("/home/"+userName+"/Proyecto1/Datos2/Canciones/" + val+".mp3"));

    player ->play(); /// El MediaPlayer reproduce la cancion


}

/**
 * @brief MainWindow::on_pushButton_2_clicked
 * Botón pausa de la canción
 */
void MainWindow::on_pushButton_2_clicked()
{
    player->pause(); /// El player pausa la canción al pulsar el botón
}

/**
 * @brief MainWindow::on_verticalSlider_sliderMoved
 * Función para cambiar el volumen del programa
 * @param position
 */

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    player -> setVolume(position); ///Se cambia el volumen según se mueva el slider del volumen
}

/**
 * @brief MainWindow::on_horizontalSlider_sliderMoved
 * Función para adelantar y atrasar la canción
 * @param position
 */
void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    player ->setPosition(position); ///Se determina la posición de la canción al mover el slider del progreso

}

/**
 * @brief MainWindow::on_positionChanged
 * Función para determinar las posición del slider de progreso
 * @param position
 */

void MainWindow::on_positionChanged(qint64 position){

    ui->horizontalSlider->setValue(position);/// Determina mediante una función de la clase QMediaPlayer la posición de slider
}

/**
 * @brief MainWindow::on_durationChanged
 * Función para determinar las posición del slider de progreso
 * @param position
 */

void MainWindow::on_durationChanged(qint64 position){

    ui->horizontalSlider->setMaximum(position);///Determinar la posición máxima del slider que será la duración de la canción
}

/**
 * @brief MainWindow::runMemUsage
 */

void MainWindow::runMemUsage()
{
    double vm,rss;
    mManager->proccesMemoryUsage(vm,rss);
    int max = 7933524;
    int value = convertToint(vm);
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
}

/**
 * @brief MainWindow::convertToint
 * @param db
 * @return conversion
 */

int MainWindow::convertToint(double db)
{
    double x = db;
    x = x + 0.5 - (x<0);
    int y = (int)x;
    return y;
}

/**
 * @brief MainWindow::on_pushButton_4_clicked
 * Carga la pagina anterior
 */
void MainWindow::on_pushButton_4_clicked()
{
    t_count -=1;
    loadPage(15);
}

/**
 * @brief MainWindow::on_pushButton_3_clicked
 * Proporciona la siguiente pagina
 */
void MainWindow::on_pushButton_3_clicked()
{
    t_count +=1;
    loadPage(15);



}

/**
 * @brief MainWindow::on_pushButton_5_clicked
 * Modo paginacion o no paginacion activado
 */
void MainWindow::on_pushButton_5_clicked()
{
    if(paginate==true){
        ui->pushButton_5->setText("No paginate");
        paginate = false;
        loadPage(15);
    }else{
        ui->pushButton_5->setText("Paginate");
        paginate = true;
        loadPage(15);
    }
}
