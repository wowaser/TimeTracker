#ifndef COLORBUTTON_H
#define COLORBUTTON_H

#include <QPushButton>
#include <QObject>
#include <QWidget>

class ColorButton : public QPushButton
{
    Q_OBJECT
public:
    ColorButton(QWidget*&);
    //void paintEvent(QPaintEvent *e);

protected:
    void mousePressEvent(QMouseEvent *event);

    //virtual void enterEvent(QEvent *);
    //virtual void leaveEvent(QEvent *);
    //virtual void hoverEvent(QEvent*);
signals:
    void pressed(QColor&);
    void selected_clr_changed(QColor&);

public slots:
    void set_color(QColor& clr);
};

#endif // COLORBUTTON_H
