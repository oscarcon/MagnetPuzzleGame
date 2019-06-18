#include <iostream>
#include <algorithm>
#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>

#include "MagnetPuzzleCore.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int OFFSET = 50;

puzzle puz = puzzle_default;

void UpdateMagnet(SDL_Surface* surface, int magnets[][N], SDL_Rect board[][N]) {
	static SDL_Surface* img_pos = IMG_Load("img/magnet1.png");
	static SDL_Surface* img_pos_rotated = IMG_Load("img/magnet1_rotated.png");
	static SDL_Surface* img_neg = IMG_Load("img/magnet2.png");
	static SDL_Surface* img_neg_rotated = IMG_Load("img/magnet2_rotated.png");
	static SDL_Surface* img_empty = IMG_Load("img/empty.png");
	static SDL_Surface* img_empty_rotated = IMG_Load("img/empty_rotated.png");

	for (int i = 0; i < M; i++) {
		for (int k = 0; k < N; k++) {
			if (board[i][k].x != -1) {
				if (puz.rules[i][k] == L) {
					if (magnets[i][k] == POS) {
						SDL_BlitSurface(img_pos, NULL, surface, &board[i][k]);
					}
					if (magnets[i][k] == NEG) {
						SDL_BlitSurface(img_neg, NULL, surface, &board[i][k]);
					}
					if (magnets[i][k] == EMPTY) {
						SDL_BlitSurface(img_empty, NULL, surface, &board[i][k]);
					}
				}
				if (puz.rules[i][k] == T) {
					if (magnets[i][k] == POS) {
						SDL_BlitSurface(img_pos_rotated, NULL, surface, &board[i][k]);
					}
					if (magnets[i][k] == NEG) {
						SDL_BlitSurface(img_neg_rotated, NULL, surface, &board[i][k]);
					}
					if (magnets[i][k] == EMPTY) {
						SDL_BlitSurface(img_empty_rotated, NULL, surface, &board[i][k]);
					}
				}

			}
		}
	}
}
void init_board(SDL_Surface* surface, SDL_Rect board[][N]) {
	SDL_Surface* img = IMG_Load("img/empty.png");
	SDL_Surface* img_rotated = IMG_Load("img/empty_rotated.png");
	for (int i = 0; i < M; i++) {
		for (int k = 0; k < N; k++) {
			if (board[i][k].x != -1) {
				if (board[i][k].w == 54)
					SDL_BlitSurface(img, NULL, surface, &board[i][k]);
				if (board[i][k].w == 26)
					SDL_BlitSurface(img_rotated, NULL, surface, &board[i][k]);
			}
		}
	}
}

