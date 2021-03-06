#include "hour.h"
#include "mainwindow.h"


hour::hour(MainWindow* parent):
    parent_(parent)
{
    setAcceptHoverEvents(true);
}


QRectF hour::boundingRect() const{
    return QRectF(0,0,hour_width,hour_height);
}

void hour::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);

    QRectF rect = boundingRect();
    painter->fillRect(rect,brush_);
    //painter->drawRoundedRect(rect, 5,5);
    painter->setPen(pen_);
    painter->drawRect(rect);

}

void hour::change_color(QColor clr){
    brush_ = clr;
    update();
}

void hour::slot_change_clr(const QColor &clr){
    brush_ = clr;
    update();
}


void hour::mousePressEvent(QGraphicsSceneMouseEvent *event){
    prev_brush_ = brush_;
    if(event->button() == Qt::LeftButton){
        QColor new_color = this->parent_->selected_clr;
        if(new_color != "Invalid"){//Dx
            change_color(new_color);
            update();
        }
    }
    else{
        change_color(def_color);
        update();
    }
    emit clickSave();
    emit update_color(brush_.color());
    emit update_piechart( brush_.color(), prev_brush_.color());

    QGraphicsRectItem::mousePressEvent(event);
}

void hour::hoverEnterEvent(QGraphicsSceneHoverEvent *event){
    pen_ = QPen(QColor("#0080ff"));
    update();
    QGraphicsRectItem::hoverEnterEvent(event);
}

void hour::hoverLeaveEvent(QGraphicsSceneHoverEvent *event){
    pen_ = QPen(Qt::black);
    update();
    QGraphicsRectItem::hoverLeaveEvent(event);
}


