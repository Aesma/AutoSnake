#include "QSnake.h"
#include <cstdlib>
#include <ctime>

QSnake::QSnake(int w, int h)
{
	if (w > 0 || h > 0){
		heighth = h;
		width = w;
	}
	else{
		heighth = 50;
		width = 50;
	}
	length = 0;
	AddNode(w / 2, h / 2);
	PutFood();
	die = false;
	director = 0;
}

void QSnake::AddNode(const int &w, const int &h)
{
	Node *newNode = new Node;
	newNode->x = w;
	newNode->y = h;
	SnakeNode.push_back(*newNode);
	length++;
}

void QSnake::Judge()
{
	if (SnakeNode[0].x == 0 || SnakeNode[0].x == width ||
		SnakeNode[0].y == 0 || SnakeNode[0].y == heighth)
	{
		die = true;
		director = 0;
		return;
	}
	for (int i = 1; i < length; i++)
	{
		if (SnakeNode[0].x == SnakeNode[i].x &&
			SnakeNode[0].y == SnakeNode[i].y)
		{
			die = true;
			return;
		}
	}
	die = false;
}

bool QSnake::Die(void)
{
	return die;
}

void QSnake::Move()
{
	int lastX = SnakeNode[length - 1].x;
	int lastY = SnakeNode[length - 1].y;
	for (int i = length - 1; i > 0; i--)
	{
		SnakeNode[i].x = SnakeNode[i - 1].x;
		SnakeNode[i].y = SnakeNode[i - 1].y;
	}
	switch (director)
	{
	case 1:
		SnakeNode[0].y--;
		break;
	case 2:
		SnakeNode[0].x++;
		break;
	case 3:
		SnakeNode[0].y++;
		break;
	case 4:
		SnakeNode[0].x--;
		break;
	default:
		break;
	}
	if (SnakeNode[0].x == Food.x && SnakeNode[0].y == Food.y)
	{
		AddNode(lastX, lastY);
		PutFood();
	}
	Judge();
}

void QSnake::ChangeDirector(const int &NewDirector)
{
	if (director != 0){
		if (director == NewDirector || director - NewDirector == 2 ||
			director - NewDirector == -2){
			return;
		}
	}
	director = NewDirector;
}

void QSnake::PutFood()
{
	int x, y;
	bool empty = false;
	srand(time(NULL));
	do
	{
		empty = true;
		x = rand() % (width - 2) + 1;
		y = rand() % (heighth - 2) + 1;
		for (int i = 0; i < length; i++)
		{
			if (SnakeNode[i].x == x &&
				SnakeNode[i].y == y)
			{
				empty = false;
				break;
			}
		}
	} while (empty == false);
	Food.x = x;
	Food.y = y;
}

void QSnake::OutputSnake(QVector<Node> & node,
	Node & food)
{
	node.resize(SnakeNode.size());
	node = SnakeNode;
	food = Food;
}

void QSnake::Clear()
{
	SnakeNode.clear();
	length = 0;
	AddNode(25, 25);
	director = 1;
}

void QSnake::Auto()
{
	while (!que.empty())
		que.pop();
	parent = new Node*[490];
	for (int i = 0; i < 490; i++)
		parent[i] = new Node[490];
	int n = SnakeNode.size();
	memset(chess, true, sizeof(chess));
	memset(visit, false, sizeof(visit));
	for (int i = 0; i < 490; i++)
	for (int j = 0; j < 490; j++)
	{
		parent[i][j].x = -1;
		parent[i][j].y = -1;
	}
	for (int i = 0; i < n; i++)
	{
		chess[SnakeNode[i].x][SnakeNode[i].y] = false;
		visit[SnakeNode[i].x][SnakeNode[i].y] = true;
	}
	if (IsSafe()){
		CalBfs(SnakeNode[0], Food);
		CalQue(Food);
		que.pop();
		Node node = que.front();
		if (node.x == SnakeNode[0].x&&node.y < SnakeNode[0].y){
			director = 1;
		}
		else if (node.x>SnakeNode[0].x&&node.y == SnakeNode[0].y){
			director = 2;
		}
		else if (node.x == SnakeNode[0].x&&node.y > SnakeNode[0].y){
			director = 3;
		}
		else if (node.x < SnakeNode[0].x&&node.y == SnakeNode[0].y){
			director = 4;
		}
		ChangeDirector(director);
		for (int i = 0; i < 490; i++)
			delete[]parent[i];
		delete[]parent;
	}
	else {
		GoSafe();
	}
}

