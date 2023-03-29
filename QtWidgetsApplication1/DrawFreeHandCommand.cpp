#include "DrawFreeHandCommand.h"

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
	painter.setPen(QPen(Qt::white, penWidth));
	for (int i = 1; i < points.size(); i++)
	{
		painter.drawLine(points[i - 1], points[i]);
	}
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
}

DrawFreeHandCommand::~DrawFreeHandCommand()
{
}
