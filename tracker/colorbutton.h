#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QWidget>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton();



protected:
    //void mousePressEvent(QMouseEvent *event);

    virtual void enterEvent(QEvent *);
    virtual void leaveEvent(QEvent *);
    //virtual void hoverEvent(QEvent*);
};

#endif // COLORBUTTON_H
