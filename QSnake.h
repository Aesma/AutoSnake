#ifndef QSNAKE_H
#define QSNAKE_H

#include <QVector>
#include <iostream>
#include <Windows.h>
#include <queue>
using std::queue;
using namespace std;

struct Node
{
	int x;
	int y;
};

class QSnake
{
public:
	QSnake(int w = 50, int h = 50);
	QSnake(QSnake &sn);
	void ChangeDirector(const int &NewDirector);
	void Move();
	void OutputSnake(QVector<Node> &node, Node &food);
	void Clear();
	bool Die();
	bool CalBfs(Node st, Node en);
	void CalQue(Node en);
	void Auto();
	bool IsSafe();
	void GoSafe();
	~QSnake();
public:
	queue<Node>que;
private:
	QVector<Node>SnakeNode;
	Node Food;
	int director;
	int length;
	int width;
	int heighth;
	bool die;
	bool chess[490][490];
	bool visit[490][490];
	Node **parent;
private:
	void Judge();
	void AddNode(const int &w, const int &h);
	void PutFood();
};

#endif

