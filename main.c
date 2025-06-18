#include <stdio.h>
#include <stdlib.h>

void print_optimal_parens(int i, int j, unsigned int** s);

int main() {
  int N;
  printf("How many matrices: ");
  if (scanf("%d", &N) != 1 || N <= 0) {
    fprintf(stderr, "Invalid number.\n");
    return 1;
  }

  int p_size = N + 1;
  int p[p_size];
  for (int i = 0; i < p_size; i++) {
    printf("Enter dimension %d: ", i);
    if (scanf("%d", &p[i]) != 1 || p[i] <= 0) {
      fprintf(stderr, "Invalid dimension.\n");
      return 1;
    }
  }

  int show_steps;
  printf("Show steps? (1 for yes, 0 for no): ");
  if (scanf("%d", &show_steps) != 1 || (show_steps != 0 && show_steps != 1)) {
    fprintf(stderr, "Invalid input for showing steps.\n");
    return 1;
  }
  getchar(); // consume the newline character left by scanf

  unsigned int m[N][N];
  unsigned int** s = malloc(N * sizeof(unsigned int*));
  if (!s) {
    perror("malloc");
    return 1;
  }
  for (int i = 0; i < N; i++) {
    s[i] = malloc(N * sizeof(unsigned int));
    if (!s[i]) {
      perror("malloc");
      return 1;
    }
  }
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      m[i][j] = 0;
      s[i][j] = 0;
    }
  }

  // l -> how many matrices are being multiplied
  for (int length_chain = 2; length_chain <= N; length_chain++) {
    // i -> starting index of the chain (which chains are being multiplied)
    for (int i = 0; i < N - length_chain + 1; i++) {
      // j -> ending index of the chain (since l tells how many matrices are being multiplied, than just add from i to get the end index)
      int j = i + length_chain - 1;
      m[i][j] = -1; // set to max value
      // k -> index where the chain is split
      for (int k = i; k < j; k++) {
        // same formula from m[i, j] = m[i, k] + m[k + 1, j] + pi−1.pk.pj, but m start by 0, so does i
        int q = m[i][k] + m[k + 1][j] + p[i] * p[k + 1] * p[j + 1];
        if (q < m[i][j]) {
          m[i][j] = q;
          s[i][j] = k;

          if (show_steps) {
            printf("--------------------\n");
            for (int i = 0; i < N; i++) {
              for (int j = 0; j < N; j++) {
                printf("%u\t", m[i][j]);
              }
              printf("\n");
            }
            printf("Updated m[%d][%d] = %u with split at k = %d\n", i, j, m[i][j], k);
            getchar(); // wait for user input to continue
          }
        }
      }
    }
  }

  printf("Minimum number of multiplications is %u\n", m[0][N - 1]);
  printf("Optimal parenthesization:\n");
  print_optimal_parens(0, N - 1, s);
  printf("\n");

  for (int i = 0; i < N; i++) {
    free(s[i]);
  }
  free(s);

  return 0;
}

void print_optimal_parens(int i, int j, unsigned int** s) {
  if (i == j) {
    printf("A%d", i + 1);
  } else {
    printf("(");
    print_optimal_parens(i, s[i][j], s);
    printf(" x ");
    print_optimal_parens(s[i][j] + 1, j, s);
    printf(")");
  }
}
