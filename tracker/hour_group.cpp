#include "hour_group.h"
#include "mainwindow.h"

hour_group::hour_group(MainWindow* parent):
    parent_(parent)
{
    //setFlag(QGraphicsItem::ItemIsMovable);
    setAcceptHoverEvents(true);
}


QRectF hour_group::boundingRect() const{
   //return QRectF(33,0,789,200);
    return QRectF(30,0,792,201);
}

void hour_group::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget){
    Q_UNUSED(option);
    Q_UNUSED(widget);


    QRectF rect = boundingRect();
    painter->setPen(pen_);
    painter->drawRect(rect);

}


void hour_group::mousePressEvent(QGraphicsSceneMouseEvent *event){
    QGraphicsItem::mousePressEvent(event);
}

void hour_group::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    pen_ = QPen(QColor("#2D33EF"));
    pen_.setWidth(4);
    update();
    QGraphicsItem::hoverEnterEvent(event);
}

void hour_group::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    pen_.setStyle(Qt::NoPen);
    update();
    QGraphicsItem::hoverLeaveEvent(event);
}


