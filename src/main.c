#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_stdinc.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_timer.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>
#include <stdbool.h>

#include "defs.h"
#include "render.c"

struct App {
	SDL_Window *window;
	SDL_Renderer *renderer;
	bool is_pause;
} App;

struct SortInf {
	int i_pos;
	int j_pos;
	bool sorted;
} SortInf;

void bubble_sort_step(struct SortInf *inf, struct ElemDisp *disp);
void selection_sort_step(struct SortInf *inf, struct ElemDisp *disp);
void init_inf(struct SortInf *inf);
int main() {

	int total_frames = 0;
	int total_steps = 0;
	int steps_per_tick = 1;

	char frames[40];
	char steps [40];
	char fps[40];
	char delay_txt[40];
	char step_txt[40];
	
	struct App app;
	struct ElemDisp disp;
	struct SortInf inf;
	TTF_Font *font; 
	
	app.is_pause = true;

	setup_elem_disp(&disp);
	init_inf(&inf);

	if(SDL_Init(SDL_INIT_VIDEO)) {
		printf("SDL2 Error (Video) : %s\n", SDL_GetError());
		return 1;
	}

	app.window = SDL_CreateWindow(
		"Visual Sort", 
		SDL_WINDOWPOS_CENTERED, 
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH, 
		SCREEN_HEIGHT, 
		0
	);
	
	if (!app.window) {
		printf("SDL2 Error (Window) : %s\n", SDL_GetError());
		return (1);
	}
	
	app.renderer = SDL_CreateRenderer(app.window, -1, SDL_RENDERER_ACCELERATED);

	if (!app.renderer) {
		printf("SDL2 Error (Renderer) : %s\n", SDL_GetError());
		return(1);
	}

	if (TTF_Init() < 0) {
		printf("SDL2 Error (Fonts) : %s\n", TTF_GetError());
		return (1);
	}

	font = TTF_OpenFont("./fonts/Roboto-Medium.ttf", 20);

	if (!font) {
		printf("Failed to load font %s\n", TTF_GetError());
	}


	bool running = true;
	bool delay = true;
	SDL_Event event;

	Uint32 fps_last = SDL_GetTicks();
	Uint32 fps_curr = 0;
	Uint32 fps_frames = 0;
	
	while (running) {
		SDL_SetRenderDrawColor(app.renderer, 0x11, 0x11, 0x11, SDL_ALPHA_OPAQUE);
		SDL_RenderClear(app.renderer);

		// render text
		// would be better to take this into its own function but w/e for now
		sprintf(frames, "Total frames: %i", total_frames);
		sprintf(steps,  "Total steps:  %i", total_steps);
		sprintf(fps,    "Current fps:  %i", fps_curr);
		sprintf(delay_txt, "Delay is %s", delay ? "ON" : "OFF");
		sprintf(step_txt, "Current steps per tick: %i", steps_per_tick);

		render_txt(app.renderer, font, frames, SORT_WIDTH + OFFSET * 2, OFFSET);		
		render_txt(app.renderer, font, steps, SORT_WIDTH + OFFSET * 2, OFFSET * 2);		
		render_txt(app.renderer, font, fps, SORT_WIDTH + OFFSET * 2, OFFSET * 3);		
		render_txt(app.renderer, font, delay_txt, SORT_WIDTH + OFFSET * 2, OFFSET * 4);
		render_txt(app.renderer, font, step_txt, SORT_WIDTH + OFFSET * 2, OFFSET * 5);

		render_txt(app.renderer, font, "Press P to play/pause", SORT_WIDTH + OFFSET * 2, OFFSET * 7);
		render_txt(app.renderer, font, "Press R to reset array", SORT_WIDTH + OFFSET * 2, OFFSET * 8);
		render_txt(app.renderer, font, "Press T to reset frames", SORT_WIDTH + OFFSET * 2, OFFSET * 9);
		render_txt(app.renderer, font, "Press S to step through", SORT_WIDTH + OFFSET * 2, OFFSET * 10);
		render_txt(app.renderer, font, "Press D to toggle delay", SORT_WIDTH + OFFSET * 2, OFFSET * 11);
		render_txt(app.renderer, font, "Press [ or ] to change steps", SORT_WIDTH + OFFSET * 2, OFFSET * 12);

		// Event loop
		while(SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT:
					running = false;
					break;
				case SDL_KEYUP:
					break;
				case SDL_KEYDOWN:
					switch(event.key.keysym.sym) {
						case SDLK_ESCAPE:
							running = false;
							break;
						case SDLK_p:
							app.is_pause = !app.is_pause;
							break;
						case SDLK_r:
							setup_elem_disp(&disp);
							init_inf(&inf);
							total_steps = 0;
							app.is_pause = true;
							break;
						case SDLK_t:
							total_frames = 0;
							break;
						case SDLK_s:
							app.is_pause = true;
							reset_high(&disp);
							total_steps += 1;
							bubble_sort_step(&inf, &disp);
							break;
						case SDLK_d:
							delay = !delay;
							break;
						case SDLK_RIGHTBRACKET:
							steps_per_tick++;
							break;
						case SDLK_LEFTBRACKET:
							if (steps_per_tick > 1)
								steps_per_tick--;
							break;
						case SDLK_l:
							steps_per_tick = 0;
							break;

							
					}
					break;
			}
		}

		// Render Loop
		draw_elem_disp(&disp, app.renderer);
	
		// Update Loop
		if (!inf.sorted && !app.is_pause) {

			for (int i = 0; i < steps_per_tick; i++) {
				reset_high(&disp);
				bubble_sort_step(&inf, &disp);
			}
			total_steps += steps_per_tick;
			if (delay)
				SDL_Delay(SORT_DELAY);
		} else {
			if (delay)
				SDL_Delay(DELAY);
		}
		
		// FPS calc
		fps_frames++;
		if (fps_last < SDL_GetTicks() - FPS_INTERVAL * 1000) {
			fps_last = SDL_GetTicks();
			fps_curr = fps_frames / FPS_INTERVAL;
			fps_frames = 0;
		}

		total_frames += 1;

		SDL_RenderPresent(app.renderer);
	//	SDL_Delay(1);
	}
	
	TTF_Quit();
	SDL_DestroyRenderer(app.renderer);
	SDL_DestroyWindow(app.window);
	SDL_Quit();

	return 0;
}

