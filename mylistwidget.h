#ifndef MYLISTWIDGET_H
#define MYLISTWIDGET_H

#include <QObject>
#include <QWidget>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMouseEvent>

class QListWidget;
class QListWidgetItem;

class MyListWidget : public QListWidget
{
    Q_OBJECT
public:
    explicit MyListWidget(QWidget * parent = nullptr);
            ~MyListWidget();

    Q_SIGNAL void mouseRightClicked(QListWidgetItem * item);

protected:
    virtual void mousePressEvent(QMouseEvent * event);
};

#endif // MYLISTWIDGET_H
