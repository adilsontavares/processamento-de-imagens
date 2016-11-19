#include "InputWindow.hpp"

#include <QLabel>
#include <QPushButton>

#include "InputWindowFieldBool.hpp"
#include "InputWindowFieldByte.hpp"
#include "InputWindowFieldInt.hpp"
#include "InputWindowFieldString.hpp"
#include "InputWindowFieldFloat.hpp"

InputWindow::InputWindow(QWidget *parent) : QDialog(parent)
{
    _titleLayout = new QVBoxLayout();
    _fieldLayout = new QVBoxLayout();

    auto layout = new QVBoxLayout();

    auto inputLayout = new QHBoxLayout();
    inputLayout->addLayout(_titleLayout);
    inputLayout->addLayout(_fieldLayout);

    auto button = new QPushButton("Continuar");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(accept()));

    layout->addLayout(inputLayout);
    layout->addWidget(button);

    setLayout(layout);

    window()->layout()->setSizeConstraint(QLayout::SetFixedSize);
}

void InputWindow::addField(const QString &id, const QString &name, InputWindowField::Type type)
{
    if (fieldExists(id))
        return;

    InputWindowField *field = 0;

    switch (type)
    {
    case InputWindowField::Type::Bool:
        field = new InputWindowFieldBool(id, name);
        break;

    case InputWindowField::Type::Byte:
        field = new InputWindowFieldByte(id, name);
        break;

    case InputWindowField::Type::Int:
        field = new InputWindowFieldInt(id, name);
        break;

    case InputWindowField::Type::String:
        field = new InputWindowFieldString(id, name);
        break;

    case InputWindowField::Float:
        field = new InputWindowFieldFloat(id, name);
        break;
    }

    if (!field)
        return;

    auto fieldHeight = 25;

    auto label = new QLabel(name);
    label->setFixedHeight(fieldHeight);

    auto input = field->getWidget();
    input->setFixedHeight(fieldHeight);

    _titleLayout->addWidget(label);
    _fieldLayout->addWidget(input);

    _fields.push_back(field);
}

bool InputWindow::getBool(const QString &id)
{
    bool value;

    if (!getValue(id, InputWindowField::Type::Bool, &value))
        return false;

    return value;
}

char InputWindow::getByte(const QString &id)
{
    char value;

    if (!getValue(id, InputWindowField::Type::Byte, &value))
        return false;

    return value;
}

int InputWindow::getInt(const QString &id)
{
    int value;

    if (!getValue(id, InputWindowField::Type::Int, &value))
        return false;

    return value;
}

float InputWindow::getFloat(const QString &id)
{
    float value;

    if (!getValue(id, InputWindowField::Type::Float, &value))
        return false;

    return value;
}

QString InputWindow::getString(const QString &id)
{
    QString value;

    if (!getValue(id, InputWindowField::Type::String, &value))
        return "";

    return value;
}

bool InputWindow::getValue(const QString &id, InputWindowField::Type type, void *value)
{
    auto field = getField(id);

    if (!field || field->getType() != type)
        return false;

    field->getValue(value);

    return true;
}

InputWindowField *InputWindow::getField(const QString &id)
{
    for (auto field : _fields)
        if (field->getId() == id)
            return field;

    return 0;
}

bool InputWindow::fieldExists(const QString &id)
{
    return getField(id) != 0;
}