void bubble_sort_step(struct SortInf *inf, struct ElemDisp *disp) {
	
	// ensure we dont do the actual sort portion if already sorted
	if(inf->sorted){
		return;
	}

	// hacky nested for loop equivalent but step by step
	if (inf->j_pos >= NUM_ELEMENTS - 2 - inf->i_pos) {
		disp->highlight[NUM_ELEMENTS - 1 - inf->i_pos] = SORTED;
		inf->j_pos = 0;
		inf->i_pos += 1;
	
		if (inf->i_pos >= NUM_ELEMENTS) {
			inf->sorted = true;
			disp->highlight[1] = SORTED;
			return;
		}
		
	} else {
		inf->j_pos += 1;
	}

	// actual sort
	if (disp->elems[inf->j_pos] > disp->elems[inf->j_pos + 1]) {
		int x = disp->elems[inf->j_pos];
		disp->elems[inf->j_pos] = disp->elems[inf->j_pos + 1];
		disp->elems[inf->j_pos + 1] = x;
		disp->highlight[inf->j_pos + 1] = HIGH;
		disp->highlight[inf->j_pos] = LOW;
	} else {
		disp->highlight[inf->j_pos + 1] = LOW;
		disp->highlight[inf->j_pos] = HIGH;
	}
	
}

void init_inf(struct SortInf *inf) {
	inf->i_pos = 0;
	inf->j_pos = 0;
	inf->sorted = false;
}
