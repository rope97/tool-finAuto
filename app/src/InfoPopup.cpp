#include "lib/InfoPopup.hpp"
#include "ui_InfoPopup.h"
#include <QtDebug>
#include <QFile>
#include <QDir>
#include <QString>
#include <QMessageBox>

Popup::Popup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Popup)
{
    ui->setupUi(this);
    connect(ui->listWidget, &QListWidget::itemClicked, this, &Popup::OnListWidgetItemClicked);
    if(ui->listWidget->isEnabled())
    {
        //initial tab selected and text shown
        showText(":/help/res/PopupTextCreation.txt");
    }
}

Popup::~Popup()
{
    delete ui;
}

void Popup::showText(std::string file)
{
    ui->textBrowser->clear();
    QFile inputFile(QString::fromStdString(file));
    if (inputFile.open(QIODevice::ReadOnly))
    {
       QTextStream in(&inputFile);
       while (!in.atEnd())
       {
          QString line = in.readLine();
          ui->textBrowser->append(line);
       }
       inputFile.close();
    }
    else
    {
        QMessageBox::information(this, tr("Error"), "Text cannot be loaded from the file, the file does not exist!");
    }

    //sets scroll to always be on top
    QTextCursor cursor = ui->textBrowser->textCursor();
    cursor.setPosition(0);
    ui->textBrowser->setTextCursor(cursor);
}

void Popup::OnListWidgetItemClicked(QListWidgetItem *item)
{
    if(ui->listWidget->item(0) == item)
    {
        showText(":/help/res/PopupTextCreation.txt");
    }
    else if(ui->listWidget->item(1) == item)
    {
        showText(":/help/res/PopupTextOperations.txt");
    }
    else if(ui->listWidget->item(2) == item)
    {
        showText(":/help/res/PopupTextTesting.txt");
    }
}