bool QSnake::CalBfs(Node st, Node en)
{
	int dir[4][2] = { { 0, 1 }, { 0, -1 }, { 1, 0 }, { -1, 0 } };
	queue<Node>temque;
	visit[st.x][st.y] = true;
	temque.push(st);
	Node head, next;
	int quesize;
	while (!temque.empty())
	{
		quesize = temque.size();
		while (quesize--)
		{
			head = temque.front();
			temque.pop();
			if (head.x == en.x&&head.y == en.y){
				return true;
			}
			for (int i = 0; i < 4; i++)
			{
				next.x = head.x + dir[i][0];
				next.y = head.y + dir[i][1];
				if (next.x<1 || (next.x>499) ||
					next.y<1 || (next.y>499) ||
					!chess[next.x][next.y]){
					continue;
				}
				if (!visit[next.x][next.y]){
					visit[next.x][next.y] = 1;
					temque.push(next);
					parent[next.x][next.y].x = head.x;
					parent[next.x][next.y].y = head.y;
				}
			}
		}
	}
	return false;
}

void QSnake::CalQue(Node en)
{
	if (en.x != -1 && en.y != -1){
		CalQue(parent[en.x][en.y]);
		que.push(en);
	}
}

bool QSnake::IsSafe()
{
	if (!CalBfs(SnakeNode[0], Food)){
		for (int i = 0; i < 490; i++)
			delete[]parent[i];
		delete[]parent;
		parent = new Node*[490];
		for (int i = 0; i < 490; i++)
			parent[i] = new Node[490];
		for (int i = 0; i < 490; i++)
		for (int j = 0; j < 490; j++)
		{
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
		int n = SnakeNode.size();
		memset(chess, true, sizeof(chess));
		memset(visit, false, sizeof(visit));
		for (int i = 0; i < n; i++)
		{
			chess[SnakeNode[i].x][SnakeNode[i].y] = false;
			visit[SnakeNode[i].x][SnakeNode[i].y] = true;
		}
		return false;
	}
	for (int i = 0; i < 490; i++)
		delete[]parent[i];
	delete[]parent;
	parent = new Node*[490];
	for (int i = 0; i < 490; i++)
		parent[i] = new Node[490];
	for (int i = 0; i < 490; i++)
	for (int j = 0; j < 490; j++)
	{
		parent[i][j].x = -1;
		parent[i][j].y = -1;
	}
	int n = SnakeNode.size();
	memset(chess, true, sizeof(chess));
	memset(visit, false, sizeof(visit));
	for (int i = 0; i < n; i++)
	{
		chess[SnakeNode[i].x][SnakeNode[i].y] = false;
		visit[SnakeNode[i].x][SnakeNode[i].y] = true;
	}
	if (CalBfs(SnakeNode[0], Food)){
		for (int i = 0; i < 490; i++)
			delete[]parent[i];
		delete[]parent;
		parent = new Node*[490];
		for (int i = 0; i < 490; i++)
			parent[i] = new Node[490];
		for (int i = 0; i < 490; i++)
		for (int j = 0; j < 490; j++)
		{
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
		int n = SnakeNode.size();
		memset(chess, true, sizeof(chess));
		memset(visit, false, sizeof(visit));
		for (int i = 0; i < n; i++)
		{
			chess[SnakeNode[i].x][SnakeNode[i].y] = false;
			visit[SnakeNode[i].x][SnakeNode[i].y] = true;
		}
		if (!CalBfs(SnakeNode[n - 1], Food)){
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
			parent = new Node*[490];
			for (int i = 0; i < 490; i++)
				parent[i] = new Node[490];
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			int n = SnakeNode.size();
			memset(chess, true, sizeof(chess));
			memset(visit, false, sizeof(visit));
			for (int i = 0; i < n; i++)
			{
				chess[SnakeNode[i].x][SnakeNode[i].y] = false;
				visit[SnakeNode[i].x][SnakeNode[i].y] = true;
			}
			Node first;
			first.x = 1;
			first.y = 1;
			if (!CalBfs(Food, first) && (SnakeNode[0].y != 1 || SnakeNode[0].x != 1)){
				for (int i = 0; i < 490; i++)
					delete[]parent[i];
				delete[]parent;
				parent = new Node*[490];
				for (int i = 0; i < 490; i++)
					parent[i] = new Node[490];
				for (int i = 0; i < 490; i++)
				for (int j = 0; j < 490; j++)
				{
					parent[i][j].x = -1;
					parent[i][j].y = -1;
				}
				n = SnakeNode.size();
				memset(chess, true, sizeof(chess));
				memset(visit, false, sizeof(visit));
				for (int i = 0; i < n; i++)
				{
					chess[SnakeNode[i].x][SnakeNode[i].y] = false;
					visit[SnakeNode[i].x][SnakeNode[i].y] = true;
				}
				if (director == 1 && chess[SnakeNode[0].x][SnakeNode[0].y - 1] == false){
					return false;
				}
				else if (director == 2 && chess[SnakeNode[0].x + 1][SnakeNode[0].y] == false){
					return false;
				}
				else if (director == 3 && chess[SnakeNode[0].x][SnakeNode[0].y + 1] == false){
					return false;
				}
				else if (director == 4 && chess[SnakeNode[0].x - 1][SnakeNode[0].y] == false){
					return false;
				}
			}
		}
		for (int i = 0; i < 490; i++)
			delete[]parent[i];
		delete[]parent;
		parent = new Node*[490];
		for (int i = 0; i < 490; i++)
			parent[i] = new Node[490];
		for (int i = 0; i < 490; i++)
		for (int j = 0; j < 490; j++)
		{
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
		n = SnakeNode.size();
		memset(chess, true, sizeof(chess));
		memset(visit, false, sizeof(visit));
		for (int i = 0; i < n; i++)
		{
			chess[SnakeNode[i].x][SnakeNode[i].y] = false;
			visit[SnakeNode[i].x][SnakeNode[i].y] = true;
		}
		return true;
	}
}

void QSnake::GoSafe()
{
	if (!CalBfs(SnakeNode[0], Food)){
		for (int i = 0; i < 490; i++)
		for (int j = 0; j < 490; j++)
		{
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
		for (int i = 0; i < 490; i++)
			delete[]parent[i];
		delete[]parent;
		parent = new Node*[490];
		for (int i = 0; i < 490; i++)
			parent[i] = new Node[490];
		for (int i = 0; i < 490; i++)
		for (int j = 0; j < 490; j++)
		{
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
		int n = SnakeNode.size();
		memset(chess, true, sizeof(chess));
		memset(visit, false, sizeof(visit));
		for (int i = 0; i < n; i++)
		{
			chess[SnakeNode[i].x][SnakeNode[i].y] = false;
			visit[SnakeNode[i].x][SnakeNode[i].y] = true;
		}
		if (CalBfs(SnakeNode[0], SnakeNode[n - 1])){
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
			while (!que.empty())
				que.pop();
			parent = new Node*[490];
			for (int i = 0; i < 490; i++)
				parent[i] = new Node[490];
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			memset(chess, true, sizeof(chess));
			memset(visit, false, sizeof(visit));
			for (int i = 0; i < n; i++)
			{
				chess[SnakeNode[i].x][SnakeNode[i].y] = false;
				visit[SnakeNode[i].x][SnakeNode[i].y] = true;
			}
			CalBfs(SnakeNode[0], SnakeNode[n - 1]);
			CalQue(SnakeNode[n - 1]);
			que.pop();
			Node node = que.front();
			if (node.x == SnakeNode[0].x&&node.y < SnakeNode[0].y){
				director = 1;
			}
			else if (node.x>SnakeNode[0].x&&node.y == SnakeNode[0].y){
				director = 2;
			}
			else if (node.x == SnakeNode[0].x&&node.y > SnakeNode[0].y){
				director = 3;
			}
			else if (node.x < SnakeNode[0].x&&node.y == SnakeNode[0].y){
				director = 4;
			}
			ChangeDirector(director);
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;

			return;
		}
		else{
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
			parent = new Node*[490];
			for (int i = 0; i < 490; i++)
				parent[i] = new Node[490];
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			memset(chess, true, sizeof(chess));
			memset(visit, false, sizeof(visit));
			for (int i = 0; i < n; i++)
			{
				chess[SnakeNode[i].x][SnakeNode[i].y] = false;
				visit[SnakeNode[i].x][SnakeNode[i].y] = true;
			}
			if (director == 1){
				if ((SnakeNode[0].y - 1 == 0 && (!chess[SnakeNode[0].x + 1][SnakeNode[0].y] || SnakeNode[0].x + 1 == 50)) || (!chess[SnakeNode[0].x][SnakeNode[0].y - 1] && (!chess[SnakeNode[0].x + 1][SnakeNode[0].y] || SnakeNode[0].x + 1 == 50))){
					director = 4;
					ChangeDirector(director);
					Sleep(500);
					director = 3;
					ChangeDirector(director);
					return;
				}
				if ((SnakeNode[0].y - 1 == 0 && (!chess[SnakeNode[0].x - 1][SnakeNode[0].y] || SnakeNode[0].x - 1 == 0)) || (!chess[SnakeNode[0].x][SnakeNode[0].y - 1] && (!chess[SnakeNode[0].x - 1][SnakeNode[0].y] || SnakeNode[0].x - 1 == 0))){
					director = 2;
					ChangeDirector(director);
					Sleep(500);
					director = 3;
					ChangeDirector(director);
					return;
				}
			}
			if (director == 2){
				if ((SnakeNode[0].x + 1 == 50 && (!chess[SnakeNode[0].x][SnakeNode[0].y + 1] || SnakeNode[0].y + 1 == 50)) || (!chess[SnakeNode[0].x + 1][SnakeNode[0].y] && (!chess[SnakeNode[0].x][SnakeNode[0].y + 1]) || SnakeNode[0].y + 1 == 50)){
					director = 1;
					ChangeDirector(director);
					Sleep(200);
					director = 4;
					ChangeDirector(director);
					return;
				}
				if ((SnakeNode[0].x + 1 == 50 && (!chess[SnakeNode[0].x][SnakeNode[0].y - 1] || SnakeNode[0].y - 1 == 0)) || (!chess[SnakeNode[0].x + 1][SnakeNode[0].y] && (!chess[SnakeNode[0].x][SnakeNode[0].y - 1]) || SnakeNode[0].y - 1 == 0)){
					director = 3;
					ChangeDirector(director);
					Sleep(200);
					director = 4;
					ChangeDirector(director);
					return;
				}
			}
			if (director == 3){
				if ((SnakeNode[0].y + 1 == 50 && (!chess[SnakeNode[0].x + 1][SnakeNode[0].y] || SnakeNode[0].x + 1 == 50)) || (!chess[SnakeNode[0].x][SnakeNode[0].y + 1] && (!chess[SnakeNode[0].x + 1][SnakeNode[0].y] || SnakeNode[0].x + 1 == 50))){
					director = 4;
					ChangeDirector(director);
					Sleep(200);
					director = 1;
					ChangeDirector(director);
					return;
				}
				if ((SnakeNode[0].y + 1 == 50 && (!chess[SnakeNode[0].x - 1][SnakeNode[0].y] || SnakeNode[0].x - 1 == 0)) || (!chess[SnakeNode[0].x][SnakeNode[0].y + 1] && (!chess[SnakeNode[0].x - 1][SnakeNode[0].y] || SnakeNode[0].x - 1 == 0))){
					director = 2;
					ChangeDirector(director);
					Sleep(200);
					director = 1;
					ChangeDirector(director);
					return;
				}
			}
			if (director == 4){
				if ((SnakeNode[0].x - 1 == 50 && (!chess[SnakeNode[0].x][SnakeNode[0].y + 1] || SnakeNode[0].y + 1 == 50)) || (!chess[SnakeNode[0].x - 1][SnakeNode[0].y] && (!chess[SnakeNode[0].x][SnakeNode[0].y + 1]) || SnakeNode[0].y + 1 == 50)){
					director = 1;
					ChangeDirector(director);
					Sleep(200);
					director = 2;
					ChangeDirector(director);
					return;
				}
				if ((SnakeNode[0].x - 1 == 50 && (!chess[SnakeNode[0].x][SnakeNode[0].y - 1] || SnakeNode[0].y - 1 == 0)) || (!chess[SnakeNode[0].x - 1][SnakeNode[0].y] && (!chess[SnakeNode[0].x][SnakeNode[0].y - 1]) || SnakeNode[0].y - 1 == 0)){
					director = 3;
					ChangeDirector(director);
					Sleep(200);
					director = 2;
					ChangeDirector(director);
					return;
				}
			}
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;

			return;
		}
	}
	else{
		for (int i = 0; i < 490; i++)
		for (int j = 0; j < 490; j++)
		{
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
		for (int i = 0; i < 490; i++)
			delete[]parent[i];
		delete[]parent;
		parent = new Node*[490];
		for (int i = 0; i < 490; i++)
			parent[i] = new Node[490];
		for (int i = 0; i < 490; i++)
		for (int j = 0; j < 490; j++)
		{
			parent[i][j].x = -1;
			parent[i][j].y = -1;
		}
		int n = SnakeNode.size();
		memset(chess, true, sizeof(chess));
		memset(visit, false, sizeof(visit));
		for (int i = 0; i < n; i++)
		{
			chess[SnakeNode[i].x][SnakeNode[i].y] = false;
			visit[SnakeNode[i].x][SnakeNode[i].y] = true;
		}
		if (!CalBfs(SnakeNode[0], SnakeNode[n - 1])){
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
			parent = new Node*[490];
			for (int i = 0; i < 490; i++)
				parent[i] = new Node[490];
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			memset(chess, true, sizeof(chess));
			memset(visit, false, sizeof(visit));
			for (int i = 0; i < n; i++)
			{
				chess[SnakeNode[i].x][SnakeNode[i].y] = false;
				visit[SnakeNode[i].x][SnakeNode[i].y] = true;
			}
			if (director == 1){
				if (Food.x > SnakeNode[0].x&&chess[SnakeNode[0].x][SnakeNode[0].y - 1] == false){
					director = 4;
					ChangeDirector(director);
					return;
				}
				else if (Food.x < SnakeNode[0].x&&chess[SnakeNode[0].x][SnakeNode[0].y - 1] == false){
					director = 2;
					ChangeDirector(director);
					return;
				}
				if (SnakeNode[0].y - 1 == 0 && Food.x>SnakeNode[0].x){
					director = 4;
					ChangeDirector(director);
					Sleep(200);
					director = 3;
					ChangeDirector(director);
					return;
				}
				else if (SnakeNode[0].y - 1 == 0 && Food.x<SnakeNode[0].x){
					director = 2;
					ChangeDirector(director);
					Sleep(200);
					director = 3;
					ChangeDirector(director);
					return;
				}
			}
			if (director == 2){
				if (Food.y > SnakeNode[0].y && chess[SnakeNode[0].x + 1][SnakeNode[0].y] == false){
					director = 1;
					ChangeDirector(director);
					return;
				}
				else if (Food.y < SnakeNode[0].y && chess[SnakeNode[0].x + 1][SnakeNode[0].y] == false){
					director = 3;
					ChangeDirector(director);
					return;
				}
				if (SnakeNode[0].x + 1 == 50 && Food.y > SnakeNode[0].y){
					director = 1;
					ChangeDirector(director);
					Sleep(200);
					director = 4;
					ChangeDirector(director);
					return;
				}
				else if (SnakeNode[0].x + 1 == 50 && Food.y < SnakeNode[0].y){
					director = 3;
					ChangeDirector(director);
					Sleep(200);
					director = 4;
					ChangeDirector(director);
					return;
				}
			}
			if (director == 3){
				if (Food.x>SnakeNode[0].x && chess[SnakeNode[0].x][SnakeNode[0].y + 1] == false){
					director = 4;
					ChangeDirector(director);
					return;
				}
				else if (Food.x<SnakeNode[0].x && chess[SnakeNode[0].x][SnakeNode[0].y + 1] == false){
					director = 2;
					ChangeDirector(director);
					return;
				}
				if (SnakeNode[0].y + 1 == 50 && Food.x>SnakeNode[0].x){
					director = 4;
					ChangeDirector(director);
					Sleep(200);
					director = 1;
					ChangeDirector(director);
					return;
				}
				else if (SnakeNode[0].y + 1 == 50 && Food.x<SnakeNode[0].x){
					director = 2;
					ChangeDirector(director);
					Sleep(200);
					director = 1;
					ChangeDirector(director);
					return;
				}
			}
			if (director == 4){
				if (Food.y > SnakeNode[0].y && chess[SnakeNode[0].x - 1][SnakeNode[0].y] == false){
					director = 1;
					ChangeDirector(director);
					return;
				}
				else if (Food.y < SnakeNode[0].y && chess[SnakeNode[0].x - 1][SnakeNode[0].y] == false){
					director = 3;
					ChangeDirector(director);
					return;
				}
				if (SnakeNode[0].x - 1 == 0 && Food.y > SnakeNode[0].y){
					director = 1;
					ChangeDirector(director);
					Sleep(200);
					director = 4;
					ChangeDirector(director);
					return;
				}
				else if (SnakeNode[0].x - 1 == 0 && Food.y < SnakeNode[0].y){
					director = 3;
					ChangeDirector(director);
					Sleep(200);
					director = 4;
					ChangeDirector(director);
					return;
				}
			}
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
		}
		else{
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
			while (!que.empty())
				que.pop();
			parent = new Node*[490];
			for (int i = 0; i < 490; i++)
				parent[i] = new Node[490];
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			memset(chess, true, sizeof(chess));
			memset(visit, false, sizeof(visit));
			for (int i = 0; i < n; i++)
			{
				chess[SnakeNode[i].x][SnakeNode[i].y] = false;
				visit[SnakeNode[i].x][SnakeNode[i].y] = true;
			}
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
			parent = new Node*[490];
			for (int i = 0; i < 490; i++)
				parent[i] = new Node[490];
			for (int i = 0; i < 490; i++)
			for (int j = 0; j < 490; j++)
			{
				parent[i][j].x = -1;
				parent[i][j].y = -1;
			}
			memset(chess, true, sizeof(chess));
			memset(visit, false, sizeof(visit));
			for (int i = 0; i < n; i++)
			{
				chess[SnakeNode[i].x][SnakeNode[i].y] = false;
				visit[SnakeNode[i].x][SnakeNode[i].y] = true;
			}
			CalBfs(SnakeNode[0], SnakeNode[n - 1]);
			CalQue(SnakeNode[n - 1]);
			que.pop();
			Node node = que.front();
			for (int i = 0; i < 490; i++)
				delete[]parent[i];
			delete[]parent;
			if (node.x == SnakeNode[0].x&&node.y < SnakeNode[0].y){
				director = 1;
			}
			else if (node.x>SnakeNode[0].x&&node.y == SnakeNode[0].y){
				director = 2;
			}
			else if (node.x == SnakeNode[0].x&&node.y > SnakeNode[0].y){
				director = 3;
			}
			else if (node.x < SnakeNode[0].x&&node.y == SnakeNode[0].y){
				director = 4;
			}
			ChangeDirector(director);
		}
	}
}

QSnake::~QSnake()
{
	SnakeNode.clear();
}
