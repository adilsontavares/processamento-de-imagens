#include "InputWindowFieldFloat.hpp"

#include <QDoubleValidator>

InputWindowFieldFloat::InputWindowFieldFloat(const QString& id, const QString& name) : InputWindowField(id, name, Type::Float)
{
    _spinBox = new QDoubleSpinBox();
    _spinBox->setMinimum(-1000000000L);
    _spinBox->setMaximum( 1000000000L);
    _spinBox->setAttribute(Qt::WA_MacShowFocusRect, false);
}

QWidget *InputWindowFieldFloat::getWidget()
{
    return _spinBox;
}

void InputWindowFieldFloat::getValue(void *&value)
{
    *((float*)value) = _spinBox->value();
}
