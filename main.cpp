#include "project.h"

int main(int argc, char* args[]) {
	srand(time(0));
	bool run = true;

	SDL_Init(SDL_INIT_EVERYTHING);
	
	//	create window
	SDL_Window* window = nullptr;
	window = CreateWindow("Snake. v 0.3",window,&run);
	
	//	create surface
	SDL_Surface* screen = SDL_GetWindowSurface(window);
	if (screen == NULL) {
		run = false;
	}
	//	launch TTF
	TTF_Init();

	//	create render
	SDL_Renderer* render = nullptr;
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	// body of snake element
	Texture snakeElement;
	snakeElement.loadImage(render, "body.bmp");
	snakeElement.setPosition(15, 150, 15, 15);

	//	generate food
	Texture apple,bigApple;
	bigApple.loadImage(render, "apple.bmp");
	bigApple.setRandPositionBigApple();
	apple.loadImage(render, "apple.bmp");
	apple.setRandPosition();


	// background
	Texture background;
	background.loadImage(render, "background.bmp");
	background.setPosition(0, 0, SCR_WIDTH, SCR_HEIGHT);

	// background panel
	Texture panel;
	panel.loadImage(render, "panel.bmp");
	panel.setPosition(0, 0, 630, 45);

	// score panel
	string tScore = "score:";
	Text score;

	//	Make a snake
	vector<SDL_Rect> snake;
	snake.push_back(snakeElement.rect);
	
	//	event
	SDL_Event occur;
	if (SDL_PollEvent(&occur) == NULL)
		run = false;

	//	active direction flag
	int actDir = 3;
	int points = 1;

	vector<int> scoreTable;
	ifstream read("score.txt");
	while (!read.eof()) {
		int i=0;
		read >> i;
		scoreTable.push_back(i);
	}
	read.close();

	int bestScore = 0;
	for (int i = 0; i < scoreTable.size(); i++) {
		if (scoreTable[i] > bestScore)
			bestScore = scoreTable[i];
	}

	SDL_RenderClear(render);

	background.copyImageInRender(render);

	tScore = "Press      'space'      to      start('esc'      to      end)";
	score.setTextPosition(100, 200, 400,70);
	score.setTextTexture(render, tScore, cWhite);
	score.copyTextInRender(render);
	SDL_RenderPresent(render);
	eventStart(occur, run);

	int start = 0;
	//	game
	while (run) {
		background.copyImageInRender(render);
		
		//	event
		event(occur,run,actDir);

		//	rise snake
		if (snake[0].x == apple.x && snake[0].y == apple.y) {
			SDL_Rect tmp = snake[snake.size() - 1];
			snake.push_back(tmp);
			apple.setRandPosition();
			points++;
		}
		if (snake[0].x >= bigApple.x && snake[0].x < (bigApple.x + bigApple.w) && snake[0].y >= bigApple.y &&  snake[0].y < (bigApple.y + bigApple.h)){
			SDL_Rect tmp = snake[snake.size() - 1];
			snake.push_back(tmp);
			bigApple.setRandPositionBigApple();
			points += 5;
		}

		//	check on going beyond
		checkBeyond(snake[0]);

		if (snake.size() >= 1) {

			// movement of the tail(shift position )
			for (int i = snake.size() - 2,j=i+1; i >=0 ; i--,j--)
					snake[j] = snake[i];
		}

		//	move the snake in active direction
		moveSnakeHead(actDir, snake[0]);

		//	drawing snake
		if (snake[0].y >= 45){
			snakeElement.rect = snake[0];
			snakeElement.copyImageInRender(render);
		}

		for (int i = 1; i < snake.size(); i++) {
			snakeElement.rect = snake[i];
			snakeElement.copyImageInRender(render);
		}
		SDL_Delay(80);

		//	draw food
		apple.copyImageInRender(render);
		int start = SDL_GetTicks();
		if (start % 10000 <= 5000) {
			bigApple.copyImageInRender(render);
		}
		else {
			bigApple.setRandPositionBigApple();
		}

		// draw panel
		panel.copyImageInRender(render);

		// update score and print score
		tScore = "score:";
		score.setTextPosition(0, 8, 70, 30);
		score.setTextTexture(render, tScore, cWhite);
		score.copyTextInRender(render);

		tScore = to_string(points);
		score.setTextPosition(80,8, 20, 30);
		score.setTextTexture(render, tScore, cWhite);
		score.copyTextInRender(render);
	

		if (points > bestScore) 
			bestScore = points;

		tScore = "best: " + to_string(bestScore);
		score.setTextPosition(200, 8, 70, 30);
		score.setTextTexture(render, tScore, cWhite);
		score.copyTextInRender(render);

		// check on collision										  
		SDL_Rect tmp = snake[0];
		for (int i = 1; i < snake.size(); i++) {
			if (tmp.x == snake[i].x && tmp.y == snake[i].y) {
				
				ofstream write("score.txt");
				write << bestScore;
				write.close();

				// print message on window
				tScore = "Game over";
				score.setTextPosition(180, 150, 250, 80);
				score.setTextTexture(render, tScore, cWhite);
				score.copyTextInRender(render);

				SDL_RenderPresent(render);

				// print message on window
				tScore = "Play more(y/n)?";
				score.setTextPosition(220, 250, 180, 40);
				score.setTextTexture(render, tScore, cYellow);
				score.copyTextInRender(render);

				SDL_RenderPresent(render);
				
				eventExit(occur,run);

				//if run, update all for next game
				if (run) {
					actDir = 3;
					snake.clear();
					snake.push_back(snakeElement.rect);
					points = 1;
				}
			}
		}

		//	update render
		SDL_RenderPresent(render);
		SDL_RenderClear(render);
	}
	
	window = nullptr;
	screen = nullptr;
	render = nullptr;

	SDL_DestroyWindow(window);
	SDL_DestroyRenderer(render);

	SDL_Quit();
	return 0;
}
