#pragma once


#include <memory>
#include <QWidget>
#include <QImage>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>
#include "CommandManager.h"

using std::shared_ptr;

class PaintArea : public QWidget
{
public:
	PaintArea(QWidget *parent = nullptr);
	~PaintArea();

	bool openImage(const QString &fileName);
	bool saveImage(const QString &fileName, const char *fileFormat);
	void setPenColor(const QColor &newColor);
	void setPenWidth(int newWidth);

	bool isModified() const { return modified; }
	QColor penColor() const { return myPenColor; }
	int penWidth() const { return myPenWidth; }
	void resizeImage(QSize& newSize);

	static const int DEF_WIDTH = 1280;
	static const int DEF_HEIGHT = 720;

public slots:
	void clearImage();
	void print();

protected:
	void mousePressEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	void drawLineTo(const QPoint &endPoint);
	void resizeImage(shared_ptr<QImage> image, const QSize& newSize);

	bool modified;
	bool painting;
	int myPenWidth;
	QColor myPenColor;
	shared_ptr<QImage> image;
	QPoint lastPoint;
	CommandManager commandManager;
	shared_ptr<DrawCommand> currentCommand;

};

