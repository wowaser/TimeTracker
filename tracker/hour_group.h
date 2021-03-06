#ifndef HOUR_GROUP_H
#define HOUR_GROUP_H

#include <QObject>
#include <QGraphicsItemGroup>
#include <QPainter>
#include <hour.h>

class MainWindow;
class hour;

class hour_group:public QGraphicsItemGroup
{
public:
    hour_group(MainWindow* parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);


    int brect_h;
    int brect_w;

    int num;

    QBrush brush_ = (Qt::white);
    QPen pen_ = QPen(Qt::white);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);


private:
    MainWindow* parent_;

};

#endif // HOUR_GROUP_H
