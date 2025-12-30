#ifndef DISPLAY_H
# define DISPLAY_H

# include <SDL.h>
# include <string.h>
# include "parsing.h"

typedef struct  s_sdl_ctx
{
	int				running;
	SDL_Event		event;
    SDL_Window	    *window;
	SDL_Renderer    *renderer;
    SDL_Texture		*texture;

}	t_sdl_ctx;

int		exit_sdl(char *error, t_sdl_ctx *display, t_bmp_img *img);
int		init_sdl(t_sdl_ctx *display, t_bmp_img *img);
int		fill_texture(t_sdl_ctx *display, t_bmp_img *img);
void	display_loop(t_sdl_ctx display);

#endif