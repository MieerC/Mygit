#include <iostream>
#include"Maze.h"


void main()
{
		cout<<"请输入迷宫路径的行列数："<<endl;
		int row, col;
		cin >> row >> col;
		Maze maze(row, col);

		cout << "请输入迷宫的具体路径信息(0表示能走，1表示不能走)：" << endl;
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

		// 调整迷宫所有节点的方向的行走状态
		maze.adjustMaze();
		
		// 寻找迷宫路径
		maze.findMazePath();

		//maze.showMaze();

		cout << "寻找到的迷宫路径如下 ：" << endl;
		
		// 打印寻找结构
		maze.showResult();
}

