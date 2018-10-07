#ifndef SCRIBBLEAREA_H
#define SCRIBBLEAREA_H

#include <QColor>
#include <QImage>
#include <QPoint>
#include <QWidget>

class ScribbleArea : public QWidget
{
	Q_OBJECT;
public:
	ScribbleArea(QWidget *parent = 0);
	
	bool openImage(const QString &fileName);
	bool saveImage(const QString &fileName, const char*filFormat);
	void setPenColor(const QColor &newColor);
	void setPenWidth(int newWidth);
	void setPenStyle(int newStyle);


	bool isModified() const { return modified; }
	QColor penColor() const { return myPenColor; }
	int penWidth() const { return myPenWidth; }
	int penStyle() const { return myPenStyle; }

	public slots:
	void clearImage();

protected:
	void mousePressEvent(QMouseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;
	void mouseReleaseEvent(QMouseEvent *event) override;
	void paintEvent(QPaintEvent *event) override;
	void resizeEvent(QResizeEvent *event) override;

private:
	void drawLineTo(const QPoint &endPoint);
	void drawRectTo(const QPoint &endPoint);
	void resizeImage(QImage *image, const QSize &newSize);

	bool modified;
	bool scribbling;
	int myPenWidth;
	int myPenStyle;
	QColor myPenColor;
	QImage image;
	QImage tempImage;
	QPoint lastPoint;
};


#endif // SCRIBBLEAREA_H
