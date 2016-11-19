#include "InputWindowField.hpp"

InputWindowField::InputWindowField(const QString& id, const QString& name, Type type) : QWidget(0)
{
    _id = id;
    _name = name;
    _type = type;
}

const QString& InputWindowField::getId()
{
    return _id;
}

const QString& InputWindowField::getName()
{
    return _name;
}

InputWindowField::Type InputWindowField::getType()
{
    return _type;
}
