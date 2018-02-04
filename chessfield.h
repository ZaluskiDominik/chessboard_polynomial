#ifndef CHESSFIELD_H
#define CHESSFIELD_H

#include <QAbstractButton>
#include <QMouseEvent>
#include <QFrame>
#include <QTimer>

class chessField : public QAbstractButton
{
    Q_OBJECT
public:
    chessField(QWidget* parent=nullptr);
    bool isMarked()
    {
        return clicked;
    }

private:
    bool clicked;
    int animState;
    QFrame* frame;
    QTimer* animationTimer;

protected:
    void paintEvent(QPaintEvent*);
    void mousePressEvent(QMouseEvent* e);

private slots:
    void onTimeout();
};

#endif // CHESSFIELD_H
