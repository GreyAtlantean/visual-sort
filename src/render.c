#ifndef RENDER_C
#define RENDER_C
#include <SDL2/SDL.h>
#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_ttf.h>

#include <time.h>

#include "defs.h"

enum colours {
	BASE = 0,
	HIGH = 1,
	LOW = 2,
	SORTED = 3,
};

struct ElemDisp {
	int elems[NUM_ELEMENTS];
	enum colours highlight[NUM_ELEMENTS];
	float elemwdth;
} ElemDisp;

int draw_elem_disp (struct ElemDisp *disp, SDL_Renderer *renderer);
int setup_elem_disp(struct ElemDisp *disp);
void print_elems(struct ElemDisp *disp);
void reset_high(struct ElemDisp *disp);
void change_col(enum colours col, SDL_Renderer *renderer);
void render_txt(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y);
	
int draw_elem_disp (struct ElemDisp *disp, SDL_Renderer *renderer) {
	SDL_Rect back;
	back.x = OFFSET;
	back.y = OFFSET;
	back.w = SORT_WIDTH;
	back.h = SORT_HEIGHT;
	
	SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(renderer, &back);	
	
	SDL_Rect e;
	e.w = (int)disp->elemwdth;
	
	if (e.w == 0) {
		e.w = 1;
	}
	
	for (int i = 0; i < NUM_ELEMENTS; i++) {
		change_col(disp->highlight[i], renderer);		
		e.x = (int)(OFFSET + (disp->elemwdth * i) + 1);
		e.y = OFFSET + 1;
		e.h = disp->elems[i] * (SORT_HEIGHT - OFFSET / 2) / MAX_VAL;
		SDL_RenderFillRect(renderer, &e);
	}
	return 0;
}


int setup_elem_disp(struct ElemDisp *disp) {

	disp->elemwdth = SORT_WIDTH / (float)NUM_ELEMENTS;

	srand(time(NULL));

	for (int i = 0; i < NUM_ELEMENTS; i++) {
		disp->elems[i] = MIN_VAL + (rand() % (MAX_VAL - MIN_VAL));
		disp->highlight[i] = BASE;
	}

	return 0;
}

void reset_high(struct ElemDisp *disp) {
	
	for (int i = 0; i < NUM_ELEMENTS; i++) {
		if (disp->highlight[i] != SORTED)
			disp->highlight[i] = BASE;
	}

}

void change_col(enum colours col, SDL_Renderer *renderer) {
	
	switch (col) {
		case BASE:
			SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0x55, SDL_ALPHA_OPAQUE);
			break;
		case HIGH:
			SDL_SetRenderDrawColor(renderer, 0x66, 0xff, 0x66, SDL_ALPHA_OPAQUE);
			break;
		case LOW:
			SDL_SetRenderDrawColor(renderer, 0x55, 0x55, 0xdd, SDL_ALPHA_OPAQUE);
			break;
		case SORTED:
			SDL_SetRenderDrawColor(renderer, 0xaa, 0xaa, 0xaa, SDL_ALPHA_OPAQUE);
			break;	

	}

}


void print_elems(struct ElemDisp *disp) {
	
	for (int i = 0; i < NUM_ELEMENTS; i++) {
		printf("Elem %i is %i\n", i, disp->elems[i]);
	}

}

void render_txt(SDL_Renderer *renderer, TTF_Font *font, const char *text, int x, int y) {
	SDL_Color colour = {0x99, 0x99, 0x99};

	SDL_Surface *surf = TTF_RenderText_Blended(font, text, colour);
	if (!surf) {
		return;
	}

	SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surf);

	SDL_Rect destRect = {x, y, surf->w, surf->h};
	SDL_RenderCopy(renderer, texture, NULL, &destRect);

	SDL_DestroyTexture(texture);
	SDL_FreeSurface(surf);

	return;
}
	
	
#endif
