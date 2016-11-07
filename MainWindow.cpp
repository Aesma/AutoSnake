#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
	:QMainWindow(parent)
{
	setWindowIcon(QIcon("Food.jpg"));
	setWindowTitle("Snake");
	startGame = new QPushButton(this);
	quitGame = new QPushButton(this);
	startGame->setIcon(QIcon("start.png"));
	startGame->setIconSize(QSize(75, 75));
	startGame->setGeometry(QRect(500, 450, 75, 75));
	startGame->setFlat(true);
	speedUp = new QPushButton(tr("SpeedUp"), this);
	speedDown = new QPushButton(tr("SpeedDown"), this);
	conti = new QPushButton(tr("Continue"), this);
	pause = new QPushButton(tr("Pause"), this);
	speedUp->setGeometry(560, 60, 90, 30);
	speedDown->setGeometry(560, 140, 90, 30);
	conti->setGeometry(560, 400, 90, 30);
	pause->setGeometry(560, 450, 90, 30);
	speedUp->hide();
	speedUp->setFocusPolicy(Qt::NoFocus);
	speedUp->setStyleSheet("QPushButton{background-color:#EEE9E9;border:2px groove gray;border-radius:10px;padding:2px 4px;}""QPushButton:hover{background-color:white; color: black;}""QPushButton:pressed{background-color:#CDCDC1;border - style: inset; }");
	speedDown->hide();
	speedDown->setFocusPolicy(Qt::NoFocus);
	speedDown->setStyleSheet("QPushButton{background-color:#EEE9E9;border:2px groove gray;border-radius:10px;padding:2px 4px;}""QPushButton:hover{background-color:white; color: black;}""QPushButton:pressed{background-color:#CDCDC1;border - style: inset; }");
	conti->hide();
	conti->setFocusPolicy(Qt::NoFocus);
	conti->setStyleSheet("QPushButton{background-color:#EEE9E9;border:2px groove gray;border-radius:10px;padding:2px 4px;}""QPushButton:hover{background-color:white; color: black;}""QPushButton:pressed{background-color:#CDCDC1;border - style: inset; }");
	pause->hide();
	pause->setFocusPolicy(Qt::NoFocus);
	pause->setStyleSheet("QPushButton{background-color:#EEE9E9;border:2px groove gray;border-radius:10px;padding:2px 4px;}""QPushButton:hover{background-color:white; color: black;}""QPushButton:pressed{background-color:#CDCDC1;border - style: inset; }");
	connect(startGame, SIGNAL(clicked()), this, SLOT(StartGame()));
	connect(quitGame, SIGNAL(clicked()), this, SLOT(close()));
	connect(speedUp, SIGNAL(clicked()), this, SLOT(SpeedUp()));
	connect(speedDown, SIGNAL(clicked()), this, SLOT(SpeedDown()));
	connect(conti, SIGNAL(clicked()), this, SLOT(StartGame()));
	connect(pause, SIGNAL(clicked()), this, SLOT(PauseGame()));
	quitGame->setIcon(QIcon("quit.png"));
	quitGame->setIconSize(QSize(75, 75));
	quitGame->setGeometry(QRect(600, 450, 75, 75));
	quitGame->setFlat(true);
	levelLab = new QLabel(this);
	levelLab->setGeometry(560, 100, 90, 30);
	levelLab->hide();
	MenuBarSize = 20;
	direct = 0;
	setFixedSize(700, 510 + MenuBarSize);
	dealMenu();
	IsDie = false;
	bRun = false;
	IsAuto = false;
	timer = new QTimer(this);
	connect(timer, SIGNAL(timeout()), this, SLOT(timeout()));
	times = 200;
	timer->start(times);
}

void MainWindow::timeout()
{
	if (bRun == false){
		timer->stop();
		return;
	}
	snake.Move();
	IsDie = snake.Die();
	if (IsDie){
		timer->stop();
		bRun = false;
		speedUp->hide();
		speedDown->hide();
		conti->hide();
		pause->hide();
		startGame->show();
		quitGame->show();
		levelLab->hide();
		QMessageBox::information(this, tr("GAME OVER"),
			tr("YOU LOSE!"));
		IsDie = false;
		IsAuto = false;
		times = 200;
		startGame->setEnabled(true);
		quitGame->setEnabled(true);
		snake.Clear();
		return;
	}
	update();
}

