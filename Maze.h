#ifndef  _MAZE_H
#define _MAZE_H

#include <iostream>
using namespace std;

const int RIGHT = 0;
const int DOWN = 1;
const int LEFT = 2;
const int UP = 3;
const int WAY_NUM = 4;


const int YES = 5;
const int NO = 6;

class  MazeNode
{
public:
	MazeNode()
	{
		for (int i = 0; i < WAY_NUM; ++i)
		{
			mstate[i] = YES;//�������Թ��ڵ��״̬����ʼ��Ϊ��ͨ��
		}
	}
	~MazeNode()
	{

	}
friend class Maze;
friend class CStack;
private:
	int mval;
	int mx;//�ڵ������
	int my;
	int mstate[WAY_NUM];//��¼�ýڵ��״̬
};

class CStack
{
public:
	CStack()
	{
		mtop = -1;
		mpstack = new MazeNode[msize];
	}
	~CStack()
	{
		delete[]mpstack;
		mpstack = nullptr;
	}

	void push(MazeNode &m);
	void pop();

	

	bool Is_full()
	{
		return mtop == msize - 1 ;
	}
	bool Is_empty()
	{
		return mtop == -1;
	}
	friend class Maze;
private:
	MazeNode *mpstack;
	int mtop;
	int msize;//ջ��������������ջ����Ҫ��ջ��������

	void resize()
	{
		MazeNode *ptmp = new MazeNode[2 * msize];
		for (int i = 0; i < msize; ++i)
		{
			ptmp[i] = mpstack[i];
		}
		delete[]mpstack;
		mpstack = ptmp;
		msize = msize * 2;
	}
};

void CStack::push(MazeNode &m)
{
	if (Is_full())
	{
		throw "stack is full";
	}
	mtop++;
	mpstack[mtop] = m;//�᲻����ø�ֵ��������غ�����
}

void CStack::pop()
{
	if (Is_empty())
	{
		throw "stack is empty";
	}
	mtop--;
}


class Maze
{ 

public:
	Maze(int row, int col):mrow(row),mcol(col)
	{
		mpMaze = new MazeNode* [row];
		for (int i = 0; i < row; ++i)
		{
			mpMaze[i] = new MazeNode[col];
		}
		mstack.msize = row * col ;
	}

	~Maze()
	{
		if (mpMaze)
		{
			for (int i = 0; i < mrow; ++i)
			{
				delete mpMaze[i];
				mpMaze[i] = nullptr;
			}
			delete mpMaze;
		}
	}

	//��ʼ���Թ��ڵ�
void initMazeNode(int val,int x,int y);//void setMazeMap(MazeNode *mmap,int row,int col);

//�����鿴�Ƿ���Ч·���ڵ���ջ
void show_stack()
{
	while(!(mstack.Is_empty()))
	{
		cout << "(" << mstack.mpstack[mstack.mtop--].mx << "," << mstack.mpstack[mstack.mtop--].my << ")" << endl;
	}
}

void showMaze()
{
	for (int i = 0; i < mrow; ++i)
	{
		for (int j = 0; j < mcol; ++j)
		{
			printf("%d ", mpMaze[i][j].mval);
		}
		printf("\n");
	}
}
   
	// �����Թ����нڵ�ķ��������״̬
	void adjustMaze();
	// Ѱ���Թ�·��
	void findMazePath();
	// ��ӡѰ�ҽṹ
	void showResult();

friend class CStack;
//CStack mstack;
private:
	int mrow;
	int mcol;
	MazeNode **mpMaze;
	CStack mstack;

	Maze(const Maze &);

	void operator=(const Maze &src)
	{

	}
};

/*void Maze::setMazeMap(MazeNode *mmap, int row, int col)
{
	mpMaze = new MazeNode* [row];
	for (int i=0; i<row; ++i)
	{
		mpMaze[i] = new MazeNode[col];
	}
	for (int i = 0; i < row; ++i)
	{
		for (int j = 0; j < col; ++j)
		{
			mpMaze[i][j] = *mmap;
			mmap++;
		}
	}
}*/

