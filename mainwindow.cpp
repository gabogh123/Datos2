#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QStandardItemModel>
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QTimer>


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    mManager = new MemoryManager(this);
    mModel = new QStandardItemModel(this);
    ui->tableView->setModel(mModel);
    setWindowTitle("OdisseyRadio");

    player = new QMediaPlayer(this);

    connect(player, &QMediaPlayer:: positionChanged, this, &MainWindow::on_positionChanged);
    connect(player, &QMediaPlayer:: durationChanged, this, &MainWindow::on_durationChanged);

    ui->progressBar->setValue(0);

    QTimer *timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::runMemUsage);
    timer->start(2000);



}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_actionAbrir_triggered()
{
    auto filename = QFileDialog:: getOpenFileName(this,
            "Abrir", QDir::rootPath(), "CSV File(*.csv)" );
    if(filename.isEmpty()){
        return;
    }

    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly|QIODevice::Text)){
        return;
    }

    QTextStream xin(&file);
    int ix = 0;
    while (!xin.atEnd()) {

        mModel->setRowCount(ix);
        auto line =xin.readLine();
        auto values = line.split(",");
        const int colCount= values.size();
        mModel->setColumnCount(colCount);
        for (int jx=0;jx<colCount ;++jx ) {
            setValueAt(ix,jx, values.at(jx));
        }
        ++ix;

    }
    file.close();
}

void MainWindow::setValueAt(int ix,int jx, const QString &value )
{

    if(!mModel->item(ix, jx)){
        mModel->setItem(ix, jx, new QStandardItem(value));
    }else{
        mModel->item(ix,jx)->setText(value);

    }

}

void MainWindow::on_pushButton_clicked()
{
    QString val = ui->tableView->currentIndex().data().toString();

        player ->setMedia(QUrl::fromLocalFile("/home/gabrielgh/Proyecto1/000/" + val+".mp3"));
        //player ->setMedia(QUrl::fromLocalFile("/Qt/OdisseyRadio/Datos2/Datos2/000/" + val+".mp3"));
        player ->play();


   // player ->setMedia(QUrl::fromLocalFile("/home/gabrielgh/Proyecto1/000/food.mp3"));
   //´ player ->play();
}

void MainWindow::on_pushButton_2_clicked()
{
    player->stop();
}

void MainWindow::on_verticalSlider_sliderMoved(int position)
{
    player -> setVolume(position);
}

void MainWindow::on_horizontalSlider_sliderMoved(int position)
{
    player ->setPosition(position);

}

void MainWindow::on_positionChanged(qint64 position){

    ui->horizontalSlider->setValue(position);
}



void MainWindow::on_durationChanged(qint64 position){

    ui->horizontalSlider->setMaximum(position);
}

void MainWindow::on_tableView_activated(const QModelIndex &index)
{
    QString val = ui->tableView->currentIndex().data().toString();
    if (val == "Food");
    player ->setMedia(QUrl::fromLocalFile("/home/gabrielgh/Proyecto1/000/"+ val +".mp3"));
    //player ->setMedia(QUrl::fromLocalFile("/home/gabrielgh/Proyecto1/000/"+ val +".mp3"));
    player ->play();

}

void MainWindow::runMemUsage()
{
    double vm,rss;
    mManager->proccesMemoryUsage(vm,rss);
    int max = 7933524;
    int value = convertToint(vm);
    ui->progressBar->setMaximum(max);
    ui->progressBar->setValue(value);
}

int MainWindow::convertToint(double db)
{
    double x = db;
    x = x + 0.5 - (x<0);
    int y = (int)x;
    return y;
}
