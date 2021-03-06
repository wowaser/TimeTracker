#ifndef DAY_H
#define DAY_H

#include <QWidget>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QObject>
#include <vector>
#include <cmath>


class MainWindow;

class hour: public QObject, public QGraphicsRectItem
{
    Q_OBJECT
    Q_INTERFACES()
public:

    hour(MainWindow* parent = nullptr);

    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

    int hour_width = 30;
    int hour_height = 200;

    void change_color(QColor clr);
    QBrush brush_ = (Qt::white);
    QBrush prev_brush_;
    QColor def_color = Qt::white;

    QPen pen_ = QPen(Qt::black);

private:
    MainWindow* parent_;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

signals:
    void clickSave();
    void update_color(const QColor&);
    void update_piechart(const QColor&, const QColor&);

public slots:
    void slot_change_clr(const QColor& clr);
};


#endif // DAY_H
