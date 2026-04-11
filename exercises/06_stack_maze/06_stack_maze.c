#include <stdbool.h>
#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
    0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1, 0,
};

int visited[MAX_ROW][MAX_COL] = {0};
int path[50][2];  // 存储路径坐标 [行, 列]

int target_row = 4;
int target_col = 4;
int path_len = 0;

bool dfs(int row, int col) {
    if (row >= MAX_ROW || col >= MAX_COL || maze[row][col] == 1 || visited[row][col]) {
        return false;
    }
    visited[row][col] = 1;
    path[path_len][0] = row;
    path[path_len][1] = col;
    path_len++;

    if (row == target_row && col == target_col) {
        return true;
    }

    if (dfs(row + 1, col)) {
        return true;
    }


    // back-track
    path_len--;
    return false;
}

int main(void) {
    // TODO: 在这里添加你的代码
    if (dfs(0, 0)) {
      for (int i = 0; i < path_len; i++) {
          printf("(%d, %d)\n", path[i][0], path[i][1]);
      }
    } else {
      printf("No path!\n");
    }
    return 0;
}