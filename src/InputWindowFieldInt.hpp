#pragma once

#include "InputWindowField.hpp"

#include <QSpinBox>

class InputWindowFieldInt : public InputWindowField
{
private:

    QSpinBox *_spinBox;

public:

    InputWindowFieldInt(const QString& id, const QString& name);

    virtual QWidget *getWidget();
    virtual void getValue(void *&value);
};
