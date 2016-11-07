#ifndef MAINWINDOW_H
#define	MAINWINDOW_H

#include "QSnake.h"
#include <QtGui>
#include <QMenuBar>
#include <QMainWindow>
#include <QMessageBox>
#include <QPushButton>
#include <QLabel>
#include <QTimer>
#include <cmath>

class MainWindow :public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget * parent = 0);
	int Min(int a, int b, int c, int d);
	int Min(int a, int b, int c);
	~MainWindow();
protected:
	void keyPressEvent(QKeyEvent *event);
	void paintEvent(QPaintEvent *event);
public slots:
	void timeout();
	void ShowAbout();
	void ShowHelp();
	void StartGame();
	void PauseGame();
	void SpeedUp();
	void SpeedDown();
	void AutoSnake();
private:
	void dealMenu();
private:
	QSnake snake;
	int level;
	int direct;
	int times;
	int MenuBarSize;
	bool IsDie;
	bool bRun;
	bool IsAuto;
	QTimer *timer;
	QLabel *levelLab;
	QPushButton *startGame;
	QPushButton *quitGame;
	QPushButton *speedUp;
	QPushButton *speedDown;
	QPushButton *conti;
	QPushButton *pause;
};

#endif // !MAINWINDOW_H
