#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QDialog>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>
#include <QSpinBox>
#include <QGroupBox>
#include "chessfield.h"

class mainWindow : public QDialog
{
    Q_OBJECT

public:
    explicit mainWindow(QWidget *parent = 0);

private:
    //frame for chessboard
    QFrame* chessFrame;
    QGridLayout* chessLayout;

    //spin box to changing chessboard's size
    QSpinBox* spinBox;
    QGroupBox* groupBox;
    QHBoxLayout* groupLayout;

    //button which after clicked starts computing chassboard polynomial
    QPushButton* computeButton;

    //chessboard fields
    chessField** chessBoard;

    //chessboard size
    int size;

    //FUNCTIONS

    void setup_chessboard_frame();
    void create_chessboard(int _size);

    //deletes previous chessboard
    void del_fields();

    //spin box
    void setup_spinBox();

    //compute button
    void setup_computeButton();

    //calculates polynomial
    void compute(auto& fields, QStack<QPoint>& marked, int *result, int rightDepth);

protected:
    void resizeEvent(QResizeEvent*);

private slots:
    //size of the chessboard changed in the spinBox
    void onSpinBoxChanged();

    //calculate polynomial
    void onComputeClicked();

};

#endif // MAINWINDOW_H
