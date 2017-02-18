#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <string>
#include <fstream>

using namespace std;

const int SCR_WIDTH = 630;
const int SCR_HEIGHT = 480;
const SDL_Color cWhite = { 255,255,255 };
const SDL_Color cYellow = { 255,255,0 };

struct Texture {
	SDL_Texture* texture = nullptr;
	SDL_Rect rect;
	int x,y,w,h;
	void loadImage(SDL_Renderer* render,char* directory);
	void setPosition(int xx, int yy, int ww, int hh);
	void setRandPosition();
	void setRandPositionBigApple();
	void copyImageInRender(SDL_Renderer* render);
};

void Texture::setPosition(int xx, int yy, int ww, int hh) {
	x = xx;
	y = yy;
	w = ww;
	h = hh;
	rect = { x,y,w,h };
}

void Texture::setRandPosition() {
	int a = 0, b = 0;
	a = (rand() % 35 + 5) * 15;
	b = (rand() % 25 + 5) * 15;
	while (sqrt((a - x)^2 - (b-y)^2)  < 15){
		a = (rand() % 40) * 15;
		b = (rand() % 25 + 4) * 15;
	}
	rect.x = x =a;
	rect.y = y =b;
	rect.h = h =15;
	rect.w = w =15;
}

void Texture::setRandPositionBigApple() {
	int a = 0, b = 0;
	a = (rand() % 35 + 5) * 15;
	b = (rand() % 25 + 5) * 15;
	while (sqrt((a - x) ^ 2 - (b - y) ^ 2)  < 20) {
		a = (rand() % 40) * 15;
		b = (rand() % 25 + 4) * 15;
	}
	rect.x = x = a;
	rect.y = y = b;
	rect.h = h = 30;
	rect.w = w = 30;
}

void Texture::loadImage(SDL_Renderer* render, char* directory) {
	texture = IMG_LoadTexture(render, directory);
}

void Texture::copyImageInRender(SDL_Renderer* render) {
	SDL_RenderCopy(render, texture, NULL, &rect);
}

struct Text {
	TTF_Font* font = TTF_OpenFont("BEBAS.ttf", 20);
	SDL_Texture* textTexture = nullptr;
	SDL_Rect textRect;
	int x, y, w, h;
	void setTextPosition(int xx, int yy, int ww, int hh);
	void setTextTexture(SDL_Renderer* render, string text, SDL_Color color);
	void copyTextInRender(SDL_Renderer* render);
};

void Text::setTextTexture(SDL_Renderer* render,string text, SDL_Color color) {
	textTexture = SDL_CreateTextureFromSurface(render, TTF_RenderText_Solid(font, text.c_str(), color));
}

void Text::setTextPosition(int xx, int yy, int ww, int hh) {
	x = xx;
	y = yy;
	w = ww;
	h = hh;
	textRect = { x,y,w,h };
}

void Text::copyTextInRender(SDL_Renderer* render) {
	SDL_RenderCopy(render, textTexture, NULL, &textRect);
}

SDL_Window* CreateWindow(const char* title, SDL_Window* window,bool* run) {
	window = SDL_CreateWindow(title, 380, 100, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_SHOWN);
	if (window == NULL) {
		cout << SDL_GetError() << endl;
		run = false;
	}
	return window;
}

void checkBeyond(SDL_Rect& rect) {
	if (rect.x == -15) {
		rect.x = 615;
	}
	else if (rect.x == 630) {
		rect.x = -15;
	}

	if (rect.y == 480) {
		rect.y = 45;
	}
	else if (rect.y == 30) {
		rect.y = 465;
	}

}

void event(SDL_Event occur, bool& run, int& actDir) {
	if (SDL_PollEvent(&occur)) {
		switch (occur.type) {
		case SDL_QUIT:
			run = false;
			break;
		case SDL_KEYDOWN:
			switch (occur.key.keysym.sym) {
			case SDLK_ESCAPE:
				run = false;
				break;
			case SDLK_UP:
				if (actDir != 2) {
					actDir = 0;
				}
				break;
			case SDLK_LEFT:
				if (actDir != 3) {
					actDir = 1;
				}
				break;
			case SDLK_DOWN:
				if (actDir != 0) {
					actDir = 2;
				}
				break;
			case SDLK_RIGHT:
				if (actDir != 1) {
					actDir = 3;
				}
				break;
			}
			break;
		}
	}
}

void eventExit(SDL_Event occur, bool& run) {
	bool chose = true;
	while (chose) {
		while (SDL_PollEvent(&occur)) {
			switch (occur.type) {
			case SDL_QUIT:
				run = false;
				chose = false;
				break;
			case SDL_KEYDOWN:
				switch (occur.key.keysym.sym) {
				case SDLK_y:
					run = true;
					chose = false;
					break;
				case SDLK_n:
					run = false;
					chose = false;
					break;
				}
				break;
			}
		}
	}
}

void eventStart(SDL_Event occur, bool& run) {
	bool chose = true;
	while (chose) {
		while (SDL_PollEvent(&occur)) {
			switch (occur.type) {
			case SDL_QUIT:
				run = false;
				chose = false;
				break;
			case SDL_KEYDOWN:
				switch (occur.key.keysym.sym) {
				case SDLK_SPACE:
					run = true;
					chose = false;
					break;
				case SDLK_ESCAPE:
					run = false;
					chose = false;
					break;
				}
				break;
			}
		}
	}
}

void moveSnakeHead(int actDir, SDL_Rect& snake) {
	if (actDir == 0) {
		snake.y -= 15;
	}
	if (actDir == 1) {
		snake.x -= 15;
	}
	if (actDir == 2) {
		snake.y += 15;
	}
	if (actDir == 3) {
		snake.x += 15;
	}
}
