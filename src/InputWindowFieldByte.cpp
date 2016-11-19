#include "InputWindowFieldByte.hpp"

InputWindowFieldByte::InputWindowFieldByte(const QString& id, const QString& name) : InputWindowField(id, name, Type::Byte)
{
    _spinBox = new QSpinBox();
    _spinBox->setMinimum(0);
    _spinBox->setMaximum(255);

    _spinBox->setAttribute(Qt::WA_MacShowFocusRect, false);
}

QWidget *InputWindowFieldByte::getWidget()
{
    return _spinBox;
}

void InputWindowFieldByte::getValue(void *&value)
{
    *((char*)value) = _spinBox->value();
}
