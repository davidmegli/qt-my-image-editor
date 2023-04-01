#include "DrawEllipseCommand.h"

DrawEllipseCommand::DrawEllipseCommand(shared_ptr<QImage> image, QPoint startPoint, QColor penColor, int penWidth)
{
	this->image = image;
	this->startPoint = startPoint;
	this->endPoint = startPoint;
	this->penColor = penColor;
	this->penWidth = penWidth;
}

void DrawEllipseCommand::execute(QPoint destPoint)
{

	qDebug() << "DrawRectangleCommand::execute(QPoint destPoint)";
	if (endPoint != destPoint) //if the new point is different from the previous one
	{
		if (endPoint != startPoint) //if the rectangle is not empty (it has been drawn at least once)
		{
			//delete the previous rectangle
			QPainter painter(image.get());
			painter.setPen(QPen(Qt::white, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter.drawEllipse(QRect(startPoint, endPoint));
			endPoint = destPoint;
		}
		redo();
		endPoint = destPoint;
	}
}

void DrawEllipseCommand::execute()
{
	redo();
}

void DrawEllipseCommand::undo()
{
	if (endPoint != startPoint)
	{
		QPainter painter(image.get());
		painter.setPen(QPen(Qt::white, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter.drawEllipse(QRect(startPoint, endPoint));
	}
}

void DrawEllipseCommand::redo()
{
	if (endPoint != startPoint)
	{
		QPainter painter(image.get());
		painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter.drawEllipse(QRect(startPoint, endPoint));

	}
}

std::shared_ptr<DrawCommand> DrawEllipseCommand::clone() const
{
	return std::make_shared<DrawEllipseCommand>(*this);
}

bool DrawEllipseCommand::isCollapsible(std::shared_ptr<DrawCommand> command) const
{
	bool sameType = typeid(*command.get()) == typeid(DrawEllipseCommand);
	if (sameType)
	{
		bool sameColor = penColor == dynamic_cast<DrawEllipseCommand*>(command.get())->penColor;
		bool sameWidth = penWidth == dynamic_cast<DrawEllipseCommand*>(command.get())->penWidth;
		bool sameStartPoint = startPoint == dynamic_cast<DrawEllipseCommand*>(command.get())->startPoint;
		return sameColor && sameWidth && sameStartPoint;
	}
	return false;
}

void DrawEllipseCommand::collapse(std::shared_ptr<DrawCommand> command)
{
	endPoint = dynamic_cast<DrawEllipseCommand*>(command.get())->endPoint;
}

DrawEllipseCommand::~DrawEllipseCommand()
{
}
