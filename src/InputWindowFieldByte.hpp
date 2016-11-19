#pragma once

#include "InputWindowField.hpp"

#include <QSpinBox>

class InputWindowFieldByte : public InputWindowField
{
private:

    QSpinBox *_spinBox;

public:

    InputWindowFieldByte(const QString& id, const QString& name);

    virtual QWidget *getWidget();
    virtual void getValue(void *&value);
};
