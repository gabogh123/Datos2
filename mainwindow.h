#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QDebug>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QStandardItemModel;


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbrir_triggered();

    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_verticalSlider_sliderMoved(int position);

    void on_horizontalSlider_sliderMoved(int position);

    void on_positionChanged(qint64 position);

    void on_durationChanged(qint64 position);

private:
    Ui::MainWindow *ui;
    QStandardItemModel *mModel;
    QMediaPlayer* player;
    void setValueAt(int ix, int jx, const QString &value);
};
#endif // MAINWINDOW_H
