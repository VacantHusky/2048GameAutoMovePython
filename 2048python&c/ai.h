#ifndef AI_H
#define AI_H
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <time.h>
// #include "grid.h"

char* ACTIONS = "ULRD";
int SIZE;
int LENGTH;
Grid* tem_g = NULL;
int _4_pows[] = {1, 4, 16, 64, 256, 1024, 4096, 16384};

typedef struct _Deci {
    char action_1;
    char action_2;
    char action_3;
    float score;
} Deci;
typedef struct _AI {
    Grid* g;
} AI;

AI ai;

void ai_init(int size) {
    SIZE = size;
    LENGTH = size * size;
    ai.g = NewGrid(size);
    tem_g = NewGrid(SIZE);
}

int min(int a, int b) {
    return a > b ? b : a;
}
int max(int a, int b) {
    return a > b ? a : b;
}
int my_log2(int z) {
    return z;
}

int* get_grid(int* tiles, char** cc, int c_len) {
    memcpy(tem_g->tiles, tiles, sizeof(int) * LENGTH);
    for (; c_len; c_len--, cc++) {
        tem_g->run(tem_g, **cc, 0);
        tem_g->add_random_tile(tem_g);
    }
    return tem_g->tiles;
}
// 空格子数量
int get_tile_num(int* tiles) {
    int n = 0, *p = tiles;
    for (int i = LENGTH; i; i--, p++)
        if (*p == 0)
            n++;
    return n;
}

int get_bj2__4(int* tiles) {
    int bj = 0, *z;
    for (int y = SIZE - 1; y; y--)
        for (int x = SIZE - 1; x; x--) {
            z = tiles + y * SIZE + x;
            if (*z < *(z - 1))
                bj -= abs(my_log2(*(z - 1)) - *z);
            if (*z < *(z - SIZE))
                bj -= abs(my_log2(*(z - SIZE)) - *z);
            if (*z < *(z - SIZE - 1))
                bj -= abs(my_log2(*(z - SIZE - 1)) - *z);
        }
    return bj;
}

int get_bj__4(int* tiles) {
    int bj = 0, size = SIZE - 1, x, y, *z = tiles;
    for (y = 0; y < SIZE; y++)
        for (x = 0; x < SIZE; x++, z++) {
            if (*z)
                bj += (*z - 2) * (x + y - (size * 2 - 1));
            else
                bj += 100 - 20 * (x + y - (size * 2 - 1));
        }
    return bj;
}

float get_score(int* tiles) {
    // 格子数量(越少越好)  金角银边（）
    int a = get_bj2__4(tiles);
    int b = get_bj__4(tiles);
    return a * 9.8 + b;
}

int combat(const void* a, const void* b) {
    return ((Deci*)a)->score - ((Deci*)b)->score;
}

char get_next() {
    int* tiles = game.grid->tiles;
    int tn = get_tile_num(tiles), i, j, fuck_step, fuck_step_2;
    float tem, min_fen;
    switch ((int)(((float)tn / ai.g->length) * 10)) {
        // case 5:
        //     fuck_step = 2;
        //     break;
        // case 4:
        //     fuck_step = 3;
        //     break;
        case 3:
            fuck_step = 3;
            break;
        case 2:
            fuck_step = 3;
            break;
        case 1:
            fuck_step = 3;
            break;
        case 0:
            fuck_step = 3;
            break;
        default:
            return rand() % 2 == 0 ? 'R' : 'D';
    }
    // tn = fuck_step * 8;
    tn = min(max(tn * tn, 20), 40);
    // tn = 25;
    fuck_step_2 = _4_pows[fuck_step];
    char *cc[fuck_step], **cp;
    Deci score_list[fuck_step_2];
    Deci* ps = score_list;  // ULRD
    for (cp = cc + fuck_step; cp != cc; cp--, *cp = ACTIONS);
    i = fuck_step_2;
    do {
        // printf("%d, %c,%c,%c,%c\n", i, **cc, **(cc + 1), **(cc + 2), **(cc + 3));
        min_fen = get_score(get_grid(tiles, cc, fuck_step));
        for (j = tn - 1; j; j--) {
            tem = get_score(get_grid(tiles, cc, fuck_step));
            if (tem < min_fen)
                min_fen = tem;
        }
        ps->action_1 = **cc;
        ps->score = min_fen;
        ps++;

        cp = cc + fuck_step;
        do {
            if (**(--cp) == 'D') {
                (*cp) -= 3;
            } else {
                (*cp)++;
                break;
            }
        } while (cp != cc);
        i--;
    } while (i);

    qsort(score_list, fuck_step_2, sizeof(Deci), combat);
    ps = score_list + fuck_step_2 - 1;
    for (i = fuck_step_2; i; i--, ps--) {
        memcpy(ai.g->tiles, tiles, sizeof(int) * LENGTH);
        if (ai.g->run(ai.g, ps->action_1, 0) != 0) {
            // ps->score /= tn;
            return ps->action_1;
        }
    }
    ps = score_list + fuck_step_2 - 1;
    // ps->score /= tn;
    return ps->action_1;
}

#endif