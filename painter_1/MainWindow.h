#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QList>
#include <QLabel>
#include <QMainWindow>
#include <QActionGroup>

class ScribbleArea;

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow();

protected:
	void closeEvent(QCloseEvent *event) override;
	void mouseMoveEvent(QMouseEvent *event) override;

	private slots:
	void open();
	void save();
	void penColor();
	void penWidth();
	void about();
	void drawActTriggered();
	void drawLineActTriggered();
	void drawRectActTriggered();
	void drawElliActTriggered();
	void eraseActTriggered();

private:
	void createActions();
	void createMenus();
	void createStatus();
	void createToolBar();
	bool maybeSave();
	bool saveFile(const QByteArray &fileFormat);

	ScribbleArea *scribbleArea;

	QToolBar *bar;
	QActionGroup *group;
	QAction *drawAct;
	QAction *drawLineAct;
	QAction *drawRectAct;
	QAction *drawElliAct;
	QAction *eraseAct;

	QLabel *labelStatus;
	QLabel *labelMousePos;

	QMenu *saveAsMenu;
	QMenu *fileMenu;
	QMenu *optionMenu;
	QMenu *helpMenu;

	QAction *openAct;
	QList<QAction *> saveAsActs;
	QAction *exitAct;
	QAction *penColorAct;
	QAction *penWidthAct;
	QAction *penStyleAct;
	QAction *clearScreenAct;
	QAction *aboutAct;
};

#endif // !MAINWINDOW_H
