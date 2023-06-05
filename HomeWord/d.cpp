#include <QApplication>
#include <QTableWidget>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Example 2D array
    int array2D[3][4] = {
        {1, 2, 3, 4},
        {5, 6, 7, 8},
        {9, 10, 11, 12}
    };

    // Define the size of the array
    const int rows = 3;
    const int columns = 4;

    // Create a QTableWidget instance
    QTableWidget table(rows, columns);

    // Set table headers
    QStringList headers;
    headers << "Column 1" << "Column 2" << "Column 3" << "Column 4";
    table.setHorizontalHeaderLabels(headers);

    // Populate the table with array values
    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < columns; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(QString::number(array2D[row][col]));
            table.setItem(row, col, item);
        }
    }

    // Show the table
    table.show();

    return app.exec();
}
