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
	//cancellare la command list al caricamento di una nuova immagine
	DrawFreeHandCommand(shared_ptr<QImage> image, QPoint startPoint, QColor penColor, int penWidth);
	void execute(QPoint destPoint) override;
	void execute() override;
	void undo() override;
	void redo() override;
	~DrawFreeHandCommand();

private:
	QColor penColor;
	int penWidth;
	QVector<QPoint> points;
	shared_ptr<QImage> image;
};

