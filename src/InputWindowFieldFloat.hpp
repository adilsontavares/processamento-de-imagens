#pragma once

#include "InputWindowField.hpp"

#include <QDoubleSpinBox>

class InputWindowFieldFloat : public InputWindowField
{
private:

    QDoubleSpinBox *_spinBox;

public:

    InputWindowFieldFloat(const QString& id, const QString& name);

    virtual QWidget *getWidget();
    virtual void getValue(void *&value);
};
