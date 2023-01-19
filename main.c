#include "general.h"

void	update_layout(s_game *game_inf, int u_flag){
	int	x[2];
	int	y[2];
	x[0] = game_inf->racket[0].posX;
	x[1] = game_inf->racket[1].posX;
	y[0] = game_inf->racket[0].posY[1];
	y[1] = game_inf->racket[1].posY[1];
	for (int i = 0; i < 2; i++)
	{
		game_inf->Co.layout[y[i] - 1][x[i]] = ('|' * u_flag) + (' ' * !u_flag);
		game_inf->Co.layout[y[i]][x[i]] = ('|' * u_flag) + (' ' * !u_flag);
		game_inf->Co.layout[y[i] + 1][x[i]] = ('|' * u_flag) + (' ' * !u_flag);
	}
}

void	init_obj(s_game *game_inf, int init_rackets, int init_ball){
	if (init_ball){
		game_inf->ball.posY = MINR / 2;
		game_inf->ball.posX = MINC / 2;
		game_inf->Co.layout[MINR / 2][ MINC / 2] = '0';
	}
	game_inf->Co.layout[game_inf->ball.posY][game_inf->ball.posX] = '0';
	update_layout(game_inf, 1);
}

void	init_layout(s_game *game_inf, int init_rackets, int init_ball){
	int	i, j;

	for (i = 0; i < MINR; i++)
	{
		j = 0;
		while(j < MINC)
		{
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
	game_inf->ball.counter_force = 250;
	game_inf->ball.x_step = 1;
	game_inf->ball.y_step = 1;
	for (int i = 0; i < 2; i++)
	{
		game_inf->racket[i].posY[0] = (MINR / 2) - 1;
		game_inf->racket[i].posY[1] = (MINR / 2);
		game_inf->racket[i].posY[2] = (MINR / 2) + 1;
	}
	game_inf->racket[1].posX = 1;
	game_inf->racket[0].posX = MINC - 2;
	game_inf->racket[0].score = 0;
	game_inf->racket[1].score = 0;
	init_layout(game_inf, 1, 1);
}

void	draw_layout(s_game *game_inf){
	int	i = 0;
	int	TmpStart = game_inf->Co.startY;

	clear();
	_player = !_player;
	update_layout(game_inf, 1);
	_player = !_player;
	move(TmpStart - 1, game_inf->Co.startX + ((MINC - 2) / 2));
	printw("(%d - %d)\n", game_inf->racket[0].score, game_inf->racket[1].score);
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
	if (game_inf->Co.layout[game_inf->ball.posY][game_inf->ball.posX] == '|')
		game_inf->ball.x_step  *= -1;
	if (tmpY < 1 || tmpY > MINR - 2)
		game_inf->ball.y_step *= -1;
	if (tmpX < 0 || tmpX > MINC - 1)
	{
		if (tmpX < 1)
			game_inf->racket[1].score += 1;
		else
			game_inf->racket[0].score += 1;
		init_obj(game_inf, 1, 1);	
	}
}

void	move_ball(s_game *game_inf){
	static int	frames;
	int	x, y;

	if (frames != game_inf->ball.counter_force){
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

void	move_them(int y, int x, s_game *game_inf, int player){
	int	next_x = game_inf->racket[player].posX + x;
	int	next_y = game_inf->racket[player].posY[1] + y;
	if ((next_x > MINC / 2 && player) || (next_x < MINC / 2 && !player) || next_x == 0 || next_x == MINC - 1)
		return ;
	else if (next_y - 1 == 0 || next_y + 1 == MINR - 1)
		return ;
	_player = player;
	update_layout(game_inf, 0);
	game_inf->racket[player].posX += x;
	game_inf->racket[player].posY[0] += y;
	game_inf->racket[player].posY[1] += y;
	game_inf->racket[player].posY[2] += y;
	update_layout(game_inf, 1);
}

void	key_press(s_game *game_inf, wchar_t c){
	if (c == 'w')
		move_them(-1, 0, game_inf, 1);
	else if (c == 's')
		move_them(1, 0, game_inf, 1);
	else if (c == 'a')
		move_them(0, -1, game_inf, 1);
	else if (c == 'd')
		move_them(0, 1, game_inf, 1);
	else if (c == KEY_UP)
		move_them(-1, 0, game_inf, 0);
	else if (c == KEY_DOWN)
		move_them(1, 0, game_inf, 0);
	else if (c == KEY_LEFT)
		move_them(0, -1, game_inf, 0);
	else if (c == KEY_RIGHT)
		move_them(0, 1, game_inf, 0);
	else if (c == 'q')
		exit (0);
}

void	move_rackets(s_game *game_inf){
	wchar_t	c;
	c = getch();
	key_press(game_inf, c);
}

int	main(){
	s_game	game_inf;

	initscr();
	cbreak();
	nodelay(stdscr, 1);
	keypad(stdscr, true);
	_player = 0;
	init_game(&game_inf);
	draw_layout(&game_inf);
	while (1)
	{
		move_ball(&game_inf);
		draw_layout(&game_inf);
		move_rackets(&game_inf);
	}
}