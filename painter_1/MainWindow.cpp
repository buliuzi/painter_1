#include <QtWidgets>

#include "MainWindow.h"
#include "ScribbleArea.h"

MainWindow::MainWindow()
{
	scribbleArea = new ScribbleArea;
	setCentralWidget(scribbleArea);

	createActions();
	createMenus();
	createStatus();
	createToolBar();

	setWindowTitle(tr("Scribble"));
	resize(1200, 800);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
	if (maybeSave()) {
		event->accept();
	}
	else {
		event->ignore();
	}
}

void MainWindow::mouseMoveEvent(QMouseEvent *event) 
{
	labelMousePos->setText(" (" + QString::number(event->x()) + "," + QString::number(event->y()) + ")");
	//update();
}

void MainWindow::open()
{
	if (maybeSave()) {
		QString fileName = QFileDialog::getOpenFileName(this, 
			tr("Open File"), QDir::currentPath());/////////////////
		if (!fileName.isEmpty())
			scribbleArea->openImage(fileName);
	}
}
void MainWindow::save()
{
	QAction *action = qobject_cast<QAction *>(sender());
	QByteArray fileFormat = action->data().toByteArray();
	saveFile(fileFormat);
}

void MainWindow::penColor()
{
	QColor newColor = QColorDialog::getColor(scribbleArea->penColor());
	if (newColor.isValid())
		scribbleArea->setPenColor(newColor);
}
void MainWindow::penWidth()
{
	bool ok;
	int newWidth = QInputDialog::getInt(this, tr("Pick Color"),
		tr("Select pen width:"), scribbleArea->penWidth(),
		1, 50, 1, &ok);
	if (ok)
		scribbleArea->setPenWidth(newWidth);
}

void MainWindow::about()
{
	QMessageBox::about(this, tr("About Scribble"),
		tr("<p>Designer: ChengRan</p><p>Class: CS1607</p>"));
}

void MainWindow::createActions()
{
	openAct = new QAction(tr("&Open..."), this);
	openAct->setShortcuts(QKeySequence::Open);
	connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

	foreach(QByteArray format, QImageWriter::supportedImageFormats())
	{
		QString text = tr("%1...").arg(QString(format).toUpper());

		QAction *action = new QAction(text, this);
		action->setData(format);
		connect(action, SIGNAL(triggered()), this, SLOT(save()));
		saveAsActs.append(action);
	}

	exitAct = new QAction(tr("E&xit..."), this);
	openAct->setShortcuts(QKeySequence::Quit);
	connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

	penColorAct = new QAction(tr("&Pen Color..."), this);
	connect(penColorAct, SIGNAL(triggered()), this, SLOT(penColor()));

	penWidthAct = new QAction(tr("&Pen Width..."), this);
	connect(penWidthAct, SIGNAL(triggered()), this, SLOT(penWidth()));

	penStyleAct = new QAction(tr("&Pen Style..."), this);
	connect(penStyleAct, SIGNAL(triggered()), this, SLOT(penStyle()));

	clearScreenAct = new QAction(tr("&Clear Screen..."), this);
	clearScreenAct->setShortcut(tr("Ctrl+L"));
	connect(clearScreenAct, SIGNAL(triggered()), scribbleArea, SLOT(clearImage()));

	aboutAct = new QAction(tr("&About..."), this);
	connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

	
	
}

void MainWindow::drawActTriggered()
{
	scribbleArea->setPenStyle(1);
}
void MainWindow::drawLineActTriggered()
{
	scribbleArea->setPenStyle(2);
}
void MainWindow::drawRectActTriggered()
{
	scribbleArea->setPenStyle(3);
}
void MainWindow::drawElliActTriggered()
{
	scribbleArea->setPenStyle(4);
}
void MainWindow::eraseActTriggered()
{
	scribbleArea->setPenStyle(0);
}

