#pragma once

#include <QWidget>

class InputWindowField : public QWidget
{
public:

    enum Type
    {
        Bool,
        Byte,
        Int,
        Float,
        String
    };

private:

    QString _id;
    QString _name;
    Type _type;

    Q_OBJECT

protected:

    InputWindowField(const QString& id, const QString& name, Type type);

public:

    const QString& getId();
    const QString& getName();
    Type getType();

    virtual QWidget *getWidget() = 0;
    virtual void getValue(void *&value) = 0;
};
