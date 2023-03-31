#pragma once
#include "DrawCommand.h"
#include <QPoint>
#include <memory>
#include <QImage>
#include <QPainter>
#include <QDebug>

class DrawEllipseCommand : public DrawCommand
{
public:
	DrawEllipseCommand(shared_ptr<QImage> image, QPoint startPoint, QColor penColor, int penWidth);
	void execute(QPoint destPoint) override;
	void execute() override;
	void undo() override;
	void redo() override;
	std::shared_ptr<DrawCommand> clone() const override;
	bool isCollapsible(std::shared_ptr<DrawCommand> command) const override;
	void collapse(std::shared_ptr<DrawCommand> command) override;
	~DrawEllipseCommand();
private:
	QColor penColor;
	int penWidth;
	QPoint startPoint;
	QPoint endPoint;
	shared_ptr<QImage> image;
};

