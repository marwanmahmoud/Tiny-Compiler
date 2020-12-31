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
                                "end", this);
    ScannerOutput = new QPlainTextEdit(this);
    Toolbar = new QToolBar(this);
    Splitter = new QSplitter(Qt::Horizontal,this);
    scanner = new Scanner();
    parser = Parser();
    parsetree =new ParseTree();

    //BrowseButton = new QPushButton(tr("Browse"), this);
    ScannerOutput->setReadOnly(true);
    ui->setupUi(this);
}
void MainWindow::init_toolbar()
{
    this->addToolBar(Qt::TopToolBarArea,Toolbar);
    QAction * BrowseAction = Toolbar->addAction("Browse");
    connect(BrowseAction,SIGNAL(triggered(bool)),this,SLOT(browse()));
    QAction * ScanAction = Toolbar->addAction("Scan");
    connect(ScanAction,SIGNAL(triggered(bool)),this,SLOT(start_Scan()));
    QAction * ParseAction = Toolbar->addAction("Parse");
    connect(ParseAction,SIGNAL(triggered(bool)),this,SLOT(parse()));

}
void MainWindow::init_ui()
{
    QGroupBox *inGb = new QGroupBox("Input Program");
    QGroupBox *scannerGb = new QGroupBox("Scanner Output");

    inGb->setLayout(new QHBoxLayout);
    inGb->layout()->addWidget(Input);
    scannerGb->setLayout(new QHBoxLayout);
    scannerGb->layout()->addWidget(ScannerOutput);
    Splitter->addWidget(inGb);
    Splitter->addWidget(scannerGb);
    Splitter->addWidget(parsetree);
    //to appear on the main window
    this->setCentralWidget(Splitter);
    QList<int> sizes;
    sizes << 100 << 100 << 400;
    Splitter->setSizes(sizes);
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::start_Scan()
{
    QString result = this->scanner->getToken(Input->toPlainText().toStdString());
    qDebug()<<result;
    ScannerOutput->setPlainText(result);
}
void MainWindow::browse()
{
    QString directory = QFileDialog::getOpenFileName(this, tr("Browsig Files"),
                                                    "QDir::currentPath()",
                                                    tr("Text (*.txt)"));
    QString result = this->scanner->getStringFile(directory);
    Input->setPlainText(result);

}
void MainWindow::parse()
{
    parser._3bas(ScannerOutput->toPlainText());
    QVector<Node> nodes = parser.get_nodes();
    QVector<Edge> edges = parser.get_edges();
    for(auto edge: edges){
        if(edge.isFriend)
            parsetree->addEdgeFriend(edge.x1, edge.y1, edge.x2, edge.y2);
        else if(!edge.isFriend)
            parsetree->addEdgeChild(edge.x1, edge.y1, edge.x2, edge.y2);
    }
    for(auto node: nodes){
        if (node.Rect)
            parsetree->addRectangleNode(node.x, node.y, QString::fromStdString(node.text));
        if(!node.Rect)
            parsetree->addEllipseNode(node.x, node.y, QString::fromStdString(node.text));
        qDebug() << QString::fromStdString(node.text) << "\n";
    }
}
