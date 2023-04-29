#include "drawfreehandcommand.h"

DrawFreeHandCommand::DrawFreeHandCommand(shared_ptr<QImage> image, QPoint startPoint, QColor penColor, int penWidth)
{
    qDebug() << "DrawFreeHandCommand Constructor";
    this->image = image;
    points.append(startPoint);
    this->penColor = penColor;
    this->penWidth = penWidth;
}

void DrawFreeHandCommand::execute()
{
    qDebug() << "DrawFreeHandCommand execute";
    redo();
}

void DrawFreeHandCommand::execute(QPoint destPoint)
{
    qDebug() << "DrawFreeHandCommand execute(QPoint destPoint)";
    points.append(destPoint); //solo nel caso di freehand, per linee e forme geometriche non devo salvare tutti i punti
    //ma solo il punto di partenza e quello di arrivo, in quel caso devo cancellare il disegno precedente prima del redo()
    redo();
}

void DrawFreeHandCommand::undo()
{
    qDebug() << "DrawFreeHandCommand undo";
    QPainter painter(image.get());
    //FIXME: colorare di bianco sull'undo cancella eventuali linee sottostanti
    //potrebbe essere non risolvibile, se non salvando l'immagine originale
    painter.setPen(QPen(Qt::white, penWidth));
    for (int i = 1; i < points.size(); i++)
    {
        painter.drawLine(points[i - 1], points[i]);
    }
    qDebug() << points.size() << " white lines drawn";
}

void DrawFreeHandCommand::redo()
{
    qDebug() << "DrawFreeHandCommand redo";
    QPainter painter(image.get());
    painter.setPen(QPen(penColor, penWidth));
    for (int i = 1; i < points.size(); i++)
    {
        painter.drawLine(points[i - 1], points[i]);
    }
    qDebug() << points.size() << " lines drawn";
}

std::shared_ptr<DrawCommand> DrawFreeHandCommand::clone() const
{
    qDebug() << "DrawFreeHandCommand::clone()";
    return std::make_shared<DrawFreeHandCommand>(*this);
}

bool DrawFreeHandCommand::isCollapsible(shared_ptr<DrawCommand> command) const
{
    bool sameType = typeid(*command.get()) == typeid(DrawFreeHandCommand);
    if (sameType)
    {
        bool sameColor = dynamic_cast<DrawFreeHandCommand*>(command.get())->penColor == this->penColor;
        bool sameWidth = dynamic_cast<DrawFreeHandCommand*>(command.get())->penWidth == this->penWidth;
        bool sameImage = dynamic_cast<DrawFreeHandCommand*>(command.get())->image == this->image;
        bool isnewLine = dynamic_cast<DrawFreeHandCommand*>(command.get())->points.size() <= 2;
        return sameColor && sameWidth && sameImage && !isnewLine;
    }
    return false;
}

DrawFreeHandCommand::~DrawFreeHandCommand()
{
}

void DrawFreeHandCommand::collapse(shared_ptr<DrawCommand> command)
{
    this->points.append(dynamic_cast<DrawFreeHandCommand*>(command.get())->points.last());
}

