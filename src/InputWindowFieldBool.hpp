#pragma once

#include "InputWindowField.hpp"

#include <QCheckBox>

class InputWindowFieldBool : public InputWindowField
{
private:

    QCheckBox *_checkbox;

public:

    InputWindowFieldBool(const QString& id, const QString& name);

    virtual QWidget *getWidget();
    virtual void getValue(void *&value);
};
