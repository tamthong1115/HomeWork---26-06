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
int main() {
  ifstream fileInput("input.txt");
  int numRows;
  int numCols;
  fileInput >> numRows >> numCols;

  // Dynamically allocate a 2D array
  //! Create an array of 'numRows' elements consisting of int* pointers
  // [int*,int*,int*,int*,int*,int*]
  int **matrix = new int *[numRows];
  for (int row = 0; row < numRows; row++) {
    // Make a array on each int* element
    matrix[row] = new int[numCols];
    for (int col = 0; col < numCols; col++) {
      int num;
      fileInput >> num;
      matrix[row][col] = num;
    }
  }
  // Display the int matrix in a table
  displayIntArray2D(matrix, numRows, numCols);

  coordinates robot1(0, 0);
  coordinates robot2(0, 0);
  cout << "Enter coordinates for robot 1(" << numRows - 1 << "x" << numCols - 1
       << "):";
  cin >> robot1.y >> robot1.x;
  // cout << "Enter coordinates for robot 2(" << numRows << "x" << numCols <<
  // "):"; cin >> robot2.x >> robot2.y;

  //! Copy matrix to a string array
  string **matrixString = new string *[numRows];
  for (int i = 0; i < numRows; i++) {
    matrixString[i] = new string[numCols];
    for (int j = 0; j < numCols; j++) {
      matrixString[i][j] = to_string(matrix[i][j]);
    }
  }

  vector<int> answer;
  int numPathRobot1 = 0;
  matrixString[robot1.y][robot1.x] = 'S'; //! Mark The Start
  while (matrix[robot1.y][robot1.x] != -2) {
    answer.push_back(matrix[robot1.y][robot1.x]);
    // Update the path that robot goes to
    coordinates next = moveRobot(matrix, robot1.y, robot1.x, numRows, numCols);
    robot1.y = next.y;
    robot1.x = next.x;
    matrixString[robot1.y][robot1.x] = 'X';
    numPathRobot1++;
  }
  matrixString[robot1.y][robot1.x] = 'E'; //!  Mark The End

  //* Write the answer to "Output.txt"
  ofstream fileOutput("Output.txt");
  fileOutput << numPathRobot1 << endl;
  //* Write the element the robot passed
  for (int i = 0; i < answer.size(); i++) {
    fileOutput << answer[i] << " ";
  }

  cout << endl;
  displayStringArray2D(matrixString, numRows, numCols);

  // Delete the dynamically allocated memory
  for (int row = 0; row < numRows; row++) {
    delete[] matrix[row];
  }
  delete[] matrix;

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