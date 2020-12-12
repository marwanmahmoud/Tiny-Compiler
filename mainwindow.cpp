#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    Input = new  QPlainTextEdit("{ Sample program in TINY language computes factorial }\n"
                                "read x; {input an integer }\n"
                                "if 0 < x then { don’t compute if x <= 0 }\n"
                                "    fact := 1;\n"
                                "    repeat\n"
                                "        fact := fact * x;\n"
                                "        x := x - 1\n"
                                "    until x = 0;\n"
                                "    write fact { output factorial of x }\n"
                                "end ", this);
    Output = new QPlainTextEdit(this);
    Toolbar = new QToolBar(this);
    Splitter = new QSplitter(Qt::Horizontal,this);
    scanner = new Scanner();
    Output->setReadOnly(true);
    ui->setupUi(this);
}
void MainWindow::init_toolbar()
{
    this->addToolBar(Qt::TopToolBarArea,Toolbar);
    QAction * ScanAction = Toolbar->addAction("Scan");
    connect(ScanAction,SIGNAL(triggered(bool)),this,SLOT(start_Scan()));
    //signal



}
void MainWindow::init_ui()
{

    Splitter->addWidget(Input);
    Splitter->addWidget(Output);
    //to appear on the main window
    this->setCentralWidget(Splitter);

}
MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::start_Scan()
{
    QString result = this->scanner->getToken(Input->toPlainText().toStdString());
    qDebug()<<result;
    Output->setPlainText(result);
}

