#include <cstdio>
#include <cstdlib>

int main(int argc, char *argv[]) {
	int n = 3, m = 3;
	// int **arr = (int **) malloc(n * sizeof(int *));
	int **arr = new int*[n];

	for (int i = 0; i < n; i++) {
		arr[i] = new int[m];
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			arr[i][j] = i + j;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("arr[%d][%d] = %d\n", i, j, arr[i][j]);
		}
	}
	return 0;
}

