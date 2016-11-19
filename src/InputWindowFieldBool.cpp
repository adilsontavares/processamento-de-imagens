#include "InputWindowFieldBool.hpp"

InputWindowFieldBool::InputWindowFieldBool(const QString& id, const QString& name) : InputWindowField(id, name, Type::Bool)
{
    _checkbox = new QCheckBox();
}

QWidget *InputWindowFieldBool::getWidget()
{
    return _checkbox;
}

void InputWindowFieldBool::getValue(void *&value)
{
    *((bool*)value) = _checkbox->isChecked();
}
