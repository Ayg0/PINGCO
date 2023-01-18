#include "general.h"

void	update_layout(s_game *game_inf, int u_flag){
	int	x = game_inf->racket[player].posX;
	int	y = game_inf->racket[player].posY[1];
	game_inf->Co.layout[y - 1][x] = ('|' * u_flag) + (' ' * !u_flag);
	game_inf->Co.layout[y][x] = ('|' * u_flag) + (' ' * !u_flag);
	game_inf->Co.layout[y + 1][x] = ('|' * u_flag) + (' ' * !u_flag);
}

void	init_obj(s_game *game_inf, int init_rackets, int init_ball){
	if (init_ball){
		game_inf->ball.posY = MINR / 2;
		game_inf->ball.posX = MINC / 2;
		game_inf->Co.layout[MINR / 2][ MINC / 2] = '0';
	}
	update_layout(game_inf, 1);
}

void	init_layout(s_game *game_inf, int init_rackets, int init_ball){
	int	i, j;

	for (i = 0; i < MINR; i++)
	{
		j = 0;
		while(j < MINC)
		{
			if (init_rackets && init_ball)
				game_inf->Co.layout[i][j] = ' ';
			if (!i || !j || j == (MINC - 1) || i == MINR - 1)
				game_inf->Co.layout[i][j] = '#';
			j++;
		}
		game_inf->Co.layout[i][j] = '\0';
	}
	init_obj(game_inf, init_rackets, init_ball);
}

void	init_game(s_game *game_inf){
    getmaxyx(stdscr, game_inf->Co.row, game_inf->Co.col);
	if (game_inf->Co.row < MINR || game_inf->Co.col < MINC)
		exit(write(2, "Error, please use a console of MINR by MINR or more.", 48));
	game_inf->Co.startX = (game_inf->Co.col - MINC) / 2;
	game_inf->Co.startY = (game_inf->Co.row - MINR) / 2;
	game_inf->ball.speed = 120;
	game_inf->ball.x_step = 1;
	game_inf->ball.y_step = 1;
	game_inf->racket[1].posY[0] = (MINR / 2) - 1;
	game_inf->racket[1].posY[1] = (MINR / 2);
	game_inf->racket[1].posY[2] = (MINR / 2) + 1;
	game_inf->racket[1].posX = 1;
	game_inf->racket[0].posY[0] = (MINR / 2) - 1;
	game_inf->racket[0].posY[1] = (MINR / 2);
	game_inf->racket[0].posY[2] = (MINR / 2) + 1;
	game_inf->racket[0].posX = MINC - 2;
	init_layout(game_inf, 1, 1);
}

void	draw_layout(s_game *game_inf){
	int	i = 0;
	int	TmpStart = game_inf->Co.startY;

	clear();
	while (i < MINR){
		init_layout(game_inf, 1, 0);
		move(TmpStart, game_inf->Co.startX);
		printw("%s\n", game_inf->Co.layout[i]);
		TmpStart++;
		i++;
	}
	refresh();
}

void	check_for_coll(s_game *game_inf){
	int tmpX = game_inf->ball.posX + game_inf->ball.x_step;
	int tmpY = game_inf->ball.posY + game_inf->ball.y_step;
	if (game_inf->Co.layout[game_inf->ball.posY][game_inf->ball.posX] == '|' || tmpX < 1 || tmpX > MINC - 2)
		game_inf->ball.x_step  *= -1;
	if (game_inf->Co.layout[game_inf->ball.posY][game_inf->ball.posX] == '|' || tmpY < 1 || tmpY > MINR - 2)
		game_inf->ball.y_step *= -1;
}

void	move_ball(s_game *game_inf){
	static int	frames;
	int	x, y;

	if (frames != game_inf->ball.speed){
		frames++;
		return ;
	}
	frames = 0;
	check_for_coll(game_inf);
	game_inf->Co.layout[game_inf->ball.posY][game_inf->ball.posX] = ' ';
	game_inf->ball.posX += game_inf->ball.x_step;
	game_inf->ball.posY += game_inf->ball.y_step;
	game_inf->Co.layout[game_inf->ball.posY][game_inf->ball.posX] = '0';
}

void	move_them(int y, int x, s_racket *racket, s_game *game_inf){
	int	next_x = racket->posX + x;
	int	next_y = racket->posY[1] + y;
	if ((next_x > MINC / 2 && player) || (next_x < MINC / 2 && !player) || next_x == 0 || next_x == MINC - 1)
		return ;
	else if (next_y - 1 == 0 || next_y + 1 == MINR - 1)
		return ;
	update_layout(game_inf, 0);
	racket->posX += x;
	racket->posY[0] += y;
	racket->posY[1] += y;
	racket->posY[2] += y;
	update_layout(game_inf, 1);
}

void	key_press(s_game *game_inf, char c){
	if (c == 'w')
		move_them(-1, 0, &game_inf->racket[player], game_inf);
	else if (c == 's')
		move_them(1, 0, &game_inf->racket[player], game_inf);
	else if (c == 'a')
		move_them(0, -1, &game_inf->racket[player], game_inf);
	else if (c == 'd')
		move_them(0, 1, &game_inf->racket[player], game_inf);
	else if (c == 'p')
		player = !player;
}

void	move_rackets(s_game *game_inf){
	char	c;
	c = getch();
	key_press(game_inf, c);
}

int	main(){
	s_game	game_inf;

	initscr();
	cbreak();
	nodelay(stdscr, 1);
	player = 0;
	/*
		|X| - get terminal info
		|X| - draw the layout
		|X| - move the ball
		|X| - accept input
	*/
	init_game(&game_inf);
	draw_layout(&game_inf);
	while (1)
	{
		//usleep(50);
		move_ball(&game_inf);
		draw_layout(&game_inf);
		move_rackets(&game_inf);
	}
}