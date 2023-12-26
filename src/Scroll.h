#ifndef SCROLL_H
#define SCROLL_H

#include "header.h"

class Scroll : public QWidget
{
    Q_OBJECT
public:
    explicit Scroll(QWidget *parent = nullptr);
    QLabel *textLabel;
    void animationStart(void);

private:
    QPropertyAnimation *topPropertyAnimation;

};

#endif // SCROLL_H
