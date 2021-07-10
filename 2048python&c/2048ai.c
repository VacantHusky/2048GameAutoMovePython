#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "grid.h"
#include "game.h"
#include "ai.h"

int random_init(int a) {
    srand((unsigned)time(NULL));
}

int ppow(int b) {
    int r = 1;
    for (; b; b--) {
        r *= 2;
    }
    return r > 1000 ? 1024 : r;
}

// int main(void) {
//     char *aa = {"LRD"};
//     int a = 3, fuck = 27, i;
//     char *cc[a], **cp;
//     for(cp = cc + 3; cp != cc; cp--, *cp = aa);
//     i = 27;
//     do {
//         printf("%d, %c,%c,%c\n\n", i,  **cc, **(cc + 1), **(cc + 2));
//         cp = cc + a;
//         do {
//             if (**(--cp) == 'D') {
//                 (*cp) -= 2;
//             } else {
//                 (*cp)++;
//                 break;
//             }
//         }while (cp != cc);
//         i--;
//     } while (i);
// }

int main() {
    game_init(4);
    game_start();
    for (int i = 0; i < game.grid->length; i++) {
        game.grid->tiles[i] = i > 4 ? ppow(i) : 0;
    }
    ai_init(4);
    game.grid->print(game.grid);

    char a = get_next();
    printf("运行： %c\n", a);
    game_run(a);
    game.grid->print(game.grid);

    a = get_next();
    printf("%c\n", a);
    game_run(a);
    game.grid->print(game.grid);

    a = get_next();
    printf("%c\n", a);
    game_run(a);
    game.grid->print(game.grid);
}