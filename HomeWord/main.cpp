#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip> // for setw
#include <iostream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

void displayIntArray2D(int **array, int rows, int cols);

struct coordinates {
  coordinates(int x1, int y1) {
    x = x1;
    y = y1;
  }
  int y, x;
};

coordinates moveRobot(int **array, int coor_y, int coor_x, int numRows,
                      int numCols);

void explorePath(int **matrixOriginal, int **matrixForMove, vector<int> &answer,
                 coordinates &robot, int numRows, int numCols,
                 int maxElementWidth, string mark, string nameRobot);

void gotoxy(int x, int y) {
  static HANDLE h = NULL;
  if (!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
  SetConsoleCursorPosition(h, c);
}

void drawPath(coordinates &robot, int maxElementWidth, string mark) {
  int x = robot.x + 2 + robot.x * (maxElementWidth + 2);
  int y = ((robot.y + 1) * 2) - 1;
  gotoxy(x, y);
  std::string space = " ";
  cout << std::string(maxElementWidth, space[0]);
  x += std::ceil(maxElementWidth / 2);
  gotoxy(x, y);
  cout << mark;
}

void writeOuput(vector<int> &answerRobot, ofstream &fileOutput) {
  //* Write the answer to "Output.txt"

  // Take the number of path that the robot have passed
  fileOutput << answerRobot.size() << endl;
  //* Write the element the robot passed
  for (int i = 0; i < answerRobot.size(); i++) {
    fileOutput << answerRobot[i] << " ";
  }
  fileOutput << endl;
}

int findMaxElementWidth(int **array, int rows, int cols) {
  int maxElementWidth = 0;

  // Find the maximum element width
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int elementWidth = to_string(array[i][j]).length();
      if (elementWidth > maxElementWidth) {
        maxElementWidth = elementWidth;
      }
    }
  }
  return maxElementWidth;
}

int main() {
  ifstream fileInput("File/input.txt");
  int numRows;
  int numCols;
  fileInput >> numRows >> numCols;

  // Dynamically allocate a 2D array
  //! Create an array of 'numRows' elements consisting of int* pointers
  // [int*,int*,int*,int*,int*,int*]
  int **matrixForMove = new int *[numRows];
  int **matrixOriginal = new int *[numRows];
  for (int row = 0; row < numRows; row++) {
    // Make a array on each int* element
    matrixForMove[row] = new int[numCols];
    matrixOriginal[row] = new int[numCols];
    for (int col = 0; col < numCols; col++) {
      int num;
      fileInput >> num;
      matrixForMove[row][col] = num;
      matrixOriginal[row][col] = num;
    }
  }

  coordinates robot1(0, 0);
  coordinates robot2(0, 0);

  std::system("cls");
  cout << "Enter coordinates for robot 1(" << numRows - 1 << "x" << numCols - 1
       << "):";
  cin >> robot1.y >> robot1.x;
  cout << "Enter coordinates for robot 2(" << numRows - 1 << "x" << numCols - 1
       << "):";
  cin >> robot2.y >> robot2.x;
  std::system("cls");

  gotoxy(0, 0);
  // Display the int matrix in a table
  displayIntArray2D(matrixForMove, numRows, numCols);

  int maxElementWidth = findMaxElementWidth(matrixOriginal, numRows, numCols);

  vector<int> answerRobot1;
  vector<int> answerRobot2;
  while ((matrixForMove[robot1.y][robot1.x] != -2) ||
         (matrixForMove[robot2.y][robot2.x] != -2)) {

    explorePath(matrixOriginal, matrixForMove, answerRobot1,
                robot1, numRows, numCols, maxElementWidth, "X", "A");
    // Sleep(500);
    explorePath(matrixOriginal, matrixForMove, answerRobot2,
                robot2, numRows, numCols, maxElementWidth, "X2", "B");
  }

  ofstream fileOutput("File/Output.txt");
  fileOutput << "Robot 1" << endl;
  writeOuput(answerRobot1, fileOutput);

  fileOutput << "Robot 2" << endl;
  writeOuput(answerRobot2, fileOutput);

  // cout << endl;
  // displayStringArray2D(matrixString, numRows, numCols);

  gotoxy(0, (numRows + 1) * 2);
  // Delete the dynamically allocated memory
  for (int row = 0; row < numRows; row++) {
    delete[] matrixForMove[row];
    delete[] matrixOriginal[row];
  }
  delete[] matrixForMove;
  delete[] matrixOriginal;

  return 0;
}

void drawLine(int cols, int maxElementWidth) {
  // Draw +----+-----+----+
  cout << "+";
  for (int j = 0; j < cols; j++) {
    for (int l = 0; l < maxElementWidth + 2; l++) {
      cout << "-";
    }
    cout << "+";
  }
  cout << endl;
};

coordinates moveRobot(int **array, coordinates &robot, int numRows,
                      int numCols) {

  // int numRow = sizeof(array) / sizeof(int);
  // int numCol = sizeof(array[0]) / sizeof(int);
  // Can't do this. B/c the dynamic the value N is not stored anywhere.
  // So sizeof(array) prefer to sizeof(int)
  array[robot.y][robot.x] = -1; //! in arr 2d y first x second arr[y][x]
  coordinates top(robot.x, robot.y - 1);
  coordinates bottom(robot.x, robot.y + 1);
  coordinates left(robot.x - 1, robot.y);
  coordinates right(robot.x + 1, robot.y);

  int ways[4] = {(robot.y > 0) ? array[top.y][top.x] : -1,
                 (robot.y < numRows - 1) ? array[bottom.y][bottom.x] : -1,
                 (robot.x > 0) ? array[left.y][left.x] : -1,
                 (robot.x < numCols - 1) ? array[right.y][right.x] : -1};

  int maxValue = ways[0];
  int maxIndex = 0;
  for (int i = 1; i < 4; i++) {
    if (ways[i] > maxValue) {
      maxValue = ways[i];
      maxIndex = i;
    }
  }
  //! Get out the loop
  if (maxValue == -1) {
    array[robot.y][robot.x] = -2;
    return coordinates(robot.x, robot.y);
  }
  if (maxIndex == 0) {
    return top;
  } else if (maxIndex == 1) {
    return bottom;
  } else if (maxIndex == 2) {
    return left;
  } else if (maxIndex == 3) {
    return right;
  }
  return robot;
}

void explorePath(int **matrixOriginal, int **matrixForMove, vector<int> &answer,
                 coordinates &robot, int numRows, int numCols,
                 int maxElementWidth, string mark, string nameRobot) {
  if (matrixForMove[robot.y][robot.x] != -2) {
    answer.push_back(matrixOriginal[robot.y][robot.x]);
    drawPath(robot, maxElementWidth, nameRobot); // draw "A", "B"
    Sleep(500);
    drawPath(robot, maxElementWidth, mark); // draw X
    // Update the path that robot passed
    coordinates next = moveRobot(matrixForMove, robot, numRows, numCols);
    robot.y = next.y;
    robot.x = next.x;
    drawPath(robot, 3, nameRobot);
  }
}

void displayIntArray2D(int **array, int rows, int cols) {

  int maxElementWidth = findMaxElementWidth(array, rows, cols);

  // Display the array with proper alignment
  drawLine(cols, maxElementWidth);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      cout << "| " << setw(maxElementWidth) << array[i][j] << " ";
    }
    cout << "|" << endl;

    drawLine(cols, maxElementWidth);
  }
}
