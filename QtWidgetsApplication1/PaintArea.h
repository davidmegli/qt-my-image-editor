#pragma once

#include <QWidget>

#include <QImage>
#include <QPoint>
#include <QColor>
#include <QPen>
#include <QPainter>
#include <QPaintEvent>

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
	void resizeImage(QImage* image, const QSize& newSize);

	bool modified;
	bool painting;
	int myPenWidth;
	QColor myPenColor;
	QImage image;
	QPoint lastPoint;

};

