#include <QtWidgets>
#include "ScribbleArea.h"

ScribbleArea::ScribbleArea(QWidget *parent)
	: QWidget(parent)
{
	setAttribute(Qt::WA_StaticContents);
	modified = false;
	scribbling = false;
	myPenWidth = 1;
	myPenStyle = 4;//Pen, 0->ERASER
	myPenColor = Qt::black;
}

bool ScribbleArea::openImage(const QString &fileName)
{
	QImage loadedImage;
	if (!loadedImage.load(fileName)) 
		return false;

	QSize newSize = loadedImage.size().expandedTo(size());
	resizeImage(&loadedImage, newSize);
	image = loadedImage;
	modified = false;
	update();
	return true;
}

bool ScribbleArea::saveImage(const QString &filename, const char *fileFormat)
{
	QImage visibleImage = image;
	resizeImage(&visibleImage, size());

	if (visibleImage.save(filename, fileFormat)) {
		modified = false;
		return true;
	} 
	else {
		return false;
	}
}

void ScribbleArea::setPenColor(const QColor &newcolor)
{
	myPenColor = newcolor;
}

void ScribbleArea::setPenWidth(int newWidth)
{
	myPenWidth = newWidth;
}

void ScribbleArea::setPenStyle(int newStyle)
{
	myPenStyle = newStyle;
}

void ScribbleArea::clearImage()
{
	image.fill(qRgb(255, 255, 255));
	modified = true;
	update();
}

void ScribbleArea::mousePressEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton)
	{
		lastPoint = event->pos();
		scribbling = true;
	}
}

void ScribbleArea::mouseMoveEvent(QMouseEvent *event)
{
	if ((event->buttons() & Qt::LeftButton) && scribbling) {
		if (myPenStyle == 0 || myPenStyle == 1) {
			drawLineTo(event->pos());
		}
		else {
			tempImage = image;
			drawRectTo(event->pos());
		}
	}
}

void ScribbleArea::mouseReleaseEvent(QMouseEvent *event)
{
	if (event->button() == Qt::LeftButton && scribbling){
		scribbling = false;
		if (myPenStyle == 0 || myPenStyle == 1) {
			drawLineTo(event->pos());
		}
		else {
			drawRectTo(event->pos());
		}
	}
}

void ScribbleArea::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	QRect dirtyRect = event->rect();////////////////////////////////////
	if (scribbling &&(myPenStyle==2|| myPenStyle == 3 || myPenStyle == 4)) {
		painter.drawImage(dirtyRect, tempImage, dirtyRect);
	}
	else {
		painter.drawImage(dirtyRect, image, dirtyRect);
	}
	
}

void ScribbleArea::drawLineTo(const QPoint &endP)
{
	QPainter painter(&image);
	painter.setPen(QPen((myPenStyle ? myPenColor : Qt::white), myPenWidth, 
		Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	painter.drawLine(lastPoint, endP);
	update();
	modified = true;

	int rad = (myPenWidth / 2) + 2;
	update(QRect(lastPoint, endP).normalized()
		.adjusted(-rad, -rad, +rad, +rad));//////////////
	lastPoint = endP;
}

void ScribbleArea::drawRectTo(const QPoint &endP)
{
	QPainter painter(&(scribbling?tempImage:image));
	painter.setPen(QPen((myPenStyle ? myPenColor : Qt::white), myPenWidth,
		Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
	switch (myPenStyle)
	{
	case 2: {
		QLine line(lastPoint, endP);
		painter.drawLine(line);
		break;
	}
	case 3: {
		QRect rect(lastPoint, endP);
		painter.drawRect(rect);
		break;
	}
	case 4: {
		QPoint midP;
		midP.setX((endP.x() + lastPoint.x()) / 2);
		midP.setY((endP.y() + lastPoint.y()) / 2);
		painter.drawEllipse(midP, (endP.x() - lastPoint.x()) / 2,
			(endP.y() - lastPoint.y()) / 2);
		break;
	}
	default:
		break;
	}
	update();
	modified = true;

	int rad = (myPenWidth / 2) + 2;
	update(QRect(lastPoint, endP).normalized()
		.adjusted(-rad, -rad, +rad, +rad));//////////////
}

void ScribbleArea::resizeEvent(QResizeEvent *event)////////////////////
{
	if (width() > image.width() || height() > image.height())
	{
		int newWidth = qMax(width() + 128, image.width());
		int newHight = qMax(height() + 128, image.height());
		resizeImage(&image, QSize(newWidth, newHight));
		update();
	}
	QWidget::resizeEvent(event);
}

void ScribbleArea::resizeImage(QImage *image, const QSize &newSize)
{
	if (image->size() == newSize)
		return;

	QImage newImage(newSize, QImage::Format_RGB32);
	newImage.fill(qRgb(255, 255, 255));
	QPainter painter(&newImage);
	painter.drawImage(QPoint(0, 0), *image);
	*image = newImage;
}