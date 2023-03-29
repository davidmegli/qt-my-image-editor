#include "QtWidgetsApplication1.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QImageWriter>


QtWidgetsApplication1::QtWidgetsApplication1(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    /*scene = new QGraphicsScene(this);
    //imposta la dimensione della vista alla dimensione attuale del contenitore
    //ui.graphicsView->fitInView(scene->sceneRect(), Qt::KeepAspectRatio);
    this->setCentralWidget(ui.graphicsView); //setta la vista come widget centrale
    
    ui.graphicsView->setScene(scene); //setta la scena alla vista
    QBrush brush(Qt::red);
    QPen pen(Qt::black);
    rect = scene->addRect(0, 0, 100, 100, pen, brush);*/
    paintArea = new PaintArea(this);
    
    //Codice per aggiungere le scrollbar
    QScrollArea* scrollArea = new QScrollArea(parent);
    scrollArea->setBackgroundRole(QPalette::Dark); //imposta il colore di sfondo del widget scrollArea a Dark
    scrollArea->setWidget(paintArea); //imposta il widget paintArea come widget interno di scrollArea
    //setCentralWidget(scrollArea); //setta scrollArea come widget centrale
    //this->layout()->addWidget(scrollArea);
    paintArea->setFixedSize(500, 500); //set the size of the paint area to 500x500
    
    setCentralWidget(scrollArea); //set the scroll area as the central widget

    //decomentare la riga successiva dopo aver rimosso il codice x la scrollArea, per ripristinare la paintArea come widget centrale
    //setCentralWidget(paintArea);
    createActions();
    createMenus();
    setWindowTitle(tr("Paint"));
    resize(500, 500);
}

void QtWidgetsApplication1::closeEvent(QCloseEvent* event)
{
    if (maybeSave()) {
		event->accept();
	}
    else {
		event->ignore();
	}
}

void QtWidgetsApplication1::open()
{
    if (maybeSave())
    {
		QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), QDir::currentPath());
        if (!fileName.isEmpty())
        {
			paintArea->openImage(fileName);
		}
	}
}

void QtWidgetsApplication1::save()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void QtWidgetsApplication1::penColor()
{
    QColor newColor = QColorDialog::getColor(paintArea->penColor());
    if (newColor.isValid())
    {
        paintArea->setPenColor(newColor);
    }
}

void QtWidgetsApplication1::penWidth()
{
	bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Paint"), tr("Select pen width:"),
        		paintArea->penWidth(), 1, 50, 1, &ok);
    if (ok)
    {
		paintArea->setPenWidth(newWidth);
	}
}

void QtWidgetsApplication1::about()
{
	QMessageBox::about(this, tr("About Paint"),
        tr("<p><b>Paint</b> is a small Qt application</p>"));
}

void QtWidgetsApplication1::createActions()
{
    openAct = new QAction(tr("Open"), this); //create the action for opening a file
    openAct->setShortcuts(QKeySequence::Open); //sets the shortcut to Ctrl+O
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    //create the menu entries for the different image formats
    foreach(QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction* action = new QAction(text, this);
        action->setData(format); //set the data of the action to the image format
        connect(action, SIGNAL(triggered()), this, SLOT(save())); //connect the action to the save slot
        saveAsActs.append(action); //add the action to the list of actions
    }
    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, SIGNAL(triggered()), this, SLOT(print()));
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    penColorAct = new QAction(tr("&Pen Color..."), this); //create the action for the pen color
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor())); //connect the action to the penColor slot
    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    clearScreenAct = new QAction(tr("&Clear Screen..."), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, SIGNAL(triggered()), this, SLOT(clearImage()));

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
}

void QtWidgetsApplication1::createMenus()
{
    saveAsMenu = new QMenu(tr("&Save As"), this);
    foreach(QAction* action, saveAsActs)
		saveAsMenu->addAction(action);
    fileMenu = new QMenu(tr("&File"), this);
    fileMenu->addAction(openAct);
    fileMenu->addMenu(saveAsMenu);
    fileMenu->addAction(printAct);
    fileMenu->addSeparator();
    fileMenu->addAction(exitAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool QtWidgetsApplication1::maybeSave()
{
    if (paintArea->isModified())
    {
		QMessageBox::StandardButton mb;
		mb = QMessageBox::warning(this, tr("Paint"),
            tr("The image has been modified.\n" "Do you want to save your changes?"),
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (mb == QMessageBox::Save)
        {
			return saveFile("png");
		}
        else if (mb == QMessageBox::Cancel)
        {
			return false;
		}
	}
    return true;
}

bool QtWidgetsApplication1::saveFile(const QByteArray &fileFormat)
{
    QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save As"),
        initialPath,
        tr("%1 Files(*.%2);; All Files(*)").arg(QString::fromLatin1(fileFormat.toUpper())).arg(QString::fromLatin1(fileFormat)));
    if (fileName.isEmpty())
        return false;
    else
        return paintArea->saveImage(fileName, fileFormat.constData());
}

QtWidgetsApplication1::~QtWidgetsApplication1()
{}
