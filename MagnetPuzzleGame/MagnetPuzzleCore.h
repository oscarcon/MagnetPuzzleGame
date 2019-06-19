#include <stdio.h>

#define L		0
#define R		2
#define T		1
#define B		3
#define	POS		'+'
#define NEG 	'-'
#define EMPTY	' '

typedef struct {
	int x;
	int y;
	int pole;
} cell;


const int M = 2, N = 4;
struct puzzle {
	int top[N];
	int left[M];
	int bottom[N];
	int right[M];
	int rules[M][N];
} puzzle_default = {
   {1, -1, -1, -1},
   {1, 1},
   {-1, -1, 1, -1},
   {2, -1},
   {
	   {L, R, T, T},
	   {L, R, B, B}
   }
};

//const int M = 5, N = 6;
//struct puzzle {
//	int top[N];
//	int left[M];
//	int bottom[N];
//	int right[M];
//	int rules[M][N];
//} puzzle_default = {
//	{ 1, -1, -1, 2, 1, -1 },
//	{ 2, 3, -1, -1, -1 },
//	{ 2, -1, -1, 2, -1, 3 },
//	{ -1, -1, -1, 1, -1 },
//	{
//		{ L, R, L, R, T, T },
//		{ L, R, L, R, B, B },
//		{ T, T, T, T, L, R },
//		{ B, B, B, B, T, T },
//		{ L, R, L, R, B, B }
//	}
//};

typedef struct puzzle puzzle;

//int magnets[M][N] = {
//	{POS, NEG, POS, NEG, EMPTY, NEG},
//	{NEG, POS, NEG, POS, EMPTY, POS},
//	{EMPTY, EMPTY, POS, NEG, POS, NEG},
//	{EMPTY, EMPTY, NEG, POS, EMPTY, POS},
//	{NEG, POS, EMPTY, EMPTY, EMPTY, NEG}
//};
//int magnets[M][N] = { EMPTY };
// const int M = 2, N = 4;
// int top[] = { 3, -1, -1, -1};
// int left[] = { 1, 1 };
// int bottom[] = { -1, -1, 1, -1};
// int right[] = { 2, -1 };
// int rules[M][N] = { 
// 	{L, R, T, T},
// 	{L, R, B, B}
// };
// int magnets[M][N] = {
// 	{POS, NEG, NEG, EMPTY},
// 	{EMPTY, EMPTY, POS, EMPTY}
// };


cell find_match(const puzzle *puz, int magnets[][N], int x, int y) {
	cell c = { 0, 0, 0 };
	if (x < 0 || x >= M || y < 0 || y >= N) {
		c.x = -1;
		c.y = -1;
		c.pole = -1;
		return c;
	}
	int up_r = x - 1, up_c = y;
	int right_r = x, right_c = y + 1;
	int down_r = x + 1, down_c = y;
	int left_r = x, left_c = y - 1;

	// kiem tra o lien ke voi 
	switch (puz->rules[x][y]) {
	case L:
		c.x = right_r;
		c.y = right_c;
		break;
	case R:
		c.x = left_r;
		c.y = left_c;
		break;
	case T:
		c.x = down_r;
		c.y = down_c;
		break;
	case B:
		c.x = up_r;
		c.y = up_c;
		break;
	}
	switch (magnets[x][y]) {
	case POS:
		c.pole = NEG;
		break;
	case NEG:
		c.pole = POS;
		break;
	case EMPTY:
		c.pole = EMPTY;
		break;
	}
	return c;
}

int check_safe(const puzzle *puz, int magnets[][N], int x, int y, int val) {
	int neg_count = 0, pos_count = 0;
	cell match_pos;
	if (magnets[x][y] == EMPTY) {
		magnets[x][y] = val;
		match_pos = find_match(puz, magnets, x, y);
		magnets[match_pos.x][match_pos.y] = match_pos.pole;
		for (int i = 0; i < M; i++) {
			neg_count = 0;
			pos_count = 0;
			for (int k = 0; k < N; k++) {
				if (magnets[i][k] == NEG) {
					neg_count++;
				}
				if (magnets[i][k] == POS) {
					pos_count++;
				}
			}
			if (puz->left[i] > 0 && pos_count > puz->left[i]) {
				magnets[x][y] = EMPTY;
				magnets[match_pos.x][match_pos.y] = EMPTY;
				return 0;
			}
			if (puz->right[i] > 0 && neg_count > puz->right[i]) {
				magnets[x][y] = EMPTY;
				magnets[match_pos.x][match_pos.y] = EMPTY;
				return 0;
			}
		}
		for (int k = 0; k < N; k++) {
			neg_count = 0;
			pos_count = 0;
			for (int i = 0; i < M; i++) {
				if (magnets[i][k] == NEG) {
					neg_count++;
				}
				if (magnets[i][k] == POS) {
					pos_count++;
				}
			}
			if (puz->top[k] > 0 && pos_count > puz->top[k]) {
				magnets[x][y] = EMPTY;
				magnets[match_pos.x][match_pos.y] = EMPTY;
				return 0;
			}
			if (puz->bottom[k] > 0 && neg_count > puz->bottom[k]) {
				magnets[x][y] = EMPTY;
				magnets[match_pos.x][match_pos.y] = EMPTY;
				return 0;
			}
		}
		magnets[x][y] = EMPTY;
		magnets[match_pos.x][match_pos.y] = EMPTY;
		return 1;
	}
	else {
		return 0;
	}

}

