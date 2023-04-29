#ifndef DRAWRECTANGLECOMMAND_H
#define DRAWRECTANGLECOMMAND_H

#include "drawcommand.h"

#include <QPoint>
#include <memory>
#include <QImage>
#include <QPainter>
#include <QDebug>

class DrawRectangleCommand : public DrawCommand
{
public:
    DrawRectangleCommand(shared_ptr<QImage> image, QPoint startPoint, QColor penColor, int penWidth);
    void execute(QPoint destPoint) override;
    void execute() override;
    void undo() override;
    void redo() override;
    std::shared_ptr<DrawCommand> clone() const override;
    bool isCollapsible(std::shared_ptr<DrawCommand> command) const override;
    void collapse(std::shared_ptr<DrawCommand> command) override;
    ~DrawRectangleCommand();

private:
    QColor penColor;
    int penWidth;
    QPoint startPoint;
    QPoint endPoint;
    shared_ptr<QImage> image;
};


#endif // DRAWRECTANGLECOMMAND_H
