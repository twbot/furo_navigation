#ifndef MAP_H
#define MAP_H

#include <QDialog>
#include <string>

#include <QLabel>
#include <QScrollArea>
#include <QWidget>
#include <QString>
#include <QTableWidget>

namespace Ui {
class Map;
}

class Map : public QDialog
{
    Q_OBJECT

public:
    explicit Map(QWidget *parent = 0, int choice=10);
    ~Map();

private slots:

private:
    Ui::Map *ui;

    const int mapWidth = 48;
    const int mapHeight = 30;

};

#endif // MAP_H
