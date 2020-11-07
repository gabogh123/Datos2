#ifndef MAINWINDOW_H
#define MAINWINDOW_H

/// Se incluyen las librerias necesarias

#include <QMainWindow>
#include <QMediaPlayer>
#include <QDebug>
#include <QFile>
#include "memorymanager.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

/**
 *Se inicializa la clase StandarItemModel para los valores del modelo del tableView
 */

class QStandardItemModel;


/**
 * @brief The MainWindow class
 *
 */

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    MemoryManager *mManager;

///Se inicializan los métodos a utilizar en el main window

private slots:
    void on_actionAbrir_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_verticalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

    void runMemUsage();


/// Se crean las variables y objetos a utilizar en el main window
    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    QStandardItemModel *mModel;
    QMediaPlayer* player;
    void setValueAt(int ix, int jx, const QString &value);
    int convertToint(double);
    void loadPage();
    QFile* file;
    int t_count;
    bool paginate;
    bool firstTime;

    QStringList titulos {"Track ID","Album comments", "Album Date_created", "Album Date_released",
                        "Album favorites", "Album ID", "Album listens", "Album title",
                        "Album tracks", "Album type", "Artist date_created", "Artist favorites",
                        "Artist id","Artist location", "Track bit_rate", "Track duration",
                        "Track genre_top","Track title"};
};
#endif // MAINWINDOW_H
