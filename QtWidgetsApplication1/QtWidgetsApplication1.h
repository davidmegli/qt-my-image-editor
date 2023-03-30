    #pragma once

    #include <QtWidgets/QMainWindow>
    #include <QGraphicsScene>
    #include "ui_QtWidgetsApplication1.h"
    #include "PaintArea.h"
    #include <QScrollArea>

    class QtWidgetsApplication1 : public QMainWindow
    {
        Q_OBJECT

    public:
        QtWidgetsApplication1(QWidget *parent = nullptr);
        ~QtWidgetsApplication1();

    protected:
        void closeEvent(QCloseEvent *event) override;

    private slots:
        void open();
        void save();
        void penColor();
        void penWidth();
        void about();
        void resizeImage();

    private:
        void createActions();
        void createMenus();
        bool maybeSave();
        bool saveFile(const QByteArray &fileFormat);
        PaintArea* paintArea;
        QMenu *saveAsMenu;
        QMenu* optionMenu;
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *helpMenu;
        QAction *openAct;
        QList<QAction *> saveAsActs;
        QAction *exitAct;
        QAction *penColorAct;
        QAction *penWidthAct;
        QAction *printAct;
        QAction *clearScreenAct;
        QAction *imageSizeAct;
        QAction* aboutAct;
        QAction* aboutQtAct;

        //TODO: Implement
        QAction *undoAct;
        QAction *redoAct;
        QAction *cutAct;
        QAction *copyAct;
        QAction *pasteAct;


        QScrollArea* scrollArea;

        Ui::QtWidgetsApplication1Class ui;
        QGraphicsScene* scene;
        QGraphicsRectItem *rect;
        QGraphicsEllipseItem* ellipse;
    };
