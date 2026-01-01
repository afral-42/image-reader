#ifndef PARSING_H
# define PARSING_H

# include <fcntl.h>
# include <stdint.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/mman.h>
# include <sys/stat.h>
# include <sys/types.h>

# define WIDTH_MAX 10000
# define HEIGHT_MAX 10000

#pragma pack(push, 1)

typedef struct	s_pixel
{
	uint8_t	r;
	uint8_t	g;
	uint8_t	b;
}	t_pixel;

typedef struct	s_bmp_file_header
{
	uint16_t	signature;
	uint32_t	file_size;
	uint16_t	reserved1;
	uint16_t	reserved2;
	uint32_t	data_offset;
}				t_bmp_file_header;

typedef struct	s_bmp_info_header
{
	uint32_t	header_size;
	int32_t		width;
	int32_t		height;
	uint16_t	planes;
	uint16_t	bpp;
	uint32_t	compression;
	uint32_t	image_size;
	int32_t  	x_ppm;
    int32_t  	y_ppm;
    uint32_t 	colors_used;
    uint32_t 	colors_important;
}				t_bmp_info_header;

#pragma pack(pop)

typedef struct	s_bmp_img
{
	int					fd;
	void				*data;
	t_pixel				*pixels;
	uint8_t				*raw_pixels;
	struct stat			stats;
	t_bmp_file_header	*file_header;
	t_bmp_info_header	*info_header;
}	t_bmp_img;

int	exit_parsing(char *error, t_bmp_img *img);
int	parse_bmp_file(t_bmp_img *img);
int	load_bmp_file(char *filename, t_bmp_img *img);
int	parse_pixels(t_bmp_img *img);

#endif