void Maze::initMazeNode(int val, int x, int y)
{
	mpMaze[x][y].mval = val;
	mpMaze[x][y].mx = x;
	mpMaze[x][y].my = y;
}
/*
0 0 1 0 0
0 0 0 0 0
1 0 0 1 1
0 1 0 1 0
1 0 0 0 0
��ʼѰ���Թ�·��...

û�ҵ�����ӡ��ʾ��Ϣ���Թ������ڿ�ͨ�е�·����
�ҵ��ˣ����Թ�·����ӡһ��
* * 1 0 0
0 * * 1 0
1 0 * 0 1
0 1 * 1 0
1 0 * * *
*/
void Maze::adjustMaze()
{
	for (int j = 0; j < mcol; ++j)
	{
		mpMaze[0][j].mstate[UP] = NO;
		mpMaze[mrow - 1][j].mstate[DOWN] = NO;
	}
	for (int i = 0; i < mrow; ++i)
	{
		mpMaze[i][0].mstate[LEFT] = NO;
		mpMaze[i][mcol - 1].mstate[RIGHT] = NO;
	}

	for (int i = 0; i < mrow; ++i)
	{    
		for (int j = 0; j < mcol; ++j)
		{
			if (1 == mpMaze[i][j].mval)
			{
				if (j > 0) mpMaze[i][j - 1].mstate[RIGHT] = NO;
				if (j < mcol - 1)  mpMaze[i][j + 1].mstate[LEFT] = NO;

				if (i < mrow - 1) mpMaze[i + 1][j].mstate[UP] = NO;
				if (i > 0) mpMaze[i - 1][j].mstate[DOWN] = NO;
			}
			else
			{
				continue;
			}
		}
		
	}
}

void Maze::findMazePath()
{
		int i = 0;
		int j = 0;
		MazeNode tmp;
	    this->mstack.push(mpMaze[0][0]);

		while(i < mrow &&  j < mcol )
		{
			if (i == mrow - 1 && j == mcol - 1)
			{
				break;
			}
			if (mpMaze[i][j].mstate[RIGHT] == YES)
			{
				mpMaze[i][j].mstate[RIGHT] = NO;
				j++;
				this->mstack.push(mpMaze[i][j]);
				mpMaze[i][j].mstate[LEFT] = NO;
				continue;
			}
			else if (mpMaze[i][j].mstate[DOWN] == YES)
			{
				mpMaze[i][j].mstate[DOWN] = NO;
				
				i++;
				this->mstack.push(mpMaze[i][j]);

				mpMaze[i][j].mstate[UP] = YES;
				
				continue;
			}
			else if (mpMaze[i][j].mstate[LEFT] == YES)
			{
				mpMaze[i][j].mstate[LEFT] = NO;
				
				j--;
				this->mstack.push(mpMaze[i][j]);
				mpMaze[i][j].mstate[RIGHT] = NO;
				
				continue;
			}
			else if (mpMaze[i][j].mstate[UP] == YES)
			{
				mpMaze[i][j].mstate[UP] = NO;

				i--;
				this->mstack.push(mpMaze[i][j]);
				mpMaze[i][j].mstate[DOWN] = NO;
				
				continue;
			}
			else
			{
				this->mstack.pop( );
				//tmp = mstack.mpstack[mstack.mtop];
				i = mstack.mpstack[mstack.mtop].mx;
				j = mstack.mpstack[mstack.mtop].my;

				continue;
			}
		}
}

void Maze::showResult()
{
	int i = 0;
	int j = 0;
	while (mstack.mtop != -1)
	{
		i = mstack.mpstack[mstack.mtop].mx;
		j = mstack.mpstack[mstack.mtop].my;
		mstack.mtop--;
		mpMaze[i][j].mval = '*';
	}
	for (int i = 0; i < mrow; ++i)
	{ 
		for (int j = 0; j < mcol; ++j)
		{
			if (mpMaze[i][j].mval == '*')
				cout << "* ";
			else
			{
				cout << mpMaze[i][j].mval << " ";
			}
		}
		cout << endl;
	}
}



#endif  
