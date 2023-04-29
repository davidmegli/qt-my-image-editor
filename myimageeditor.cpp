#include "myimageeditor.h"
#include "./ui_myimageeditor.h"
#include <QFileDialog>
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QImageWriter>
#include <QToolBar>


MyImageEditor::MyImageEditor(QWidget *parent)
    : QMainWindow(parent)
{
    ui->setupUi(this);

    paintArea = new PaintArea(this);

    // Crea la toolbar
    toolbar = new QToolBar("My Toolbar", this);
    // Aggiunge la toolbar alla finestra principale
    addToolBar(Qt::LeftToolBarArea, toolbar);

    //Codice per aggiungere le scrollbar
    QScrollArea* scrollArea = new QScrollArea(parent);

    scrollArea->setWidgetResizable(true); //imposta la scrollArea come ridimensionabile
    paintArea->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

    scrollArea->setBackgroundRole(QPalette::Dark); //imposta il colore di sfondo del widget scrollArea a Dark
    scrollArea->setWidget(paintArea); //imposta il widget paintArea come widget interno di scrollArea
    //setCentralWidget(scrollArea); //setta scrollArea come widget centrale
    //this->layout()->addWidget(scrollArea);
    paintArea->setFixedSize(PaintArea::DEF_WIDTH, PaintArea::DEF_HEIGHT); //set the size of the paint area to 500x500
    //paintArea->resize(500, 500);


    //TODO : nel paint della window devo adattare la dimensione della paintArea alla dimensione dell'immagine, oppure nella resize dell'immagine


    setCentralWidget(scrollArea); //set the scroll area as the central widget

    //decomentare la riga successiva dopo aver rimosso il codice x la scrollArea, per ripristinare la paintArea come widget centrale
    //setCentralWidget(paintArea);
    createActions();
    createMenus();
    setWindowTitle(tr("Paint"));
    resize(PaintArea::DEF_WIDTH, PaintArea::DEF_HEIGHT);
}

void MyImageEditor::closeEvent(QCloseEvent* event)
{
    if (maybeSave()) {
        event->accept();
    }
    else {
        event->ignore();
    }
}

void MyImageEditor::open()
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

void MyImageEditor::save()
{
    QAction* action = qobject_cast<QAction*>(sender());
    QByteArray fileFormat = action->data().toByteArray();
    saveFile(fileFormat);
}

void MyImageEditor::penColor()
{
    QColor newColor = QColorDialog::getColor(paintArea->penColor());
    if (newColor.isValid())
    {
        paintArea->setPenColor(newColor);
    }
}

void MyImageEditor::penWidth()
{
    bool ok;
    int newWidth = QInputDialog::getInt(this, tr("Paint"), tr("Select pen width:"),
                paintArea->penWidth(), 1, 50, 1, &ok);
    if (ok)
    {
        paintArea->setPenWidth(newWidth);
    }
}

void MyImageEditor::about()
{
    QMessageBox::about(this, tr("About Paint"),
        tr("<p><b>Paint</b> is a small Qt application</p>"));
}

void MyImageEditor::resizeImage()
{
    int currentSize = 1024;//paintArea->image().size().width();
    bool ok;
    int width = QInputDialog::getInt(this, tr("Image Size"),
        tr("Width:"), currentSize, 0, 10000000, 1, &ok);
    if (ok)
    {
        int height = QInputDialog::getInt(this, tr("Image Size"),
            tr("Height:"), currentSize, 0, 10000000, 1, &ok);
        if (ok)
        {
            QSize qSize = QSize(width, height);
            paintArea->resizeImage(qSize);
        }
    }
}

