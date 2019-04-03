#include <iostream>
#include"Maze.h"


void main()
{
		cout<<"�������Թ�·������������"<<endl;
		int row, col;
		cin >> row >> col;
		Maze maze(row, col);

		cout << "�������Թ��ľ���·����Ϣ(0��ʾ���ߣ�1��ʾ������)��" << endl;
		int arr[5][5];
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				int data;
				cin >> data;
				arr[i][j] = data;
			}
		}
		for (int i = 0; i < row; ++i)
		{
			for (int j = 0; j < col; ++j)
			{
				maze.initMazeNode(arr[i][j], i, j);
			}
		}

		// �����Թ����нڵ�ķ��������״̬
		maze.adjustMaze();
		
		// Ѱ���Թ�·��
		maze.findMazePath();

		//maze.showMaze();

		cout << "Ѱ�ҵ����Թ�·������ ��" << endl;
		
		// ��ӡѰ�ҽṹ
		maze.showResult();
}

