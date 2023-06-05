#include <fstream>
#include <iomanip> // for setw
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void displayIntArray2D(int **array, int rows, int cols);
void displayStringArray2D(std::string **array, int rows, int cols);

struct coordinates {
  coordinates(int x1, int y1) {
    x = x1;
    y = y1;
  }
  int y, x;
};

coordinates moveRobot(int **array, int coor_y, int coor_x, int numRows,
                      int numCols);


void explorePath(int **matrixOriginal, int **matrixForMove,
                 string **matrixString, vector<int> &answer, coordinates &robot,
                 int numRows, int numCols, string mark);                      
int main() {
  ifstream fileInput("input.txt");
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
  // Display the int matrix in a table
  displayIntArray2D(matrixForMove, numRows, numCols);

  coordinates robot1(0, 0);
  coordinates robot2(0, 0);
  cout << "Enter coordinates for robot 1(" << numRows - 1 << "x" << numCols - 1
       << "):";
  cin >> robot1.y >> robot1.x;
  cout << "Enter coordinates for robot 2(" << numRows - 1 << "x" << numCols - 1
       << "):";
  cin >> robot2.y >> robot2.x;

  //! Copy matrix to a string array
  string **matrixString = new string *[numRows];
  for (int i = 0; i < numRows; i++) {
    matrixString[i] = new string[numCols];
    for (int j = 0; j < numCols; j++) {
      matrixString[i][j] = to_string(matrixForMove[i][j]);
    }
  }

  vector<int> answerRobot1;
  matrixString[robot1.y][robot1.x] = 'S'; //! Mark The Start
  explorePath(matrixOriginal, matrixForMove, matrixString, answerRobot1, robot1,
              numRows, numCols, "X");
  matrixString[robot1.y][robot1.x] = "E"; //!  Mark The End

  vector<int> answerRobot2;
  matrixString[robot2.y][robot2.x] = "S2";
 explorePath(matrixOriginal, matrixForMove, matrixString, answerRobot2, robot2,
              numRows, numCols, "X2");
  matrixString[robot2.y][robot2.x] = "E2"; //!  Mark The End

  //* Write the answer to "Output.txt"
  ofstream fileOutput("Output.txt");
  fileOutput << "Robot 1" << endl;
  // Take the number of path that the robot have passed
  fileOutput << answerRobot1.size() << endl;
  //* Write the element the robot passed
  for (int i = 0; i < answerRobot1.size(); i++) {
    fileOutput << answerRobot1[i] << " ";
  }

  fileOutput << endl;

  fileOutput << "Robot 2" << endl;
  fileOutput << answerRobot2.size() << endl;
  for (int i = 0; i < answerRobot2.size(); i++) {
    fileOutput << answerRobot2[i] << " ";
  }

  // // Check path matches
  // ofstream matchesFile("matchesPath.txt");
  // vector<int> matchesPath;
  // int arr[numRows * numCols] = {0};
  // int largerPath;
  // if(answerRobot1.size() > answerRobot2.size()){
  //   largerPath = answerRobot1.size();
  // }else{
  //   largerPath = answerRobot2.size();
  // }
  // for(int i =0; i < largerPath; i++ ){
  //   arr[answerRobot1[i]]++;
  //   arr[answerRobot2[i]]++;
  // }
  // for(int i =0; i < largerPath; i++){
  //   if(arr[i] == 2) matchesPath.push_back(arr[i]);
  // }

  // for(int i =0; i < matchesPath.size();i++){
  //   matchesFile << matchesPath[i] << " ";
  // }

  cout << endl;
  displayStringArray2D(matrixString, numRows, numCols);

  // Delete the dynamically allocated memory
  for (int row = 0; row < numRows; row++) {
    delete[] matrixForMove[row];
  }
  delete[] matrixForMove;

  for (int row = 0; row < numRows; row++) {
    delete[] matrixOriginal[row];
  }
  delete[] matrixOriginal;

  for (int row = 0; row < numRows; row++) {
    delete[] matrixString[row];
  }
  delete[] matrixString;

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

coordinates moveRobot(int **array, int coor_y, int coor_x, int numRows,
                      int numCols) {

  // int numRow = sizeof(array) / sizeof(int);
  // int numCol = sizeof(array[0]) / sizeof(int);
  // Can't do this. B/c the dynamic the value N is not stored anywhere.
  // So sizeof(array) prefer to sizeof(int)
  array[coor_y][coor_x] = -1; //! in arr 2d y first x second arr[y][x]
  coordinates top(coor_x, coor_y - 1);
  coordinates bottom(coor_x, coor_y + 1);
  coordinates left(coor_x - 1, coor_y);
  coordinates right(coor_x + 1, coor_y);

  int ways[4] = {(coor_y > 0) ? array[top.y][top.x] : -1,
                 (coor_y < numRows - 1) ? array[bottom.y][bottom.x] : -1,
                 (coor_x > 0) ? array[left.y][left.x] : -1,
                 (coor_x < numCols - 1) ? array[right.y][right.x] : -1};

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
    array[coor_y][coor_x] = -2;
    return coordinates(coor_x, coor_y);
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
}

void explorePath(int **matrixOriginal, int **matrixForMove,
                 string **matrixString, vector<int> &answer, coordinates &robot,
                 int numRows, int numCols, string mark) {
  answer.push_back(matrixOriginal[robot.y][robot.x]);
  // Update the path that robot passed
  coordinates next =
      moveRobot(matrixForMove, robot.y, robot.x, numRows, numCols);
  robot.y = next.y;
  robot.x = next.x;
  matrixString[robot.y][robot.x] = mark;
  while (matrixForMove[robot.y][robot.x] != -2) {
    explorePath(matrixOriginal, matrixForMove,
                matrixString, answer,robot,
                 numRows,  numCols,  mark);
  }
}
void displayIntArray2D(int **array, int rows, int cols) {
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

void displayStringArray2D(std::string **array, int rows, int cols) {
  int maxElementWidth = 0;

  // Find the maximum element width
  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      int elementWidth = array[i][j].length();
      if (elementWidth > maxElementWidth) {
        maxElementWidth = elementWidth;
      }
    }
  }
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