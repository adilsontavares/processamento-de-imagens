#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QPixmap>

#include "Image.hpp"

class MainWindow : public QMainWindow
{
private:

    Image *_originalImage;
    Image *_outputImage;

    QLabel *_statusLabel;

    QLabel *_originalImageView;
    QLabel *_outputImageView;

    QPushButton *_applyFilterButton;

    void createMenu();

    void setOriginalImage(Image *image);
    void setOutputImage(Image *image);

    QPixmap pixmapFromImage(Image *image);

    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);

signals:

public slots:

    void openFile();
    void applyFilter();
};

#endif // MAINWINDOW_H
