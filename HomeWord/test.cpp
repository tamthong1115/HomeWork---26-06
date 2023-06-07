#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iomanip> // for setw
#include <iostream>
#include <random>
#include <set>
#include <string>
#include <unordered_map>
#include <vector>
#include <windows.h>

using namespace std;

void displayIntArray2D(int **array, int rows, int cols, int maxElementWidth);

struct coordinates {
  coordinates(int x1, int y1) {
    x = x1;
    y = y1;
  }
  int y, x;
};

coordinates moveRobot(int **array, int coor_y, int coor_x, int NUMROWS,
                      int NUMCOLS);

void explorePath(int **matrixOriginal, int **matrixForMove, vector<int> &answer,
                 coordinates &robot, int NUMROWS, int NUMCOLS,
                 int maxElementWidth, string mark, string nameRobot,
                 string color);

void gotoxy(int x, int y) {
  static HANDLE h = NULL;
  if (!h)
    h = GetStdHandle(STD_OUTPUT_HANDLE);
  COORD c = {static_cast<SHORT>(x), static_cast<SHORT>(y)};
  SetConsoleCursorPosition(h, c);
}

void writeOuput(vector<int> &answerRobot, ofstream &fileOutput) {
  //* Write the answer to "Output.txt"

  // Take the number of path that the robot have passed
  fileOutput << answerRobot.size() << endl;
  //* Write the element the robot passed
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

void coutTextColor(string text, string color) {
  std::unordered_map<string, string> colorMap = {
      {"black", "\033[30m"},  {"red", "\033[31m"},  {"green", "\033[32m"},
      {"yellow", "\033[33m"}, {"blue", "\033[34m"}, {"magenta", "\033[35m"},
      {"cyan", "\033[36m"},   {"white", "\033[37m"}};
  // Check if the color exists in the map
  if (colorMap.count(color) > 0) {
    std::cout << colorMap[color] << text << colorMap["black"];
  } else {
    std::cout << text; // Color not found, print the text without color
  }
}

void drawPath(coordinates &robot, int maxElementWidth, string mark,
              string color) {
  int x = robot.x + 2 + robot.x * (maxElementWidth + 2);
  int y = ((robot.y + 1) * 2) - 1;
  gotoxy(x, y);
  std::string space = " ";
  std::cout << std::string(maxElementWidth, space[0]);
  x += std::ceil(maxElementWidth / 2);
  gotoxy(x, y);
  coutTextColor(mark, color);
}

size_t generateRandNum() {
  // Create a seed to send value to engine
  std::random_device rand;
  // Take the seed value as an argument
  std::mt19937 generate(rand()); // Mersenne Twister engine

  const int MAX = 1000;

  std::uniform_int_distribution<> dist(1, MAX);
  return dist(generate);
}

void hideCursor() {
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  cursorInfo.dwSize = sizeof(cursorInfo);
  cursorInfo.bVisible = FALSE;
  SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

void showCursor() {
  HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
  CONSOLE_CURSOR_INFO cursorInfo;
  cursorInfo.dwSize = sizeof(cursorInfo);
  cursorInfo.bVisible = TRUE;
  SetConsoleCursorInfo(consoleHandle, &cursorInfo);
}

int main() {
  ifstream fileInput("File/input.txt");
  int NUMROWS;
  int NUMCOLS;
  std::system("cls");
  std::cout << "Num Rows: ";
  std::cin >> NUMROWS;
  std::cout << "Num Cols: ";
  std::cin >> NUMCOLS;

  std::set<size_t> setVector;
  // Dynamically allocate a 2D array
  //! Create an array of 'NUMROWS' elements consisting of int* pointers
  // [int*,int*,int*,int*,int*,int*]
  int **matrixForMove = new int *[NUMROWS];
  int **matrixOriginal = new int *[NUMROWS];
  int maxElementWidth = 0;
  for (int row = 0; row < NUMROWS; row++) {
    // Make a array on each int* element
    matrixForMove[row] = new int[NUMCOLS];
    matrixOriginal[row] = new int[NUMCOLS];
    for (int col = 0; col < NUMCOLS; col++) {
      size_t num;
      // fileInput >> num;

      // Generate unique number
      do {
        num = generateRandNum();
      } while (setVector.count(num) == 1); // Check num is exist
      setVector.insert(num);

      int elementWidth = to_string(num).length();
      if (elementWidth > maxElementWidth) {
        maxElementWidth = elementWidth;
      }
      matrixForMove[row][col] = num;
      matrixOriginal[row][col] = num;
    }
  }

  coordinates robot1(0, 0);
  coordinates robot2(0, 0);

  std::cout << "Enter coordinates for robot 1(" << NUMROWS - 1 << "x"
            << NUMCOLS - 1 << "):";
  std::cin >> robot1.y >> robot1.x;
  std::cout << "Enter coordinates for robot 2(" << NUMROWS - 1 << "x"
            << NUMCOLS - 1 << "):";
  std::cin >> robot2.y >> robot2.x;
  std::system("cls");

  gotoxy(0, 0);
  // Display the int matrix in a table
  displayIntArray2D(matrixForMove, NUMROWS, NUMCOLS, maxElementWidth);

  std::vector<int> answerRobot1;
  std::vector<int> answerRobot2;
  while ((matrixForMove[robot1.y][robot1.x] != -2) ||
         (matrixForMove[robot2.y][robot2.x] != -2)) {
    hideCursor();
    explorePath(matrixOriginal, matrixForMove, answerRobot1, robot1, NUMROWS,
                NUMCOLS, maxElementWidth, "X", "A", "red");
    // Sleep(500);
    explorePath(matrixOriginal, matrixForMove, answerRobot2, robot2, NUMROWS,
                NUMCOLS, maxElementWidth, "Y", "B", "cyan");
  }

  ofstream fileOutput("File/Output.txt");
  fileOutput << "Robot 1" << endl;
  writeOuput(answerRobot1, fileOutput);

  fileOutput << "Robot 2" << endl;
  writeOuput(answerRobot2, fileOutput);

  // cout << endl;
  // displayStringArray2D(matrixString, NUMROWS, NUMCOLS);

  gotoxy(0, ((NUMROWS + 1) * 2) + 1);
  
  std::cout << endl;
  std::system("pause > null");
  std::system("cls");
  showCursor();
  // Delete the dynamically allocated memory
  for (int row = 0; row < NUMROWS; row++) {
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

coordinates moveRobot(int **array, coordinates &robot, int NUMROWS,
                      int NUMCOLS) {

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
                 (robot.y < NUMROWS - 1) ? array[bottom.y][bottom.x] : -1,
                 (robot.x > 0) ? array[left.y][left.x] : -1,
                 (robot.x < NUMCOLS - 1) ? array[right.y][right.x] : -1};

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
    array[top.y][top.x] = -1;
    return top;
  } else if (maxIndex == 1) {
    array[bottom.y][bottom.x] = -1;
    return bottom;
  } else if (maxIndex == 2) {
    array[left.y][left.x] = -1;
    return left;
  } else if (maxIndex == 3) {
    array[right.y][right.x] = -1;
    return right;
  }
  return robot;
}

void explorePath(int **matrixOriginal, int **matrixForMove, vector<int> &answer,
                 coordinates &robot, int NUMROWS, int NUMCOLS,
                 int maxElementWidth, string mark, string nameRobot,
                 string color) {
  if (matrixForMove[robot.y][robot.x] != -2) {
    answer.push_back(matrixOriginal[robot.y][robot.x]);
    drawPath(robot, maxElementWidth, nameRobot, color); // draw "A", "B"
    Sleep(500);
    drawPath(robot, maxElementWidth, mark, color); // draw X
    // Update the path that robot passed
    coordinates next = moveRobot(matrixForMove, robot, NUMROWS, NUMCOLS);
    robot.y = next.y;
    robot.x = next.x;
    drawPath(robot, maxElementWidth, nameRobot, color);
  }
}

void displayIntArray2D(int **array, int rows, int cols, int maxElementWidth) {
  drawLine(cols, maxElementWidth);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      // Display the array with proper alignment
      std::cout << "| " << setw(maxElementWidth) << array[i][j] << " ";
    }
    std::cout << "|" << endl;

    drawLine(cols, maxElementWidth);
  }
}
