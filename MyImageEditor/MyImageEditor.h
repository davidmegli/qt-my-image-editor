    #pragma once

    #include <QtWidgets/QMainWindow>
    #include <QGraphicsScene>
    #include "ui_MyImageEditor.h"
    #include "PaintArea.h"
    #include <QScrollArea>

    class MyImageEditor : public QMainWindow
    {
        Q_OBJECT

    public:
        MyImageEditor(QWidget *parent = nullptr);
        ~MyImageEditor();

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
        QMenu* saveAsMenu;
        QMenu* optionMenu;
        QMenu* fileMenu;
        QMenu* editMenu;
        QMenu* helpMenu;
        QAction* openAct;
        QList<QAction *> saveAsActs;
        QAction* exitAct;
        QAction* penColorAct;
        QAction* penWidthAct;
        QAction* printAct;
        QAction* clearScreenAct;
        QAction* imageSizeAct;
        QAction* aboutAct;
        QAction* aboutQtAct;

        QToolBar* toolbar;
        QAction* drawFreeHandAct;
        QAction* drawRectangleAct;
        QAction* drawLineAct;
        QAction* drawEllipseAct;

        //TODO: Implement
        QAction *undoAct;
        QAction *redoAct;
        QAction *cutAct;
        QAction *copyAct;
        QAction *pasteAct;


        QScrollArea* scrollArea;

        Ui::MyImageEditorClass ui;
        QGraphicsScene* scene;
        QGraphicsRectItem *rect;
        QGraphicsEllipseItem* ellipse;
    };