void MyImageEditor::createActions()
{
    openAct = new QAction(tr("Open"), this); //create the action for opening a file
    openAct->setShortcuts(QKeySequence::Open); //sets the shortcut to Ctrl+O
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    //creates the menu entries for the different image formats
    foreach(QByteArray format, QImageWriter::supportedImageFormats())
    {
        QString text = tr("%1...").arg(QString(format).toUpper());
        QAction* action = new QAction(text, this);
        action->setData(format); //set the data of the action to the image format
        connect(action, SIGNAL(triggered()), this, SLOT(save())); //connect the action to the save slot
        saveAsActs.append(action); //add the action to the list of actions
    }
    printAct = new QAction(tr("&Print..."), this);
    connect(printAct, &QAction::triggered, paintArea, &PaintArea::print);

    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcuts(QKeySequence::Quit);
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    undoAct = new QAction(tr("&Undo"), this);
    undoAct->setShortcuts(QKeySequence::Undo);
    connect(undoAct, &QAction::triggered, paintArea, &PaintArea::undo);

    redoAct = new QAction(tr("&Redo"), this);
    redoAct->setShortcuts(QKeySequence::Redo);
    connect(redoAct, &QAction::triggered, paintArea, &PaintArea::redo);

    penColorAct = new QAction(tr("&Pen Color..."), this);
    connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));
    penWidthAct = new QAction(tr("Pen &Width..."), this);
    connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

    imageSizeAct = new QAction(tr("&Image Size..."), this);
    connect(imageSizeAct, SIGNAL(triggered()), this, SLOT(resizeImage()));

    clearScreenAct = new QAction(tr("&Clear Screen..."), this);
    clearScreenAct->setShortcut(tr("Ctrl+L"));
    connect(clearScreenAct, &QAction::triggered, paintArea, &PaintArea::clearImage);

    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    // Azione strumento disegno a mano libera
    drawFreeHandAct = new QAction(QIcon("/icons/freehand_32.png"), tr("&Draw FreeHand"), this);
    //drawFreeHandAct = new QAction(tr("&Draw FreeHand"), this);
    drawFreeHandAct->setShortcut(tr("F"));
    drawFreeHandAct->setStatusTip(tr("Draw FreeHand"));
    connect(drawFreeHandAct, &QAction::triggered, paintArea, &PaintArea::drawFreeHand);

    // Azione strumento disegno linea
    drawLineAct = new QAction(QIcon("/icons/line_32.png"), tr("&Draw Line"), this);
    drawLineAct->setShortcut(tr("L"));
    drawLineAct->setStatusTip(tr("Draw Line"));
    connect(drawLineAct, &QAction::triggered, paintArea, &PaintArea::drawLine);

    // Azione strumento disegno rettangolo
    drawRectangleAct = new QAction(QIcon("/icons/rectangle_32.png"), tr("&Draw Rectangle"), this);
    drawRectangleAct->setShortcut(tr("R"));
    drawRectangleAct->setStatusTip(tr("Draw Rectangle"));
    connect(drawRectangleAct, &QAction::triggered, paintArea, &PaintArea::drawRectangle);

    // Azione strumento disegno ellisse
    drawEllipseAct = new QAction(QIcon("/icons/ellipse_32.png"), tr("&Draw Ellipse"), this);
    drawEllipseAct->setShortcut(tr("E"));
    drawEllipseAct->setStatusTip(tr("Draw Ellipse"));
    connect(drawEllipseAct, &QAction::triggered, paintArea, &PaintArea::drawEllipse);

}

void MyImageEditor::createMenus()
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

    editMenu = new QMenu(tr("&Edit"), this);
    editMenu->addAction(undoAct);
    editMenu->addAction(redoAct);

    optionMenu = new QMenu(tr("&Options"), this);
    optionMenu->addAction(penColorAct);
    optionMenu->addAction(penWidthAct);
    optionMenu->addSeparator();
    optionMenu->addAction(imageSizeAct);
    optionMenu->addSeparator();
    optionMenu->addAction(clearScreenAct);

    helpMenu = new QMenu(tr("&Help"), this);
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);

    toolbar->addAction(drawFreeHandAct);
    toolbar->addAction(drawLineAct);
    toolbar->addAction(drawRectangleAct);
    toolbar->addAction(drawEllipseAct);

    menuBar()->addMenu(fileMenu);
    menuBar()->addMenu(editMenu);
    menuBar()->addMenu(optionMenu);
    menuBar()->addMenu(helpMenu);
}

bool MyImageEditor::maybeSave()
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

bool MyImageEditor::saveFile(const QByteArray &fileFormat)
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

MyImageEditor::~MyImageEditor()
{
    delete ui;
}

