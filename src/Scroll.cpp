#include "Scroll.h"

Scroll::Scroll(QWidget *parent) : QWidget{parent}
{
    // Initial
    textLabel = new QLabel(this);
    textLabel->setStyleSheet("QLabel {\
                            font-family: \"Microsoft YaHei\";\
                            font-size: 18px;\
                            color: red;\
                            font-style: normal;\
                            font-weight: bold;}");
    textLabel->setText("This is scroll text");
    textLabel->setFixedHeight(20);
    textLabel->hide();
    // Animation
    topPropertyAnimation = new QPropertyAnimation(this, "geometry");
    topPropertyAnimation->setDuration(600);
}

void Scroll::animationStart(void)
{ // The scroll can allows the item pop a read tip
    if (textLabel->isHidden())
        textLabel->show();
    topPropertyAnimation->setStartValue(QRect(0, textLabel->height(), textLabel->width(), textLabel->height()));
    topPropertyAnimation->setEndValue(QRect(0, 0, textLabel->width(), textLabel->height()));
    topPropertyAnimation->setEasingCurve(QEasingCurve::OutQuart);
    topPropertyAnimation->start(QAbstractAnimation::KeepWhenStopped);
    QTimer *timer = new QTimer(this);
    timer->start(3000);
    connect(timer, &QTimer::timeout, this, [=]{
        topPropertyAnimation->setStartValue(QRect(0, 0, textLabel->width(), textLabel->height()));
        topPropertyAnimation->setEndValue(QRect(0, textLabel->height(), textLabel->width(), textLabel->height()));
        topPropertyAnimation->start(QAbstractAnimation::KeepWhenStopped);
        timer->stop();
        delete timer;
    });
}

