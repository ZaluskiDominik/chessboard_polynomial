#include "mainwindow.h"
#include <QStack>
#include <vector>
#include <QDebug>

mainWindow::mainWindow(QWidget *parent) :
    QDialog(parent, Qt::WindowCloseButtonHint | Qt::WindowTitleHint)
{
    chessBoard=nullptr;

    //resizing window
    setMinimumSize(400, 400);
    resize(600, 600);

    //set title and icon
    setWindowTitle("Chessboard Polynomial Calculator");

    setup_chessboard_frame();
    setup_spinBox();
    setup_computeButton();

    //resize event
    updateGeometry();

    create_chessboard(spinBox->value());

    //connect signals to slots
    connect (computeButton, SIGNAL(clicked(bool)), this, SLOT(onComputeClicked()));
    connect (spinBox, SIGNAL(valueChanged(int)), this, SLOT(onSpinBoxChanged()));
}

void mainWindow::setup_chessboard_frame()
{
    chessFrame=new QFrame(this);
    chessLayout=new QGridLayout;

    //creating frame look
    chessFrame->setFrameStyle(QFrame::Sunken | QFrame::Box);
    chessFrame->setLineWidth(1);
    chessFrame->setMidLineWidth(0);
    chessFrame->show();

    //setting layout
    chessFrame->setLayout(chessLayout);
    chessLayout->setContentsMargins(5, 5, 5, 5);
    chessLayout->setSpacing(0);
}

void mainWindow::create_chessboard(int _size)
{
    del_fields();
    //set new chessboard size
    size=_size;

    //create new chessboard
    chessBoard=new chessField* [size];
    for (int i=0 ; i<size ; i++)
    {
        chessBoard[i]=new chessField[size];
        for (int j=0 ; j<size ; j++)
        {
            chessLayout->addWidget(&chessBoard[i][j], i, j);
            chessBoard[i][j].setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
        }
    }
}

void mainWindow::del_fields()
{
    if (chessBoard!=nullptr)
    {
        for (int i=0 ; i<size ; i++)
            delete [] chessBoard[i];
        delete [] chessBoard;
    }
}

void mainWindow::setup_spinBox()
{
    groupBox=new QGroupBox(this);
    groupLayout=new QHBoxLayout;
    spinBox=new QSpinBox;

    groupBox->setTitle("Resize chessboard");
    groupBox->setLayout(groupLayout);

    groupLayout->addWidget(spinBox);

    //init value
    spinBox->setValue(4);
    //set boundaries to chessboard size
    spinBox->setMaximum(8);
    spinBox->setMinimum(1);
}

void mainWindow::setup_computeButton()
{
    computeButton=new QPushButton(this);
    computeButton->setText("Calculate");

    //set text font
    QFont f("Arial", 12);
    f.setBold(true);
    computeButton->setFont(f);
}

void mainWindow::compute(auto& fields, QStack<QPoint> &marked, int* result, int rightDepth)
{
    int row=-1, col;
    //find not market field of chessboard
    for (int i=0 ; i<size && row==-1; i++)
    {
        for (int j=0 ; j<size ; j++)
        {
            if (!fields[i][j])
            {
                //empty field
                row=i;
                col=j;
                fields[i][j]=true;
                break;
            }
        }
    }

    if (row==-1)
    {
        //all fields are marked
        result[rightDepth]++;
        return;
    }

    //recurion calls

    //left call
    compute(fields, marked, result, rightDepth);

    //mark all fields that are in the same column/row as original marked field
    marked.push(QPoint(-1, -1));    //Point (-1, -1) means end of local set of fields
    for (int i=0 ; i<size ; i++)
    {
        if (!fields[row][i])
            marked.push(QPoint(row, i));
        if (!fields[i][col])
            marked.push(QPoint(i, col));
        fields[row][i]=fields[i][col]=true;
    }

    //right call
    compute(fields, marked, result, ++rightDepth);

    //unmark all fields from marked stack
    fields[row][col]=false;
    while (marked.top().x()!=-1)
    {
        fields[marked.top().x()][marked.top().y()]=false;
        marked.pop();
    }
    marked.pop();
}

void mainWindow::resizeEvent(QResizeEvent *)
{
    //resizing groupBox with spinBox
    groupBox->setGeometry(5, 5, 150, 60);

    //resizing computeBotton
    computeButton->setGeometry(groupBox->width() + 5 + ((width() - 10 - groupBox->width())/2) - 100, 5, 200, groupBox->height());

    //resizing chessboard frame
    chessFrame->setGeometry(5, groupBox->height() + 15, width() - 10, height() - groupBox->height() - 20);
}

void mainWindow::onSpinBoxChanged()
{
    //create new chessboard
    create_chessboard(spinBox->value());
}

void mainWindow::onComputeClicked()
{
    QStack<QPoint> markedFields;
    std::vector<std::vector<bool>> fields;
    //array for coefficients of the chessboard polynomial
    int polynomial[size]={0};

    fields.resize(size);
    for (int i=0 ; i<size ; i++)
    {
        fields[i].resize(size);
        for (int j=0 ; j<size ; j++)
            fields[i][j]=chessBoard[i][j].isMarked();
    }

    //calculate polynomial
    compute(fields, markedFields, polynomial, 0);

    for (int i=0 ; i<size ; i++)
        qDebug()<<i<<": "<<polynomial[i];
}