void MainWindow::createToolBar()
{
	QToolBar *bar = this->addToolBar("Tools");
	QActionGroup *group = new QActionGroup(bar);

	QAction *drawAct = new QAction("Scribble", bar);
	drawAct->setToolTip(tr("Scribble."));
	drawAct->setStatusTip(tr("Scribble."));
	drawAct->setCheckable(true);
	drawAct->setChecked(true);
	group->addAction(drawAct);
	bar->addAction(drawAct);

	QAction *drawLineAct = new QAction("Line", bar);
	drawLineAct->setToolTip(tr("Draw a line."));
	drawLineAct->setStatusTip(tr("Draw a line."));
	drawLineAct->setCheckable(true);
	drawLineAct->setChecked(true);
	group->addAction(drawLineAct);
	bar->addAction(drawLineAct);

	QAction *drawRectAct = new QAction("Rectangle", bar);
	drawRectAct->setToolTip(tr("Draw a Rectangle."));
	drawRectAct->setStatusTip(tr("Draw a Rectangle."));
	drawRectAct->setCheckable(true);
	drawRectAct->setChecked(true);
	group->addAction(drawRectAct);
	bar->addAction(drawRectAct);

	QAction *drawElliAct = new QAction("Ellipse", bar);
	drawElliAct->setToolTip(tr("Draw a Ellipse."));
	drawElliAct->setStatusTip(tr("Draw a Ellipse."));
	drawElliAct->setCheckable(true);
	drawElliAct->setChecked(true);
	group->addAction(drawElliAct);
	bar->addAction(drawElliAct);

	QAction *eraseAct = new QAction("Erase", bar);
	eraseAct->setToolTip(tr("Erase."));
	eraseAct->setStatusTip(tr("Erase."));
	eraseAct->setCheckable(true);
	eraseAct->setChecked(true);
	group->addAction(eraseAct);
	bar->addAction(eraseAct);

	connect(drawAct, SIGNAL(triggered()), this, SLOT(drawActTriggered()));
	connect(drawLineAct, SIGNAL(triggered()), this, SLOT(drawLineActTriggered()));
	connect(drawRectAct, SIGNAL(triggered()), this, SLOT(drawRectActTriggered()));
	connect(drawElliAct, SIGNAL(triggered()), this, SLOT(drawElliActTriggered()));
	connect(eraseAct, SIGNAL(triggered()), this, SLOT(eraseActTriggered()));

}

void MainWindow::createMenus()
{
	saveAsMenu = new QMenu(tr("&Save as..."), this);
	foreach(QAction *action, saveAsActs)
		saveAsMenu->addAction(action);

	fileMenu = new QMenu(tr("&File"), this);
	fileMenu->addAction(openAct);
	fileMenu->addMenu(saveAsMenu);
	fileMenu->addSeparator();
	fileMenu->addAction(exitAct);

	optionMenu = new QMenu(tr("&Option"), this);
	optionMenu->addAction(penColorAct);
	optionMenu->addAction(penWidthAct);
	optionMenu->addSeparator();
	optionMenu->addAction(clearScreenAct);

	helpMenu = new QMenu(tr("&Help"), this);
	helpMenu->addAction(aboutAct);

	menuBar()->addMenu(fileMenu);
	menuBar()->addMenu(optionMenu);
	menuBar()->addMenu(helpMenu);
}

void MainWindow::createStatus()
{
	labelStatus = new QLabel();
	labelStatus->setMinimumSize(0, 30);
	labelStatus->setText(tr("Mouse Position:"));
	labelStatus->setFixedWidth(250);
	labelMousePos = new QLabel();
	labelMousePos->setText(tr(""));
	labelMousePos->setFixedWidth(200);
	statusBar()->addPermanentWidget(labelStatus);
	statusBar()->addPermanentWidget(labelMousePos);
	//this->setCentralWidget->setMouseTracking(true);
	this->setMouseTracking(true);
	//setAttribute(Qt::WA_Hover, true);
}

bool MainWindow::maybeSave()
{
	if (scribbleArea->isModified())
	{
		QMessageBox::StandardButton ret;
		ret = QMessageBox::warning(this, tr("Scribble"),
			tr("The image has been modified.\n"
				"Do you want to save your changes?"),
			QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
		if (ret == QMessageBox::Save) {
			return saveFile("png");
		}
		else if (ret == QMessageBox::Cancel) {
			return false;
		}
	}
	return true;
}

bool MainWindow::saveFile(const QByteArray &fileFormat)
{
	QString initialPath = QDir::currentPath() + "/untitled." + fileFormat;

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save as"),
		initialPath,
		tr("%1 Files (*.%2);;All Files(*)")
		.arg(QString::fromLatin1(fileFormat.toUpper()))
		.arg(QString::fromLatin1(fileFormat)));
	if (fileName.isEmpty()) {
		return false;
	}
	else {
		return scribbleArea->saveImage(fileName, fileFormat.constData());
	}
}