int check_valid(const puzzle *puz, int magnets[][N]) {
	int neg_count = 0, pos_count = 0;
	for (int i = 0; i < M; i++) {
		neg_count = 0;
		pos_count = 0;
		for (int k = 0; k < N; k++) {
			if (magnets[i][k] == NEG) {
				neg_count++;
			}
			if (magnets[i][k] == POS) {
				pos_count++;
			}
		}
		if (puz->left[i] > 0 && pos_count != puz->left[i]) {
			return 0;
		}
		if (puz->right[i] > 0 && neg_count != puz->right[i]) {
			return 0;
		}
	}
	for (int k = 0; k < N; k++) {
		neg_count = 0;
		pos_count = 0;
		for (int i = 0; i < M; i++) {
			if (magnets[i][k] == NEG) {
				neg_count++;
			}
			if (magnets[i][k] == POS) {
				pos_count++;
			}
		}
		if (puz->top[k] > 0 && pos_count != puz->top[k]) {
			return 0;
		}
		if (puz->bottom[k] > 0 && neg_count != puz->bottom[k]) {
			return 0;
		}
	}
	return 1;
}

void print_solution(const puzzle *puz, int magnets[][N]) {
	// printf("   ");
	// for (int i = 0; i < N; i++) {
	// 	if (top[i] > 0) {
	// 		printf(" %d ", top[i]);
	// 	}
	// 	else printf("   ");
	// }
	printf("#---#---#---#---#---#---#\n");
	for (int i = 0; i < M; i++) {
		printf("|");
		int rm_pos[N] = { 0 };
		for (int k = 0; k < N; k++) {
			char c = 0;
			int x = puz->rules[i][k];
			if (x == L) {
				c = ' ';
			}
			else if (x == R || x == B) {
				c = '|';
			}
			else if (x == T) {
				c = '|';
				rm_pos[k] = 1;
			}
			printf(" %c %c", magnets[i][k], c);
		}
		printf("\n");
		for (int m = 0; m < N; m++) {
			printf("#");
			if (rm_pos[m]) {
				printf("   ");
			}
			else printf("---");
		}
		printf("#");
		printf("\n");
	}
}
void clear_solution(int magnets[][N]) {
	for (int i = 0; i < M; i++) {
		for (int k = 0; k < N; k++) {
			magnets[i][k] = EMPTY;
		}
	}
}

//long long count = 0;
int solve_problem(const puzzle *puz, int magnets[][N], int x, int y) {
	//count++;
	if (x >= M - 1 && y >= N - 1) {
		if (check_valid(puz, magnets)) {
			return 1;
		}
		return 0;
	}
	if (y >= N) {
		x = x + 1;
		y = 0;
	}
	if (check_safe(puz, magnets, x, y, POS)) {
		magnets[x][y] = POS;
		cell match = find_match(puz, magnets, x, y);
		magnets[match.x][match.y] = NEG;
		if (solve_problem(puz, magnets, x, y + 1)) {
			return 1;
		}
		magnets[x][y] = EMPTY;
		magnets[match.x][match.y] = EMPTY;
	}
	if (check_safe(puz, magnets, x, y, NEG)) {
		magnets[x][y] = NEG;
		cell match = find_match(puz, magnets, x, y);
		magnets[match.x][match.y] = POS;
		if (solve_problem(puz, magnets, x, y + 1)) {
			return 1;
		}
		magnets[x][y] = EMPTY;
		magnets[match.x][match.y] = EMPTY;
	}
	if (solve_problem(puz, magnets, x, y + 1)) {
		return 1;
	}
	return 0;
}
