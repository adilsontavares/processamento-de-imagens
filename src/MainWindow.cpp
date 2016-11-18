#include "MainWindow.hpp"

#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFileDialog>
#include <QWidget>
#include <QStyle>
#include <QString>
#include <QMessageBox>

#include "Image.hpp"
#include "ImageFilters.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto mainLayout = new QVBoxLayout();
    auto imageLayout = new QHBoxLayout();

    _statusLabel = new QLabel("Abra uma imagem para aplicar o filtro.");
    _statusLabel->setAlignment(Qt::AlignCenter);

    _originalImageView = new QLabel("Imagem original");
    _originalImageView->setAlignment(Qt::AlignCenter);
    _originalImageView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    _outputImageView = new QLabel("Imagem modificada");
    _outputImageView->setAlignment(Qt::AlignCenter);
    _outputImageView->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);

    _applyFilterButton = new QPushButton("Aplicar filtro");
    connect(_applyFilterButton, SIGNAL(clicked(bool)), this, SLOT(applyFilter()));

    auto separator = new QWidget();
    separator->setFixedWidth(1);
    separator->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
    separator->setStyleSheet(QString("background-color: rgba(0, 0, 0, 0.2);"));

    imageLayout->addWidget(_originalImageView);
    imageLayout->addWidget(separator);
    imageLayout->addWidget(_outputImageView);

    imageLayout->setStretch(0, 1);
    imageLayout->setStretch(2, 1);

    mainLayout->addWidget(_statusLabel);
    mainLayout->addLayout(imageLayout);
    mainLayout->addWidget(_applyFilterButton);

    auto centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setOriginalImage(0);

    createMenu();
}

QPixmap MainWindow::pixmapFromImage(Image *image)
{
    auto qimage = QImage((uchar*)&image->getData()[0], image->getWidth(), image->getHeight(), QImage::Format_RGBA8888);

    QPixmap pixmap;
    pixmap.convertFromImage(qimage);

    return pixmap;
}

void MainWindow::applyFilter()
{
    auto filter = new FilterInvert();
    auto image = new Image(*_originalImage);

    double start = Time::seconds();
    image->applyFilter(filter);
    double end = Time::seconds();

    double duration = (end - start);

    QString status;
    status.sprintf("Filtro \"%s\" aplicado em %.2fs", filter->getName(), duration);
    _statusLabel->setText(status);

    setOutputImage(image);
}

void MainWindow::setOutputImage(Image *image)
{
    _outputImage = image;

    if (image)
    {
        _outputImageView->setPixmap(pixmapFromImage(image));
        _applyFilterButton->setEnabled(false);
    }
    else
    {
        _outputImageView->setText("Imagem modificada");
    }
}

void MainWindow::setOriginalImage(Image *image)
{
    _originalImage = image;

    if (!image)
    {
        _statusLabel->setText("Abra uma imagem para aplicar o filtro.");

        _originalImageView->setText("Imagem original");
        _applyFilterButton->setEnabled(false);
    }
    else
    {
        QString status;
        status.sprintf("Imagem de tamanho %d x %d", image->getWidth(), image->getHeight());
        _statusLabel->setText(status);

        _originalImageView->setPixmap(pixmapFromImage(image));
        _applyFilterButton->setEnabled(true);
    }

    setOutputImage(0);
}

void MainWindow::createMenu()
{
    auto menu = menuBar()->addMenu("&Arquivo");
    auto openFile = menu->addAction("&Abrir");

    connect(openFile, SIGNAL(triggered(bool)), this, SLOT(openFile()));
}

void MainWindow::openFile()
{
    auto dialog = new QFileDialog();
    dialog->setViewMode(QFileDialog::Detail);

    if (dialog->exec())
    {
        auto file = dialog->selectedFiles().first();
        auto image = Image::load(file.toStdString().c_str());

        if (!image)
            QMessageBox::warning(this, tr("Erro ao abrir a imagem."), tr("A imagem não pôde ser aberta. Verifique se ela está em formato PNG e tente novamente."), QMessageBox::Ok);

        setOriginalImage(image);
    }
}
