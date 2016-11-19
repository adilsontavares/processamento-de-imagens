#pragma once

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QButtonGroup>

#include "Image.hpp"
#include "ImageView.hpp"

class MainWindow : public QMainWindow
{
private:

	QLabel *_statusLabel;

	ImageView *_originalImageView;
	ImageView *_outputImageView;

	QButtonGroup *_filterButtonsGroup;

	QLabel *_zoomTitleLabel;
	QSlider *_zoomSlider;
	QLabel *_zoomLabel;

	void createMenu();

	void setOriginalImage(Image *image);
	void setOutputImage(Image *image);

	void setEditImageEnabled(bool enabled);

	Q_OBJECT

public:

	explicit MainWindow(QWidget *parent = 0);

private slots:

public slots:

	void openFile();

	void applyFilterAt(int index);
	void applyFilter(ImageFilter *filter);

	void setZoom(int zoom);
};