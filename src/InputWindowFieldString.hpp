#pragma once

#include "InputWindowField.hpp"

#include <QLineEdit>

class InputWindowFieldString : public InputWindowField
{
private:

    QLineEdit *_lineEdit;

public:

    InputWindowFieldString(const QString& id, const QString& name);

    virtual QWidget *getWidget();
    virtual void getValue(void *&value);
};
