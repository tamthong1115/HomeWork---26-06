#include<iostream>
#include<fstream>
using namespace std;

struct posRobot {
	int curPosX = 0;
	int curPosY = 0;
	int value = 0;
	int amountCell = 1;
};
struct posElement {
	int value = 0;
	int posX = 0;
	int posY = 0;
};

posRobot robot;
posElement infoStep[4] = { 0 };
int sizeMatrix[2] = {}; //[0]=row and [1]=cloumn

#pragma region funcsEvent
bool isElementExist(int i, int j);
posElement nextStep(posElement a[4], int size);
#pragma endregion

#pragma region funcsDeclare
bool isElementExist(int posY, int posX) {
	if (posY == -1 || posY == sizeMatrix[0])
	{
		return false;
	}
	else if (posX == -1 || posX == sizeMatrix[1])
	{
		return false;
	}
	return true;
};
posElement nextStep(posElement a[4], int size) {
	posElement max = { 0,0,robot.value };
	for (int i = 0; i < size; i++)
	{
		if (max.value <= a[i].value)
		{
			max = a[i];
		}
	};
	if (max.value==robot.value)
	{
		max.value = 0;
	}
	return max;
}
int main() {
	ifstream fileInput("input.txt");

	int i = 0, j = 0;

	fileInput.seekg(0, ios::beg);//set con tro ve dau file
	for (int i = 0; i < 2; i++)
	{
		int num = 0;
		fileInput.seekg(i, ios::beg);
		if (fileInput >> num)
		{
			sizeMatrix[i] = num;
		}
	}

	posElement** arr = new posElement* [sizeMatrix[0]]; //ma tran dau vao
	for (int i = 0; i < sizeMatrix[0]; i++)
	{
		arr[i] = new posElement[sizeMatrix[1]];
	};

	fileInput.seekg(0, ios::cur);
	for (int i = 0; i < sizeMatrix[0]; i++)
	{
		for (int j = 0; j < sizeMatrix[1]; j++)
		{
			int num;
			if (fileInput >> num)
			{
				arr[i][j].value = num;
				arr[i][j].posX = j;
				arr[i][j].posY = i;
			}
		}
	}

	int** arrFlag = new int* [sizeMatrix[0]]; //ma tran danh dau
	for (int i = 0; i < sizeMatrix[0]; i++)
	{
		arrFlag[i] = new int[sizeMatrix[1]];
		for (int j = 0; j < sizeMatrix[1]; j++)
		{
			arrFlag[i][j] = 0;
		}
	};
	cout << "Enter you ROBOT's Position: ";
	cin >> robot.curPosY >> robot.curPosX;

	robot.value = arr[robot.curPosX, robot.curPosY]->value;

	ofstream fileOutput("output.txt");
	if (fileOutput.fail())
	{
		cout << "Can not open this file";
	}

	while (true)
	{
		arrFlag[robot.curPosY][robot.curPosX] = 1;
		int step = 0;

		if (isElementExist(robot.curPosY - 1, robot.curPosX)) //len
		{
			if (arrFlag[robot.curPosY - 1][robot.curPosX] != 1)
			{
				infoStep[step] = arr[robot.curPosY - 1][robot.curPosX];
				step++;
			}
		}
		if (isElementExist(robot.curPosY + 1, robot.curPosX)) //xuong
		{
			if (arrFlag[robot.curPosY + 1][robot.curPosX] != 1)
			{
				infoStep[step] = arr[robot.curPosY + 1][robot.curPosX];
				step++;
			};
		};

		if (isElementExist(robot.curPosY, robot.curPosX - 1)) //trai
		{
			if (arrFlag[robot.curPosY][robot.curPosX - 1] != 1)
			{
				infoStep[step] = arr[robot.curPosY][robot.curPosX - 1];
				step++;
			}
		}
		if (isElementExist(robot.curPosY, robot.curPosX + 1)) //phai
		{
			if (arrFlag[robot.curPosY][robot.curPosX + 1] != 1)
			{
				infoStep[step] = arr[robot.curPosY][robot.curPosX + 1];
				step++;
			}
		}

		posElement temp = nextStep(infoStep, step);
		if (temp.value != 0)
		{
			robot.curPosX = temp.posX;
			robot.curPosY = temp.posY;
			robot.value = temp.value;
		}
		else break;

		robot.amountCell++;
		fileOutput << temp.value << ' ';
	};
	fileOutput << endl;
	fileOutput << robot.amountCell;

	return 0;
}