void render_text(SDL_Surface* screen, SDL_Rect horizon[][N], SDL_Rect vertical[][M], puzzle* puz) {
	SDL_Surface* surface;
	TTF_Font* Sans = TTF_OpenFont("fonts/Minecraft.ttf", 16); //this opens a font style and sets a size
	std::cout << TTF_GetError() << std::endl;
	SDL_Color black = { 0, 0, 0, 0};
	SDL_Rect pos_rect = { OFFSET - 26, OFFSET - 26, 26, 26 };
	SDL_Rect neg_rect = { OFFSET + N * 28, OFFSET + M * 28, 26, 26 };

	surface = TTF_RenderText_Blended(Sans, "+", { 255,0,0,255 });
	SDL_BlitSurface(surface, NULL, screen, &pos_rect);
	surface = TTF_RenderText_Blended(Sans, "-", { 0,0,255,255 });
	SDL_BlitSurface(surface, NULL, screen, &neg_rect);

	for (int i = 0; i < M; i++) {
		char str[3];
		if (puz->left[i] < 0) {
			strcpy_s(str, 1, "");
		}
		else {
			sprintf_s(str, 2, "%d", puz->left[i]);
		}
		surface = TTF_RenderText_Blended(Sans, str, black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface 
		SDL_BlitSurface(surface, NULL, screen, &vertical[0][i]);
		if (puz->right[i] < 0) {
			strcpy_s(str, 1, "");
		}
		else {
			sprintf_s(str, 2, "%d", puz->right[i]);
		}
		surface = TTF_RenderText_Blended(Sans, str, black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_BlitSurface(surface, NULL, screen, &vertical[1][i]);
	}
	for (int i = 0; i < N; i++) {
		char str[3];
		if (puz->top[i] < 0) {
			strcpy_s(str, 1, "");
		}
		else {
			sprintf_s(str, 2, "%d", puz->top[i]);
		}
		surface = TTF_RenderText_Solid(Sans, str, black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface first
		SDL_BlitSurface(surface, NULL, screen, &horizon[0][i]);
		if (puz->bottom[i] < 0) {
			strcpy_s(str, 1, "");
		}
		else {
			sprintf_s(str, 2, "%d", puz->bottom[i]);
		}
		surface = TTF_RenderText_Solid(Sans, str, black); // as TTF_RenderText_Solid could only be used on SDL_Surface then you have to create the surface 
		SDL_BlitSurface(surface, NULL, screen, &horizon[1][i]);
	}
}

bool mouse_over(SDL_Rect* rect) {
	int mouse_x, mouse_y;
	SDL_GetMouseState(&mouse_x, &mouse_y);
	if (mouse_x >= rect->x && mouse_x <= rect->x + rect->w) {
		if (mouse_y >= rect->y && mouse_y <= rect->y + rect->h) {
			return true;
		}
	}
	return false;
}

int main(int argc, char** agrv) {
	SDL_Surface* start_img = IMG_Load("img/start.png");
	SDL_Surface* custom_img = IMG_Load("img/custom.png");
	SDL_Surface* exit_img = IMG_Load("img/exit.png");

	SDL_Surface* img = IMG_Load("img/empty.png");
	SDL_Surface* img_rotated = IMG_Load("img/empty_rotated.png");
	SDL_Surface* img_pos = IMG_Load("img/magnet1.png");
	SDL_Surface* img_pos_rotated = IMG_Load("img/magnet1_rotated.png");
	SDL_Surface* img_neg = IMG_Load("img/magnet2.png");
	SDL_Surface* img_neg_rotated = IMG_Load("img/magnet2_rotated.png");

	SDL_Surface* correct = IMG_Load("img/correct.png");
	SDL_Surface* notcorrect = IMG_Load("img/notcorrect.png");
	SDL_Rect check_rect = { 300, 80, 100, 100 };

	int magnets[M][N] = { EMPTY };
	SDL_Rect menu[3];
	menu[0] = { 0, 0, start_img->w, start_img->h };
	menu[1] = { 0, menu[0].y + menu[0].h + 10, menu[0].w, menu[0].h };
	menu[2] = { 0, menu[1].y + menu[1].h + 10, menu[0].w, menu[0].h };

	SDL_Window* window = NULL;
	SDL_Surface* screenSurface = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Texture* gTexture = NULL;
	bool quit = false;
	SDL_Event e;

	SDL_Rect board[M][N];
	SDL_Rect rules_vertical[2][M];
	SDL_Rect rules_horizontal[2][N];

	// init boards location
	int index = 0;
	for (int i = 0; i < M; i++) {
		int x = 0;
		for (int k = 0; k < N; k++) {
			int y = i * 28;
			if (puz.rules[i][k] == L) {
				board[i][k].x = x;
				board[i][k].y = y;
				board[i][k].w = 54;
				board[i][k].h = 26;
				x += 56;
			}
			if (puz.rules[i][k] == T) {
				board[i][k].x = x;
				board[i][k].y = y;
				board[i][k].w = 26;
				board[i][k].h = 54;
				x += 28;
			}
			if (puz.rules[i][k] == B) {
				x += 28;
			}
			if (puz.rules[i][k] == R || puz.rules[i][k] == B) {
				board[i][k] = { -1, -1, -1, -1 };
			}
		}
	}
	for (int i = 0; i < M; i++) {
		for (int k = 0; k < N; k++) {
			if (board[i][k].x != -1) {
				board[i][k].x += OFFSET;
				board[i][k].y += OFFSET;
			}

		}
	}

	// init rules location based on board
	for (int i = 0; i < M; i++) {
		rules_vertical[0][i].x = OFFSET - 26;
		rules_vertical[0][i].y = OFFSET + 28 * i;
		rules_vertical[1][i].x = OFFSET + 28 * N;
		rules_vertical[1][i].y = OFFSET + 28 * i;
		rules_vertical[0][i].w = 26;
		rules_vertical[0][i].h = 26;
		rules_vertical[1][i].w = 26;
		rules_vertical[1][i].h = 26;
	}
	for (int i = 0; i < N; i++) {
		rules_horizontal[0][i].x = OFFSET + 28 * i;
		rules_horizontal[0][i].y = OFFSET - 26;
		rules_horizontal[1][i].x = OFFSET + 28 * i;
		rules_horizontal[1][i].y = OFFSET + 28 * M;
		rules_horizontal[0][i].w = 26;
		rules_horizontal[0][i].h = 26;
		rules_horizontal[1][i].w = 26;
		rules_horizontal[1][i].h = 26;
	}

	if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
		printf("SDL init error %s\n", SDL_GetError());
	}
	else {
		TTF_Init();
		window = SDL_CreateWindow("Magnet Puzzle", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
		if (window == NULL) {
			printf("Window can't be created %s\n", SDL_GetError());
		}
		else {
			gRenderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			if (gRenderer == NULL) {
				printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
			}
			else {
				/*SDL_SetRenderDrawColor(gRenderer, 213, 211, 206, 0xff);
				SDL_RenderClear(gRenderer);
				SDL_RenderPresent(gRenderer);*/
			}
			screenSurface = SDL_GetWindowSurface(window);
			SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 213, 211, 206));
			SDL_BlitSurface(start_img, NULL, screenSurface, &menu[0]);
			SDL_BlitSurface(custom_img, NULL, screenSurface, &menu[1]);
			SDL_BlitSurface(exit_img, NULL, screenSurface, &menu[2]);
			SDL_UpdateWindowSurface(window);
			/*SDL_SetRenderDrawColor(gRenderer, 0xff, 0x00, 0x00, 0xff);
			for (int i = 0; i < M * N / 2; i++) {
				SDL_RenderDrawRect(gRenderer, &board[i]);
			}
			SDL_RenderPresent(gRenderer);*/
		}
	}

	while (!quit) {
		bool start = false;
		bool exit = false;
		bool custom = false;

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_MOUSEBUTTONDOWN) {
				if (mouse_over(&menu[0])) {
					screenSurface = SDL_GetWindowSurface(window);
					SDL_FillRect(screenSurface, NULL, SDL_MapRGB(screenSurface->format, 213, 211, 206));
					init_board(screenSurface, board);
					render_text(screenSurface, rules_horizontal, rules_vertical, &puz);
					SDL_UpdateWindowSurface(window);
					start = true;
				}
				if (mouse_over(&menu[1])) {
					custom = true;
				}
				if (mouse_over(&menu[2])) {
					exit = true;
				}
			}
		}

		// running the game
		while (start) {
			SDL_Event e;
			static int count[M][N] = { 0 };
			cell c;
			while (SDL_PollEvent(&e) != 0) {
				if (e.type == SDL_QUIT) {
					quit = true;
					start = false;
				}
				
				if (e.type == SDL_KEYDOWN) {
					if (e.key.keysym.sym == SDLK_s) {
						std::cout << "Sovling" << std::endl;
						clear_solution(magnets);
						solve_problem(&puz, magnets, 0, 0);
						UpdateMagnet(screenSurface, magnets, board);
						SDL_UpdateWindowSurface(window);
					}
				}
				if (e.type == SDL_MOUSEBUTTONDOWN) {
					for (int i = 0; i < M; i++) {
						for (int k = 0; k < N; k++) {
							if (mouse_over(&board[i][k])) {
								cell c = find_match(&puz, magnets, i, k);
								std::cout << i << ',' << k << std::endl;
								count[i][k] += 1;
								if (count[i][k] > 2) count[i][k] = 0;
								switch (count[i][k]) {
								case 0:
									magnets[i][k] = EMPTY;
									magnets[c.x][c.y] = EMPTY;
									break;
								case 1:
									magnets[i][k] = POS;
									magnets[c.x][c.y] = NEG;
									break;
								case 2:
									magnets[i][k] = NEG;
									magnets[c.x][c.y] = POS;
									break;
								}
								UpdateMagnet(screenSurface, magnets, board);
							}
						}
					}

					SDL_FillRect(screenSurface, &check_rect, SDL_MapRGB(screenSurface->format, 213, 211, 206));
					if (check_valid(&puz, magnets)) {
						SDL_BlitSurface(correct, NULL, screenSurface, &check_rect);
					}
					else {
						SDL_BlitSurface(notcorrect, NULL, screenSurface, &check_rect);
					}
					SDL_UpdateWindowSurface(window);
				}
			}
		}
	}
	SDL_DestroyWindow(window);
	SDL_Quit();
	return 0;
}