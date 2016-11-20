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
#include <QScrollArea>
#include <QScrollBar>
#include <QSignalMapper>

#include "Image.hpp"
#include "ImageFilterManager.hpp"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    auto mainLayout = new QVBoxLayout();
    auto imageLayout = new QHBoxLayout();

    _statusLabel = new QLabel("Abra uma imagem para aplicar o filtro.");
    _statusLabel->setStyleSheet(QString("background-color: rgba(0, 0, 0, 0.1);"));
	_statusLabel->setFixedHeight(30);
    _statusLabel->setAlignment(Qt::AlignCenter);

    _originalImageView = new ImageView();
    _originalImageView->setTitle("Imagem original");

    _outputImageView = new ImageView();
    _outputImageView->setTitle("Imagem modificada");

	connect(_originalImageView->horizontalScrollBar(), SIGNAL(valueChanged(int)), _outputImageView->horizontalScrollBar(), SLOT(setValue(int)));
	connect(_originalImageView->verticalScrollBar(), SIGNAL(valueChanged(int)), _outputImageView->verticalScrollBar(), SLOT(setValue(int)));

	connect(_outputImageView->horizontalScrollBar(), SIGNAL(valueChanged(int)), _originalImageView->horizontalScrollBar(), SLOT(setValue(int)));
	connect(_outputImageView->verticalScrollBar(), SIGNAL(valueChanged(int)), _originalImageView->verticalScrollBar(), SLOT(setValue(int)));

	auto filtersLayout = new QHBoxLayout();
	auto filters = ImageFilterManager::instance()->getFilters();
	auto filtersMapper = new QSignalMapper(this);

	_filterButtonsGroup = new QButtonGroup();

    for (unsigned int i = 0; i < filters.size(); ++i)
	{
		auto button = new QPushButton(filters[i]->getName());

		filtersMapper->setMapping(button, i);
		filtersLayout->addWidget(button);

		_filterButtonsGroup->addButton(button, i);

		connect(button, SIGNAL(clicked()), filtersMapper, SLOT(map()));
	}

    connect(filtersMapper, SIGNAL(mapped(int)), this, SLOT(applyFilterAt(int)));

	_zoomTitleLabel = new QLabel("Zoom:");
	_zoomLabel = new QLabel("100%");
	_zoomLabel->setAlignment(Qt::AlignRight);
	_zoomLabel->setFixedWidth(30);

	_zoomSlider = new QSlider(Qt::Horizontal);
	_zoomSlider->setTickInterval(25);
	_zoomSlider->setTickPosition(QSlider::TickPosition::TicksBelow);
	_zoomSlider->setMinimum(0);
	_zoomSlider->setMaximum(200);
	_zoomSlider->setValue(100);
	_zoomSlider->setTracking(true);
	connect(_zoomSlider, SIGNAL(valueChanged(int)), this, SLOT(setZoom(int)));

	auto zoomLayout = new QHBoxLayout();
	zoomLayout->addWidget(_zoomTitleLabel);
	zoomLayout->addWidget(_zoomSlider);
	zoomLayout->addWidget(_zoomLabel);

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
	mainLayout->addLayout(zoomLayout);
	mainLayout->addLayout(filtersLayout);

    auto centralWidget = new QWidget();
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    auto image = Image::load("../images/brain.png");
    setOriginalImage(image);

    setZoom(68);

    createMenu();
}

void MainWindow::setZoom(int zoom)
{
	auto scale = zoom / 100.0;

	_originalImageView->setScale(scale);
	_outputImageView->setScale(scale);

    if (_zoomSlider->value() != zoom)
        _zoomSlider->setValue(zoom);

	_zoomLabel->setText(QString("%1%").arg(zoom));
}

void MainWindow::applyFilterAt(int index)
{
	auto filters = ImageFilterManager::instance()->getFilters();

    if (index >= filters.size())
		return;

	auto filter = filters[index];
	applyFilter(filter);
}

void MainWindow::applyFilter(ImageFilter * filter)
{
    auto originalImage = _originalImageView->getImage();

    if (!originalImage)
    {
        QMessageBox::warning(this, "O filtro não pode ser aplicado", "A imagem original não foi carregada ou está danificada.", QMessageBox::Ok);
        return;
    }

    auto image = new Image(*originalImage);

    if (!filter->configure())
        return;

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
    _outputImageView->setImage(image);
}

void MainWindow::setEditImageEnabled(bool enabled)
{
	for (int i = 0; i < _filterButtonsGroup->buttons().count(); ++i)
	{
		auto button = _filterButtonsGroup->button(i);
		button->setEnabled(enabled);
	}

	_zoomSlider->setEnabled(enabled);
}

void MainWindow::setOriginalImage(Image *image)
{
    _originalImageView->setImage(image);

    if (!image)
    {
        _statusLabel->setText("Abra uma imagem para aplicar o filtro.");
		setEditImageEnabled(false);
    }
    else
    {
        QString status;
        status.sprintf("Imagem de tamanho %d x %d", image->getWidth(), image->getHeight());
        _statusLabel->setText(status);
		
		setEditImageEnabled(true);
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

        std::cout << "Abrindo imagem: \"" << file.toStdString() << "\"" << std::endl;

        auto image = Image::load(file.toStdString().c_str());

        if (image)
        {
            std::cout << "Imagem carregada com sucesso!" << std::endl;
        }
        else
        {
            std::cout << "Erro ao abrir a imagem..." << std::endl;
            QMessageBox::warning(this, tr("Erro ao abrir a imagem."), tr("A imagem não pôde ser aberta. Verifique se ela está em formato PNG e tente novamente."), QMessageBox::Ok);
        }

        setOriginalImage(image);
    }
}
