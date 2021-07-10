#ifndef GAME_H
#define GAME_H

typedef struct _Game {
    int score;
    int state;
    int grid_size;
    Grid* grid;
} Game;

Game game;

int game_get_score() {
    return game.score;
}
int game_get_state() {
    return game.state;
}
int game_get_titles(int x, int y) {
    return game.grid->tiles[y * game.grid_size + x];
}


// 开始或重新开始
void game_start(){
    free(game.grid);
    game.grid = NewGrid(game.grid_size);
    game.grid->add_tile_init(game.grid);
    game.state = 1;
}

void game_init(int size) {
    game.score = 0;
    game.state = 0;
    game.grid_size = size;
}
// 运行一步
void game_run(char direction){
    if (game.state == 2)
        return;
    game.grid->run(game.grid, direction, 0);
    game.score += game.grid->score;
    if (game.grid->is_over(game.grid))
        game.state = 2;
    // 产生新方块
    game.grid->add_random_tile(game.grid);
}


#endif