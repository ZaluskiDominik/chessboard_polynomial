#include "chessfield.h"
#include <QPainter>
#include <algorithm>

chessField::chessField(QWidget* parent)
    :QAbstractButton(parent)
{
    clicked=false;
    animationTimer=nullptr;

    //set up frame of the button
    frame=new QFrame(this);
    frame->setLineWidth(1);
    frame->setMidLineWidth(2);
    frame->setFrameStyle(QFrame::Raised | QFrame::Box);
}

void chessField::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen(Qt::red);
    pen.setWidth(10);

    //fill background
    painter.fillRect(0, 0, width(), height(), QColor(211,211,211));

    if (clicked)
    {
        //field is taken
        //paint cross
        painter.setPen(pen);
        painter.drawLine(0, 0, std::min( width()*animState/10, width() ), std::min( height()*animState/10, height() ));
        if (animState>10)
            painter.drawLine(width(), 0, width()/10*(20-animState), height()*(animState-10)/10);
    }

    //resize frame
    frame->setGeometry(0, 0, width(), height());
}

void chessField::mousePressEvent(QMouseEvent *e)
{
    if (e->button()==Qt::LeftButton)
    {
        //change state of the field on left click
        if (!clicked)
        {
            //mark field
            animationTimer=new QTimer(this);
            animationTimer->start(20);
            connect (animationTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
            animState=0;
        }
        else if (animationTimer!=nullptr)
        {
            animationTimer->deleteLater();
            animationTimer=nullptr;
        }
        clicked=!clicked;
        e->accept();
    }
}

void chessField::onTimeout()
{
    animState+=1;
    if (animState>=20)
    {
        animationTimer->deleteLater();
        animationTimer=nullptr;
    }
    update();
}
