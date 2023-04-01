#pragma once
#include "DrawCommand.h"
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <memory>
#include <QDebug>

using std::shared_ptr;

class DrawFreeHandCommand : public DrawCommand
{
public:
	//TODO: cancellare la command list al caricamento di una nuova immagine
	DrawFreeHandCommand(shared_ptr<QImage> image, QPoint startPoint, QColor penColor, int penWidth);
	void execute(QPoint destPoint) override;
	void execute() override;
	void undo() override;
	void redo() override;
	std::shared_ptr<DrawCommand> clone() const override;
	bool isCollapsible(shared_ptr<DrawCommand> command) const override;
	void collapse(shared_ptr<DrawCommand> command) override;
	~DrawFreeHandCommand();

private:
	QColor penColor;
	int penWidth;
	QVector<QPoint> points;
	shared_ptr<QImage> image;
};

