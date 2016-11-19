#include "ImageView.hpp"

#include <QImage>
#include <QPixmap>

ImageView::ImageView(QWidget *parent) : QScrollArea(parent)
{
	setAlignment(Qt::AlignCenter);

	_label = new QLabel();
	_label->setAlignment(Qt::AlignCenter);
	_label->setText(_title);

	_scale = 1;

	setWidget(_label);
}

void ImageView::setTitle(const QString& title)
{
	_title = title;
	
	_label->setText(title);
	_label->adjustSize();
}

void ImageView::setImage(Image * image)
{
	_image = image;

	updateLabel();
}

void ImageView::setScale(float scale)
{
	_scale = scale;
	updateLabel();
}

void ImageView::updateLabel()
{
	if (!_image)
	{
		_label->setText(_title);
		_label->adjustSize();
	}
	else
	{
		std::cout << "Criando QImage a partir da imagem..." << std::endl;
		auto qimage = QImage((uchar*)&_image->getData()[0], _image->getWidth(), _image->getHeight(), QImage::Format_RGBA8888);

		auto size = QSize(_image->getWidth() * _scale, _image->getHeight() * _scale);

		std::cout << "Criando QPixmap a partir da QImage..." << std::endl;
		auto pixmap = QPixmap::fromImage(qimage.scaled(size, Qt::KeepAspectRatio));

		std::cout << "QPixmap criado com sucesso!" << std::endl;

		_label->setPixmap(pixmap);
		_label->adjustSize();
	}
}

Image * ImageView::getImage()
{
	return _image;
}
