#include "PaintArea.h"

#include <QtWidgets>
#if defined(QT_PRINTSUPPORT_LIB)
#include <QtPrintSupport/qtprintsupportglobal.h>
#if QT_CONFIG(printer)
#include <QPrinter>
#include <QPrintDialog>
#endif
#endif

PaintArea::PaintArea(QWidget* parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents); //root the widget to the top-left corner of its parent widget
	modified = false; //set the modified flag to false
	painting = false; //set the scribbling flag to false
	myPenWidth = 1;
	myPenColor = Qt::black;
}

bool PaintArea::openImage(const QString& fileName)
{
	QImage loadedImage; //a new image is created
	if (!loadedImage.load(fileName)) //the image is loaded from the file
		return false;

	QSize newSize = loadedImage.size().expandedTo(size()); //the size of the image is expanded to the size of the widget
	resizeImage(&loadedImage, newSize); //the image is resized to the new size
	image = loadedImage; //the image is assigned to the image member variable
	modified = false; //the modified flag is set to false
	update(); //the widget is updated
	return true;
}

bool PaintArea::saveImage(const QString& filename, const char* fileFormat)
{
	QImage visibleImage = image;
	resizeImage(&visibleImage, size());
	if (visibleImage.save(filename, fileFormat))
	{
		modified = false;
		return true;
	}
	else
		return false;
}

void PaintArea::setPenColor(const QColor& newColor)
{
	myPenColor = newColor;
}

void PaintArea::setPenWidth(int newWidth)
{
	myPenWidth = newWidth;
}

void PaintArea::clearImage()
{
	image.fill(qRgb(255, 255, 255)); //fills the image with white
	modified = true; //sets the modified flag to true
	update(); //update the widget
}

void PaintArea::mousePressEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton) //if the left mouse button is pressed
	{
		lastPoint = event->pos(); //store the position of the mouse cursor
		painting = true;
	}
}

void PaintArea::mouseMoveEvent(QMouseEvent* event)
{
	if ((event->buttons() & Qt::LeftButton) && painting) //if the left mouse button is pressed and the painting flag is true
		drawLineTo(event->pos()); //draw a line from the last point to the current point
}

void PaintArea::mouseReleaseEvent(QMouseEvent* event)
{
	if (event->button() == Qt::LeftButton && painting) //if the left mouse button is released and the painting flag is true
	{
		drawLineTo(event->pos()); //draw a line from the last point to the current point
		painting = false; //set the painting flag to false
	}
}

void PaintArea::paintEvent(QPaintEvent* event)
{
	QPainter painter(this); //create a painter object
	QRect rect = event->rect(); //get the rectangle that needs to be updated
	painter.drawImage(rect, image, rect); //draw the image on the widget
}

void PaintArea::resizeEvent(QResizeEvent* event)
{
	if (width() > image.width() || height() > image.height()) //if the widget is resized to a larger size
	{
		int newWidth = qMax(width() + 128, image.width()); //calculate the new width
		int newHeight = qMax(height() + 128, image.height()); //calculate the new height
		resizeImage(&image, QSize(newWidth, newHeight)); //resize the image
		update(); //update the widget
	}
	QWidget::resizeEvent(event);
}

void PaintArea::drawLineTo(const QPoint& endPoint)
{
	QPainter painter(&image); //create a painter object
	painter.setPen(QPen(myPenColor, myPenWidth, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin)); //set the pen
	painter.drawLine(lastPoint, endPoint); //draw a line from the last point to the current point
	modified = true; //set the modified flag to true
	int rad = (myPenWidth / 2) + 2; //calculate the radius of the circle
	update(QRect(lastPoint, endPoint).normalized().adjusted(-rad, -rad, +rad, +rad)); //update the widget
	lastPoint = endPoint; //set the last point to the current point
}

void PaintArea::resizeImage(QImage* image, const QSize& newSize)
{
	if (image->size() == newSize) //if the size of the image is the same as the new size
		return;

	QImage newImage(newSize, QImage::Format_RGB32); //create a new image
	newImage.fill(qRgb(255, 255, 255)); //fill the image with white
	QPainter painter(&newImage); //create a painter object
	painter.drawImage(QPoint(0, 0), *image); //draw the image on the new image
	*image = newImage; //assign the new image to the image member variable
}

void PaintArea::print()
{
#if QT_CONFIG(printdialog) //if the print dialog is available
	QPrinter printer(QPrinter::HighResolution); //create a printer object
	QPrintDialog printDialog(&printer, this); //create a print dialog object and set the printer object as the parent
	if (printDialog.exec() == QDialog::Accepted)
	{
		QPainter painter(&printer); //create a painter object and set the printer object as the parent
		QRect rect = painter.viewport(); //get the rectangle of the painter
		QSize size = image.size(); //get the size of the image
		size.scale(rect.size(), Qt::KeepAspectRatio); //scale the size of the image to the size of the rectangle
		painter.setViewport(rect.x(), rect.y(), size.width(), size.height()); //set the viewport of the painter
		painter.setWindow(image.rect()); //set the window of the painter
		painter.drawImage(0, 0, image); //draw the image on the painter
	}
#endif
}

PaintArea::~PaintArea()
{
}