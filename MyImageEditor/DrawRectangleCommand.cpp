#include "DrawRectangleCommand.h"

DrawRectangleCommand::DrawRectangleCommand(shared_ptr<QImage> image, QPoint startPoint, QColor penColor, int penWidth)
{
	this->image = image;
	this->startPoint = startPoint;
	this->endPoint = startPoint;
	this->penColor = penColor;
	this->penWidth = penWidth;
}

void DrawRectangleCommand::execute(QPoint destPoint)
{
	qDebug() << "DrawRectangleCommand::execute(QPoint destPoint)";
	if (endPoint != destPoint) //if the new point is different from the previous one
	{
		qDebug() << "DrawRectangleCommand::execute(QPoint destPoint) - endPoint != destPoint";
		if (endPoint != startPoint) //if the rectangle is not empty (it has been drawn at least once)
		{
			qDebug() << "DrawRectangleCommand::execute(QPoint destPoint) - endPoint != startPoint";
			//delete the previous rectangle
			QPainter painter(image.get());
			painter.setPen(QPen(Qt::white, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
			painter.drawRect(QRect(startPoint, endPoint));
			endPoint = destPoint;
		}
		redo();
		endPoint = destPoint;
	}
}

void DrawRectangleCommand::execute()
{
	redo();
}

void DrawRectangleCommand::undo()
{
	qDebug() << "DrawRectangleCommand::undo()";
	if (endPoint != startPoint)
	{
		qDebug() << "DrawRectangleCommand::undo() - endPoint != startPoint";
		QPainter painter(image.get());
		painter.setPen(QPen(Qt::white, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter.drawRect(QRect(startPoint, endPoint));
	}
}

void DrawRectangleCommand::redo()
{
	qDebug() << "DrawRectangleCommand::redo()";
	if (endPoint != startPoint)
	{
		qDebug() << "DrawRectangleCommand::redo() - endPoint != startPoint";
		QPainter painter(image.get());
		painter.setPen(QPen(penColor, penWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
		painter.drawRect(QRect(startPoint, endPoint));

	}
}

std::shared_ptr<DrawCommand> DrawRectangleCommand::clone() const
{
	return std::make_shared<DrawRectangleCommand>(*this);
}

bool DrawRectangleCommand::isCollapsible(std::shared_ptr<DrawCommand> command) const
{
	bool sameType = typeid(*command.get()) == typeid(DrawRectangleCommand);
	if (sameType)
	{
		bool sameColor = penColor == dynamic_cast<DrawRectangleCommand*>(command.get())->penColor;
		bool sameWidth = penWidth == dynamic_cast<DrawRectangleCommand*>(command.get())->penWidth;
		bool sameStartPoint = startPoint == dynamic_cast<DrawRectangleCommand*>(command.get())->startPoint;
		return sameColor && sameWidth && sameStartPoint;
	}
	return false;
}

void DrawRectangleCommand::collapse(std::shared_ptr<DrawCommand> command)
{
	endPoint = dynamic_cast<DrawRectangleCommand*>(command.get())->endPoint;
}

DrawRectangleCommand::~DrawRectangleCommand()
{
}
