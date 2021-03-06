#include "colorbutton.h"

ColorButton::ColorButton(){
    setAutoFillBackground(true);
}

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
