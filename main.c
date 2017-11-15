#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <math.h>

#define FONT "FreeSans.ttf"

typedef enum {intro, show, imediato_, incrementalBasico_, bresenham_, todos_} GameState;
typedef enum {pA, pB} PState;
typedef enum {pX, pY} PStateP;

SDL_Rect ScreenRect = {0, 0, 800, 480};

int iniciar(SDL_Window **window, SDL_Renderer **renderer);
int renderFont(SDL_Texture **texture, SDL_Renderer **renderer, TTF_Font *font, char *text, SDL_Rect *pos);
void removerCaractere(char *text);
int offset(int y);
void desenhaPonto(SDL_Renderer **renderer, int x, int y);

int imediato(SDL_Renderer **renderer, SDL_Point *point);
int incrementalBasico(SDL_Renderer **renderer, SDL_Point *point);
int bresenham(SDL_Renderer **renderer, SDL_Point *point);

int main(int argc, char **argv) {
	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;
	SDL_Texture *texture = NULL;
	SDL_Texture *texture2 = NULL;
	TTF_Font *font = NULL;
	SDL_Event e;
	SDL_Rect pos = {0,0,0,0};
	SDL_Rect pos2 = {0,0,0,0};
	SDL_Point point[2] = {
		{0,0},
		{400,333}
	};
	
	unsigned int lastTime = 0, currentTime;
	int quit = 0;
	int temValor = 0; 
	int tmp = 0;
	char text[20] = "Ponto Ax: ";
	char ctext[20] = "";
	
	GameState gstate = 0;
	PState pstate = pA;
	PStateP pstateP = pX;
	
	if (!iniciar(&window,&renderer)) {
		return 1;
	}
	
	if (TTF_Init() == -1) {
		SDL_Log("TTF_Init: %s\n", TTF_GetError());
		return 1;
	}
	
	font = TTF_OpenFont(FONT, 28);
	
	if (font == NULL)
		return 1;
	
	SDL_StartTextInput();

	while (!quit) {
		currentTime =  SDL_GetTicks();
		while(SDL_PollEvent(&e)) {
			switch(e.type) {
				case SDL_QUIT:
					quit = 1;
					break;
				case SDL_KEYDOWN:
					if (e.key.keysym.sym == SDLK_ESCAPE) {
						quit = 1;
						break;
					}
					if (gstate == intro) {
						if (e.key.keysym.sym == SDLK_RETURN) {
							temValor = 1;
							break;
						}
						if (e.key.keysym.sym == SDLK_BACKSPACE && strlen(ctext) > 0 ) {
							removerCaractere(ctext);
							break;
						}
					}
					else {
						switch (e.key.keysym.sym) {
							case SDLK_1:
								gstate = imediato_;
							break;
							case SDLK_2:
								gstate = incrementalBasico_;
							break;
							case SDLK_3:
								gstate = bresenham_;
							break;
							case SDLK_4:
								gstate = todos_;
							break;
							default:
							break;
						}
					}
					break;
				case SDL_TEXTINPUT:
					if (strlen(ctext) < 20)
						strcat(ctext, e.text.text);
					break;
			}
		}
		
		if (currentTime > lastTime + 60) {
			lastTime = currentTime;
			SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			SDL_RenderClear(renderer);
			
			switch (gstate) {
				case 0:
					switch (pstate) {
						case pA:
							if (pstateP == 0) {
								if (temValor) {
									temValor = 0;
									tmp = atoi(ctext);
									point[0].x = tmp;
									pstateP = pY;
									strcpy(text, "Ponto Ay: ");
									strcpy(ctext, "");
									tmp = 0;
								}
							}
							else {
								if (pstateP == 1) {
									if (temValor) {
										temValor = 0;
										tmp = atoi(ctext);
										point[0].y = tmp;
										pstateP = pX;
										pstate = pB;
										strcpy(text, "Ponto Bx: ");
										strcpy(ctext, "");
										tmp = 0;
									}
								}
							}
						break;
						case pB:
							if (pstateP == 0) {
								if (temValor) {
									temValor = 0;
									tmp = atoi(ctext);
									point[1].x = tmp;
									pstateP = pY;
									strcpy(text, "Ponto By: ");
									strcpy(ctext, "");
									tmp = 0;
								}
							}
							else {
								if (pstateP == 1) {
									if (temValor) {
										temValor = 0;
										tmp = atoi(ctext);
										point[1].y = tmp;
										pstateP = pX;
										pstate = pA;
										strcpy(text, "");
										strcpy(ctext, "");
										gstate = show;
										tmp = 0;
										//SDL_StopTextInput();
									}
								}
							}
						break;
						default:
						break;
					}
					pos2.x = (ScreenRect.w - pos2.w) / 2;
					pos2.y = (ScreenRect.h - pos2.h) / 2;
					
					renderFont(&texture2, &renderer, font, text, &pos2);
					
					pos.x = pos2.x + pos2.w + 5;
					pos.y = pos2.y;
			
					renderFont(&texture, &renderer, font, ctext, &pos);
					
					SDL_SetRenderDrawColor(renderer,255,255,255,255);
					if (strlen(ctext) > 0 )
						SDL_RenderCopy(renderer,texture, NULL, &pos);
					SDL_RenderCopy(renderer,texture2, NULL, &pos2);
				break;
				case show:
					//Poderia fazer uma tela que mostra as Opções:
					// 1 para imediato
					// 2 para incremental básico
					// 3 para bresenham
					// 4 para todos
				break;
				case imediato_:
					SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0xFF, 0xFF);
					imediato(&renderer,point);
				break;
				case incrementalBasico_:
					SDL_SetRenderDrawColor(renderer, 0x0, 0xFF, 0x0, 0xFF);
					incrementalBasico(&renderer,point);
				break;
				case bresenham_:
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x0, 0x0, 0xFF);
					bresenham(&renderer,point);
				break;
				case todos_:
					SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0xFF, 0xFF);
					imediato(&renderer,point);
					SDL_SetRenderDrawColor(renderer, 0x0, 0xFF, 0x0, 0xFF);
					incrementalBasico(&renderer,point);
					SDL_SetRenderDrawColor(renderer, 0xFF, 0x0, 0x0, 0xFF);
					bresenham(&renderer,point);
				break;
				default:
				break;
			}

			SDL_RenderPresent(renderer);
		}
	}
	
	TTF_CloseFont(font);
	TTF_Quit();	
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	return 0;
}
int iniciar(SDL_Window **window, SDL_Renderer **renderer)
{
	int status = 1;
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		status = 0;
	}
	else {
		// Create an application window with the following settings:
		*window = SDL_CreateWindow(
			"Segmentação Computação Gráfica",										// window title
			SDL_WINDOWPOS_UNDEFINED,		// initial x position
			SDL_WINDOWPOS_UNDEFINED,		// initial y position
			ScreenRect.w,								// width, in pixels
			ScreenRect.h,								// height, in pixels
			SDL_WINDOW_SHOWN						// flags - see below
		);
	
		if (*window == NULL) {
			SDL_Log("Window could not be created! SDL Error: %s\n", SDL_GetError());
			status = 0;
		}
		else {
			*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
			if (*renderer == NULL) {
				SDL_Log( "Renderer could not be created! SDL Error: %s\n", SDL_GetError());
				status = 0;
			}
			else { 	
				SDL_RenderSetLogicalSize(*renderer, 800, 480);
				SDL_SetRenderDrawColor(*renderer, 0xFF, 0xFF, 0xFF, 0xFF);
			}
		}
	}
	return status;
}
int renderFont(SDL_Texture **texture, SDL_Renderer **renderer, TTF_Font *font, char *text, SDL_Rect *pos)
{
	SDL_Surface *sText = NULL;
	SDL_Color textColor = {0,0,0,255};
	
	sText = TTF_RenderUTF8_Blended(font, text, textColor);
    
	if (sText == NULL )
		return 1;
		
	pos->w = sText->w;
	pos->h = sText->h;

	*texture = SDL_CreateTextureFromSurface(*renderer,sText);
	SDL_FreeSurface(sText);
	
	return 1;
}
void removerCaractere(char *text)
{
	char *c = text;
	while (*c != '\0')
		++c;
	*(c - 1) = '\0';
}
int offset(int y)
{
	return 480 - y;
}
void desenhaPonto(SDL_Renderer **renderer, int x, int y)
{
	SDL_RenderDrawPoint(*renderer, x, offset(y));
}
int imediato(SDL_Renderer **renderer, SDL_Point *point)
{
	double a, b, c;
	int x = point[0].x, y = point[0].y;
	
	a = (double)(point[1].y - point[0].y) / (double)(point[1].x - point[0].x);
	b = (double) point[1].y - (a * point[1].x);
	
	while (x <= point[1].x) {
		x = x + 1;
		c = (double) x * a;
		c = (double) c + b;
		y = round(c);
		desenhaPonto(renderer, x, y);
	}
	
	return 0;
}
int incrementalBasico(SDL_Renderer **renderer, SDL_Point *point)
{
	double a, t;
	int x = point[0].x, y = point[0].y;
	
	a = (double)(point[1].y - point[0].y) / (double)(point[1].x - point[0].x);
	
	while (x <= point[1].x) {
		x = x + 1;
		t = (double) y + a;
		y = round(t);
		desenhaPonto(renderer, x, y);
	}
	
	return 0;
}
int bresenham(SDL_Renderer **renderer, SDL_Point *point)
{
	int dx, dy, d, incE, incNE, x, y;
	dx = point[1].x - point[0].x;
	dy = point[1].y - point[0].y;
	d = (2 * dy) - dx;
	incE = (2 * dy);
	incNE = (2 * (dy - dx));
	x = point[0].x;
	y = point[0].y;
	
	while (x <= point[1].x) {
		if (d <= 0)
			d += incE;
		else {
			d += incNE;
			y++;
		}
		x++;
		desenhaPonto(renderer, x, y);
	}
	
	return 0;
}