void MainWindow::paintEvent(QPaintEvent *event)
{
	QPainter painter(this);
	painter.drawPixmap(0, 0, width(), height(), QPixmap("Snake.jpg"));
	if (IsDie || !bRun){
		return;
	}
	startGame->hide();
	quitGame->hide();
	startGame->setEnabled(false);
	quitGame->setEnabled(false);
	painter.drawPixmap(0, 0, 500, 540, QPixmap("Grass.jpg"));
	speedUp->show();
	speedDown->show();
	conti->show();
	pause->show();
	if (times >= 200){
		level = (550 - times) / 50;
	}
	else if (times < 200){
		level = (200 - times) / 20 + 7;
	}
	QString sLevel = QString::number(level, 10);
	levelLab->show();
	levelLab->setText("   Speed : " + sLevel);
	painter.drawPixmap(510, 10 + MenuBarSize, 180, 490, QPixmap("StartRight.jpg"));
	painter.drawPixmap(560, 240, 90, 100, QPixmap("icon.png"));
	QVector<Node>node;
	Node food;
	snake.OutputSnake(node,food);
	painter.setBrush(Qt::red);
	painter.drawEllipse(10 * food.x, 10 * food.y + MenuBarSize, 10, 10);
	painter.setBrush(Qt::white);
	painter.drawRect(0, 0 + MenuBarSize, 700, 10);  
	painter.drawRect(0, 500 + MenuBarSize, 700, 10); 
	painter.drawRect(0, 0 + MenuBarSize, 10, 510); 
	painter.drawRect(500, 0 + MenuBarSize, 10, 510); 
	painter.drawRect(690, 0 + MenuBarSize, 10, 700);
	painter.setBrush(Qt::yellow);
	painter.drawRect(10 * node[0].x, 10 * node[0].y + MenuBarSize, 10, 10);
	painter.setBrush(Qt::blue);
	int n = node.size();
	for (int i = 1; i < n; i++)
	{
		painter.drawRect(10 * node[i].x, 10 * node[i].y + MenuBarSize, 9, 9);
	}
	node.clear();
	if (IsAuto){
		snake.Auto();
	}
	QWidget::paintEvent(event);
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
	switch (event->key())
	{
	case Qt::Key_Up:
		direct = 1;
		break;
	case Qt::Key_Right:
		direct = 2;
		break;
	case Qt::Key_Down:
		direct = 3;
		break;
	case Qt::Key_Left:
		direct = 4;
		break;
	default:
		break;
	}
	snake.ChangeDirector(direct);
	QWidget::keyPressEvent(event);
}

void MainWindow::dealMenu()
{
	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *gameMenu = new QMenu(tr("Game"), menuBar);
	QMenu *helpMenu = new QMenu(tr("Help"), menuBar);
	QAction *autoSnake = new QAction(tr("Auto"), gameMenu);
	gameMenu->addAction(autoSnake);
	menuBar->addMenu(gameMenu);
	connect(autoSnake, SIGNAL(triggered()), this, SLOT(AutoSnake()));
	QAction *helpGame = new QAction(tr("Help"), helpMenu);
	QAction *aboutGame = new QAction(tr("About"), helpMenu);
	helpMenu->addAction(helpGame);
	helpMenu->addAction(aboutGame);
	menuBar->addMenu(helpMenu);
	connect(helpGame, SIGNAL(triggered()), this, SLOT(ShowHelp()));
	connect(aboutGame, SIGNAL(triggered()), this, SLOT(ShowAbout()));
	menuBar->addMenu(helpMenu);
	setMenuBar(menuBar);
}

void MainWindow::ShowAbout()
{
	QMessageBox::information(this, tr("About snake game"),
		tr("By Belphegor   Q676836533	Qt 5.60."));
}

void MainWindow::ShowHelp()
{
	QMessageBox::information(this, tr("Help"),
		tr("Just Start and Control. \n If Auto,Start first then click Auto."));
}

void MainWindow::StartGame()
{
	bRun = TRUE;
	timer->start(times);
}

void MainWindow::PauseGame()
{
	bRun = false;
	IsAuto = false;
	timer->stop();
}

void MainWindow::SpeedUp()
{
	times -= 20;
	if (times <= 20){
		times = 20;
	}
	timer->stop();
	timer->start(times);
}

void MainWindow::SpeedDown()
{
	times += 20;
	if (times >= 500){
		times = 500;
	}
	timer->stop();
	timer->start(times);
}

void MainWindow::AutoSnake()
{
	direct = 1;
	IsAuto = true;
}

int MainWindow::Min(int a, int b, int c, int d)
{
	return ((a<b ? a : b)<c ? (a<b ? a : b) : c)<d ? ((a<b ? a : b)<c ? (a<b ? a : b) : c) : d;
}

int MainWindow::Min(int a, int b, int c)
{
	return (a < b ? a : b) < c ? (a < b ? a : b) : c;
}

MainWindow::~MainWindow()
{

}
