#ifndef GAMEOFLIFE_H
#define GAMEOFLIFE_H

int gol_Generations = 0;
int** gol_Cells = nullptr;

void randomizeCells(int rows, int columns) {
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      int rand = random(0, 3000);
      if ((rand % 2) == 0) {
        gol_Cells[x][y] = 1;
      }
    }
  }
}

void initCells(int** arr, int rows, int columns) {
  for (int x = 0; x < columns; x++) {
    for (int y = 0; y < rows; y++) {
      arr[x * columns + y] = 0;
    }
  }
}

// int countNeighbors(int rows, int columns, int x, int y) {
//   int neighbors = 0;
//   for (int i = -1; i <= 1; i++) {
//     for (int j = -1; j <= 1; j++) {
//       if (x + i < 0 || x + i >= columns || y + j < 0 || y + j >= rows) {
//         continue;
//       } else {
//         neighbors += gol_Cells[(x + i) * columns + (y + j)];
//       }
//     }
//   }
//   return neighbors -= gol_Cells[x * columns + y];
// }

// void applyRulesToCells(int rows, int columns) {
//   int nextGenCells[rows][columns];
//   initCells(*nextGenCells, rows, columns);

//   for (int x = 0; x < columns; x++) {
//     for (int y = 0; y < rows; y++) {
//       int neighbors = countNeighbors(rows, columns, x, y);
//       if (gol_Cells[x * columns + y] == 1 &&
//           (neighbors == 2 || neighbors == 3)) {
//         nextGenCells[x][y] = 1;
//       } else if (gol_Cells[x * columns + y] == 0 && neighbors == 3) {
//         nextGenCells[x][y] = 1;
//       } else {
//         nextGenCells[x][y] = 0;
//       }
//     }
//   }
//   // copy nextGenCells to current cells
//   for (int x = 0; x < columns; x++) {
//     for (int y = 0; y < rows; y++) {
//       gol_Cells[x * columns + y] = nextGenCells[x][y];
//     }
//   }
// }

// void updateGameOfLife(int rows, int columns, int maxGenerations) {
//   applyRulesToCells(rows, columns);
//   if (gol_Generations++ >= maxGenerations) {
//     gol_Generations = 0;
//     initCells(gol_Cells, rows, columns);
//     randomizeCells(rows, columns);
//   }
//   delay(100);
// }

void startGameOfLife(int rows, int columns) {
  gol_Cells = new int*[rows];
  for (int i = 0; i < rows; ++i) gol_Cells[i] = new int[columns];

  initCells(gol_Cells, rows, columns);
  randomSeed(analogRead(0));
  randomizeCells(rows, columns);
}
#endif