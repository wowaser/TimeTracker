#include "colorbutton.h"
#include <QPainter>
#include <QDebug>

ColorButton::ColorButton(QWidget*&){
    setAutoFillBackground(true);
}
/*
void ColorButton::paintEvent(QPaintEvent *e){

    //QPainter painter(this);
    //painter.fillRect(this->rect(), btn_color);


    QPushButton::paintEvent(e);
}
*/
void ColorButton::mousePressEvent(QMouseEvent *event){
    QColor clr = this->palette().button().color();
    emit pressed(clr);
    QPushButton::mousePressEvent(event);
}

//  SLOT
void ColorButton::set_color(QColor &clr){
    QPalette pal = palette();
    pal.setColor(QPalette::Button, clr);
    setAutoFillBackground(true);
    setPalette(pal);
    update();
    emit selected_clr_changed(clr);
}


/*
void ColorButton::enterEvent(QEvent *){
       QPalette pal = palette();
       pal.setColor(QPalette::Button, QColor(Qt::blue));
       setPalette(pal);
       update();
}

void ColorButton::leaveEvent(QEvent *){
    QPalette pal = palette();
    pal.setColor(QPalette::Button, QColor(Qt::red));
    setPalette(pal);
    update();
}
*/
