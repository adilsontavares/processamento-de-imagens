#include "InputWindowFieldString.hpp"

InputWindowFieldString::InputWindowFieldString(const QString& id, const QString& name) : InputWindowField(id, name, Type::String)
{
    _lineEdit = new QLineEdit();
    _lineEdit->setAttribute(Qt::WA_MacShowFocusRect, false);
}

QWidget *InputWindowFieldString::getWidget()
{
    return _lineEdit;
}

void InputWindowFieldString::getValue(void *&value)
{
    *((QString*)value) = _lineEdit->text();
}
