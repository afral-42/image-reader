#include "parsing.h"
#include <errno.h>
#include <stdio.h>

int	exit_parsing(char *error, t_bmp_img *img)
{
	if (error)
		perror(error);
	if (img->fd && img->fd != -1)
		close(img->fd);
	if (img->data && img->data != MAP_FAILED && img->stats.st_size > 0)
		munmap(img->data, img->stats.st_size);
	if (img->pixels)
		free(img->pixels);
	return (-1);
}

int	parse_bmp_file(t_bmp_img *img)
{
	img->data = mmap(NULL, img->stats.st_size, PROT_READ, MAP_PRIVATE, img->fd, 0);
	if (img->data == MAP_FAILED)
		return (exit_parsing("Mmap error", img));
	img->file_header = (t_bmp_file_header *)img->data;
	img->info_header = (t_bmp_info_header *)(((uint8_t *)img->data) + 14);
	// Checker la validité du fichier
	img->raw_pixels = ((uint8_t *)img->data + img->file_header->data_offset);
	return (0);
}

int	load_bmp_file(char *filename, t_bmp_img *img)
{
	img->fd = open(filename, O_RDONLY);
	if (img->fd == -1)
		return (exit_parsing("Error opening the file", img));
	if (fstat(img->fd, &(img->stats)) == -1)
		return (exit_parsing("Fstat error", img));
	return (0);
}

int	parse_pixels(t_bmp_img *img)
{
	int	x; // Coordonnée x dans le fichier 
	int	y; // Coordonnée y dans le fichier
	int	i; // Index dans notre tableau sérialisé
	int	padding; // Padding d'alignement mémoire du format BMP (4 octets) 
	int	row_size; // Taille total d'une ligne (width + padding d'alignement)

	img->pixels = malloc(sizeof(t_pixel) * img->info_header->width * img->info_header->height);
	if (!(img->pixels))
		return (exit_parsing("Pixels array allocation error", img));

	// On calcule le padding en octet (combien d'octets sont rajoutés pour que la width soit un multipe de 4)
	// Width = nombre de pixels ; 1 pixel = 3 octets (BGR)
	// Width % 4, ce qui reste quand on divise par 4, il faudra y rajouter le padding pour arriver au prochain multiple de 4 donc 4 - reste
	padding = (4 - (img->info_header->width * 3) % 4) % 4; // On prend le modulo à 4 pour écarter le cas parfait qui donne 4 - 0 = 4 (pas besoin de rajouter dans ce cas)

	// On peut maintenant ajouter ce padding à la width pour obtenir la taille d'une ligne avec alignement mémoire
	row_size = img->info_header->width * 3 + padding;

	// Initialisation des index
	i = 0;
	y = img->info_header->height - 1; // On va commencer au début de la dernière ligne pour lire le fichier à l'envers
	// Nous allons parcourir le tableau 1d en deux boucles pour éviter de recalculer x et y
	while (y >= 0)
	{
		// On commence par lire au début de la ligne à chaque fois (width - 1 étant le dernier pixel)
		x = 0;
		while (x < img->info_header->width * 3) // Nous allons parcourir octet par octet pour remplir couleur par couleur
		{
			(img->pixels[i]).b = (img->raw_pixels)[y * row_size + x++]; // On se rend au début de la ligne et on ajoute x pour obtenir le point
			(img->pixels[i]).g = (img->raw_pixels)[y * row_size + x++]; // On extrait couleur par couleur le pixel dans notre tableau sérialisé t_pixel *
			(img->pixels[i++]).r = (img->raw_pixels)[y * row_size + x++];
		}
		y--; // On passe à la ligne d'au dessus
	}
	return (0);
}