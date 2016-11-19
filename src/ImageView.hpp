#pragma once

#include <QScrollArea>
#include <QLabel>
#include <QString>

#include "Image.hpp"

class ImageView : public QScrollArea
{
private:

	QLabel *_label;
	Image *_image;
	QString _title;
	float _scale;

	void updateLabel();

    Q_OBJECT

public:

	explicit ImageView(QWidget *parent = 0);

	void setTitle(const QString& title);

	void setImage(Image *image);
	Image *getImage();

	void setScale(float scale);
};
