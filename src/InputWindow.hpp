#pragma once

#include "InputWindowField.hpp"

#include <QDialog>
#include <QString>
#include <QVBoxLayout>

#include <iostream>
#include <vector>

class InputWindow : public QDialog
{
private:

    QVBoxLayout *_titleLayout;
    QVBoxLayout *_fieldLayout;

    std::vector<InputWindowField*> _fields;

    Q_OBJECT

    bool getValue(const QString& id, InputWindowField::Type type, void *value);

public:

    explicit InputWindow(QWidget *parent = 0);

    void addField(const QString& id, const QString& name, InputWindowField::Type type);

    bool fieldExists(const QString& id);

    bool getBool(const QString& id);
    char getByte(const QString& id);
    int getInt(const QString& id);
    float getFloat(const QString& id);
    QString getString(const QString& id);

    InputWindowField *getField(const QString& id);
};
