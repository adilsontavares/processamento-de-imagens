#include "InputWindowFieldInt.hpp"

#include <QIntValidator>

InputWindowFieldInt::InputWindowFieldInt(const QString& id, const QString& name) : InputWindowField(id, name, Type::Int)
{
    _spinBox = new QSpinBox();
    _spinBox->setMinimum(0);
    _spinBox->setMaximum(INT_MAX);
    _spinBox->setAttribute(Qt::WA_MacShowFocusRect, false);
}

QWidget *InputWindowFieldInt::getWidget()
{
    return _spinBox;
}

void InputWindowFieldInt::getValue(void *&value)
{
    *((int*)value) = _spinBox->value();
}
