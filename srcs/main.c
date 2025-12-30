#include "parsing.h"
#include "display.h"

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
	display_loop(display);
	exit_sdl(NULL, &display, &img);
	return (0);
}
