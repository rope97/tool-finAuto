#include "lib/AutomataView.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    AutomataView w;
    w.show();
    return a.exec();
}
