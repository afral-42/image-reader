#include <SDL.h>
#include <stdio.h>

int	exit_sdl(SDL_Window *window, SDL_Renderer *renderer)
{
	if (!window || !renderer)
	{
		if (window)
			SDL_DestroyWindow(window);
		SDL_Quit();
		return (-1);
	}
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
	return (0);
}

int	init_sdl(SDL_Window **window, SDL_Renderer **renderer, int w, int h)
{
	*window = NULL;
	*renderer = NULL;
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {    
		fprintf(stderr, "Erreur SDL_Init : %s", SDL_GetError());
        return (-1);
	}
	*window = SDL_CreateWindow("IMG Reader", SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED, w, h,
									SDL_WINDOW_SHOWN);
	if (!(*window))
	{
		fprintf(stderr, "Erreur SDL_CreateWindow : %s", SDL_GetError());
		return (exit_sdl(*window, *renderer));
	}
	*renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
	if (!(*renderer))
	{
		fprintf(stderr, "Erreur SDL_CreateRenderer : %s", SDL_GetError());
		return (exit_sdl(*window, *renderer));
	}
	return (0);
}

int main(void)
{
	SDL_Window		*window;
	SDL_Renderer	*renderer;

	if (init_sdl(&window, &renderer, 600, 480) == -1)
		return (1);

	SDL_Event	event;
	int			running;

	running = 1;
	while (running)
	{
		while (SDL_PollEvent(&event))
		{
			if (event.type == SDL_QUIT)
				running = 0;
		}
	}
	return (exit_sdl(window, renderer));
}