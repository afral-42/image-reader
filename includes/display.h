#ifndef DISPLAY_H
# define DISPLAY_H

# include <SDL.h>
# include <string.h>

typedef struct  s_sdl_ctx
{
	int				running;
	SDL_Event		event;
    SDL_Window	    *window;
	SDL_Renderer    *renderer;
    SDL_Texture		*texture;

}	t_sdl_ctx;

#endif