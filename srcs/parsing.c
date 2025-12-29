#include "parsing.h"
#include <errno.h>
#include <stdio.h>

int	exit_parsing(char *error, int fd, void *file, size_t filesize)
{
	if (error)
		perror(error);
	if (fd != -1)
		close(fd);
	if (file && file != MAP_FAILED && filesize > 0)
		munmap(file, filesize);
	return (-1);
}

int	parse_bmp_file(t_bmp_img *img)
{
	img->data = mmap(NULL, img->stats.st_size, PROT_READ, MAP_PRIVATE, img->fd, 0);
	if (img->data == MAP_FAILED)
		return (exit_parsing("Mmap error", img->fd, NULL, 0));
	img->file_header = (t_bmp_file_header *)img->data;
	img->info_header = (t_bmp_info_header *)(((uint8_t *)img->data) + 14);
	return (0);
}

int	load_bmp_file(t_bmp_img *img)
{
	img->fd = open("../tests/IMG_6676.bmp", O_RDONLY);
	if (img->fd == -1)
		return (exit_parsing("Error opening the file", img->fd, NULL, 0));
	if (fstat(img->fd, &(img->stats)) == -1)
		return (exit_parsing("Fstat error", img->fd, NULL, 0));
	return (0);
}

int	parse_pixels(t_bmp_img *img)
{
	
}

int	main(void)
{
	t_bmp_img	img = {0};

	if (load_bmp_file(&img) == -1)
		return (1);
	if (parse_bmp_file(&img) == -1)
		return (1);
	printf("Signature : %#X\n", img.file_header->signature);
	printf("Size : %u", (int)(img.file_header->file_size));
	exit_parsing(NULL, img.fd, img.data, img.stats.st_size);
}