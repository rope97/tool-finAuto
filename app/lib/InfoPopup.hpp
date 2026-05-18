#ifndef POPUP_HPP
#define POPUP_HPP

#include <QDialog>
#include <QListWidget>

namespace Ui {
class Popup;
}

class Popup : public QDialog
{
    Q_OBJECT

public:
    explicit Popup(QWidget *parent = nullptr);
    ~Popup();

private:
    void OnListWidgetItemClicked(QListWidgetItem *item);
    void showText(std::string file);

private:
    Ui::Popup *ui;
};

#endif // POPUP_HPP
