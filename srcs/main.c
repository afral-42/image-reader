#include "display.h"
#include "parsing.h"

int	exit_sdl(char *error, t_sdl_ctx *display, t_bmp_img *img)
{
	if (error)
		fprintf(stderr, "%s : %s", error, SDL_GetError());
	if (display->texture)
		SDL_DestroyTexture(display->texture);
	if (display->renderer)
		SDL_DestroyRenderer(display->renderer);
	if (display->window)
		SDL_DestroyWindow(display->window);
	if (strcmp(error, "Error SDL_Init"))
		SDL_Quit();
	exit_parsing(NULL, img);
	return (-1);
}

int	init_sdl(t_sdl_ctx *display, t_bmp_img *img)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)   
		return (exit_sdl("Error SDL_Init", display, img));
	display->window = SDL_CreateWindow("IMG Reader", SDL_WINDOWPOS_CENTERED,
									SDL_WINDOWPOS_CENTERED, img->info_header->width, img->info_header->height,
									SDL_WINDOW_SHOWN);
	if (!(display->window))
		return (exit_sdl("Error SDL_CreateWindow", display, img));
	display->renderer = SDL_CreateRenderer(display->window, -1, SDL_RENDERER_ACCELERATED);
	if (!(display->renderer))
		return (exit_sdl("Error SDL_CreateRenderer", display, img));
	display->texture = SDL_CreateTexture(
		display->renderer,
		SDL_PIXELFORMAT_RGB24,
		SDL_TEXTUREACCESS_STATIC,
		img->info_header->width,
		img->info_header->height);
	if (!(display->texture))
		return (exit_sdl("Unable to create texture", display, img));
	return (0);
}

int	fill_texture(t_sdl_ctx *display, t_bmp_img *img)
{
	if (SDL_UpdateTexture(display->texture, NULL, img->pixels, (img->info_header->width) * sizeof(t_pixel)) != 0)
		return (exit_sdl("Error updating the texture", display, img));
	return (0);
}

int main(int ac, char **av)
{
	t_sdl_ctx	display = {0};
	t_bmp_img	img = {0};

	if (ac != 2)
		return (1);
	if (load_bmp_file(av[1], &img) == -1)
		return (1);
	if (parse_bmp_file(&img) == -1)
		return (1);
	if (parse_pixels(&img) == -1)
		return (-1);

	printf("Signature : %#X\n", img.file_header->signature);
	printf("Size : %u\n\n", (int)(img.file_header->file_size));

	if (init_sdl(&display, &img) == -1)
		return (1);
	if (fill_texture(&display, &img) == -1)
		return (1);
	display.running = 1;
	//SDL_RenderSetLogicalSize(display.renderer, img.info_header->width, img.info_header->height);

	while (display.running)
	{
		while (SDL_PollEvent(&(display.event)))
		{
			if (display.event.type == SDL_QUIT)
				display.running = 0;
			SDL_RenderClear(display.renderer);
			SDL_RenderCopy(display.renderer, display.texture, NULL, NULL);
			SDL_RenderPresent(display.renderer);
		}
	}
	exit_sdl(NULL, &display, &img);
}
