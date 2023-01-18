#ifndef GENERAL_H
# define GENERAL_H

# include <curses.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <math.h>

# define MINC 80
# define MINR (MINC / 5)

bool	player;

typedef	struct console
{
	int	row;
	int	col;
	int	startX;
	int	startY;
	char	layout[MINR + 1][MINC + 1];
} s_console;

typedef	struct ball
{
	int	color;
	int	posX;
	int	posY;
	int	speed;
	int	x_step;
	int	y_step;
} s_ball;

typedef	struct racket
{
	int		hight;
	int		color;
	int		posX;
	int		posY[3];
} s_racket;

typedef	struct game
{
	s_console	Co;
	s_ball		ball;
	s_racket	racket[2];
} s_game;


#endif