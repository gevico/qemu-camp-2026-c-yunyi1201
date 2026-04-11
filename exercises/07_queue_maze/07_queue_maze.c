#include <stdio.h>

#define MAX_ROW 5
#define MAX_COL 5

int maze[MAX_ROW][MAX_COL] = {
	0, 1, 0, 0, 0,
	0, 1, 0, 1, 0,
	0, 0, 0, 0, 0,
	0, 1, 1, 1, 0,
	0, 0, 0, 1, 0,
};

typedef struct {
    int row;
    int col;
    int prev;
} QueueNode;

#define QUEUE_MAX (MAX_ROW * MAX_COL)

int main(void)
{
    QueueNode queue[QUEUE_MAX];
    int visited[MAX_ROW][MAX_COL] = {0};
    int dr[] = {-1, 1, 0, 0};
    int dc[] = {0, 0, -1, 1};

    int head = 0, tail = 0;

    queue[tail].row = 0;
    queue[tail].col = 0;
    queue[tail].prev = -1;
    tail++;
    visited[0][0] = 1;

    int found = 0;
    int found_idx = -1;

    while (head < tail) {
        int cur = head++;
        int r = queue[cur].row;
        int c = queue[cur].col;

        if (r == MAX_ROW - 1 && c == MAX_COL - 1) {
            found = 1;
            found_idx = cur;
            break;
        }

        for (int d = 0; d < 4; d++) {
            int nr = r + dr[d];
            int nc = c + dc[d];
            if (nr >= 0 && nr < MAX_ROW && nc >= 0 && nc < MAX_COL
                && !visited[nr][nc] && maze[nr][nc] == 0) {
                visited[nr][nc] = 1;
                queue[tail].row = nr;
                queue[tail].col = nc;
                queue[tail].prev = cur;
                tail++;
            }
        }
    }

    if (!found) {
        printf("No path!\n");
    } else {
        int path[QUEUE_MAX][2];
        int path_len = 0;
        int idx = found_idx;
        while (idx != -1) {
            path[path_len][0] = queue[idx].row;
            path[path_len][1] = queue[idx].col;
            path_len++;
            idx = queue[idx].prev;
        }
        for (int i = 0; i < path_len; i++) {
            printf("(%d, %d)\n", path[i][0], path[i][1]);
        }
    }

	return 0;
}
