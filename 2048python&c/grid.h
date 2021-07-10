#ifndef GRID_H
#define GRID_H

// 二维向量
typedef struct _Vector {
    int x;
    int y;
} Vector;

typedef struct _Grid {
    int* tiles;
    int size;
    int length;
    int score;
    int (*is_zero)(struct _Grid* self, int x, int y);
    int (*is_full)(struct _Grid* self);
    void (*set_tiles)(struct _Grid* self, Vector xy, int number);
    Vector (*get_random_xy)(struct _Grid* self);
    void (*add_random_tile)(struct _Grid* self);
    void (*add_tile_init)(struct _Grid* self);
    void (*move_hl)(struct _Grid* self, int ii, char direction);
    int (*run)(struct _Grid* self, char direction, int is_fake);
    int (*is_over)(struct _Grid* self);
    void (*print)(struct _Grid* self);
} Grid;

int is_zero(Grid* self, int x, int y) {
    return *(self->tiles + (self->size * y + x)) == 0 ? 1 : 0;
}
int is_full(Grid* self) {
    int* p = self->tiles;
    for (int i = self->length; i; i--)
        if (*p++ == 0)
            return 0;
    return 1;
}
// 设置瓷砖
void set_tiles(Grid* self, Vector xy, int number) {
    self->tiles[self->size * xy.y + xy.x] = number;
}
// 获取一个随机的空坐标
Vector get_random_xy(Grid* self) {
    Vector xy = {-1, -1};
    if (self->is_full(self) == 0) {
        int size = self->size, x, y;
        while (1) {
            x = rand() % size, y = rand() % size;
            if (self->is_zero(self, x, y)) {
                xy.x = x;
                xy.y = y;
                break;
            }
        }
    }
    return xy;
}
// 添加一个随机的瓷砖
void add_random_tile(Grid* self) {
    if (self->is_full(self) == 0) {
        int value = (float)rand() / RAND_MAX < 0.9 ? 2 : 4;
        self->set_tiles(self, self->get_random_xy(self), value);
    }
}
// 初始设置瓷砖
void add_tile_init(Grid* self) {
    self->add_random_tile(self);
    self->add_random_tile(self);
}
// 移动某一行或某一列
// ii: 要移动的行数还是列数
// direction: 方向
void move_hl(Grid* self, int ii, char direction) {
    int *hl[self->size], i, j, fuck = self->size * ii;
    int **p = hl, **q;
    switch (direction) {
        case 'U':
            for (i = 0; i < self->size; i++, p++)
                *p = self->tiles + (i * self->size + ii);
            break;
        case 'D':
            for (i = self->size - 1; i >= 0; i--, p++)
                *p = self->tiles + (i * self->size + ii);
            break;
        case 'L':
            for (i = 0; i < self->size; i++, p++)
                *p = self->tiles + (fuck + i);
            break;
        case 'R':
            for (i = self->size - 1; i >= 0; i--, p++)
                *p = self->tiles + (fuck + i);
            break;
        default:
            printf("move_hl: fuck=%X\n", direction);
            break;
    }
    p = hl;
    for (i = 0; i < self->size - 1; i++, p++) {
        if (**p == 0)
            for (j = self->size - i - 1, q = p + 1; j > 0; j--, q++) {
                if (**q != 0) {
                    **p = **q;
                    **q = 0;
                    self->score++;
                    break;
                }
            }
        if (**p == 0)
            break;
        for (j = self->size - i - 1, q = p + 1; j > 0; j--, q++) {
            if (**q == **p) {
                **p += **q;
                self->score += **q;
                **q = 0;
                break;
            }
            if (**q != 0)
                break;
        }
    }
}
// 运行
int run(Grid* self, char direction, int is_fake) {
    if (direction != 'U' && direction != 'D' && direction != 'L' && direction != 'R')
        printf("run fuck: %x\n", direction);
    int *t, i;
    self->score = 0;
    if (is_fake) {
        t = (int*)malloc(sizeof(self->tiles));
        memcpy(t, self->tiles, sizeof(self->tiles));
    } else
        t = self->tiles;
    for (i = 0; i < self->size; i++)
        self->move_hl(self, i, direction);
    if (is_fake) free(t);
    return self->score;
}
// 判断是否结束
int is_over(Grid* self) {
    if (self->is_full(self) == 0)
        return 0;
    int x, y, ys, ys1, tem, *t = self->tiles, s = self->size;
    for (y = s - 2; y >= 0; y--) {
        ys = y * s;
        ys1 = (y + 1) * s;
        for (x = s - 2; x >= 0; x--) {
            tem = *(t + (ys + x));
            if (tem == *(t + (ys + x + 1)) || tem == *(t + (ys1 + x)))
                return 0;
        }
    }
    return 1;
}
void print(Grid* self){
    printf("====================\n");
    int x, y;
    for(y = 0; y < self->size; y ++) {
        for(x = 0; x< 5 * self->size + 1; x++)
            printf("-");
        printf("\n");
        for(x = 0; x< self->size; x++) {
            printf("|%4d", *(self->tiles + (self->size*y+x)));
        }
        printf("\n");
    }
    for(x = 0; x< 5 * self->size + 1; x++)
        printf("-");
    printf("\n");
    printf("====================\n");
}
Grid g_obj;
int g_flag = 1;

void CreateGrid() {  // 提前创建对象
    // g_obj = (Grid*)malloc(sizeof(Grid));
    // 赋值成员变量
    g_obj.score = 0;
    // 赋值成员方法
    g_obj.is_zero = is_zero;
    g_obj.is_full = is_full;
    g_obj.set_tiles = set_tiles;
    g_obj.get_random_xy = get_random_xy;
    g_obj.add_random_tile = add_random_tile;
    g_obj.add_tile_init = add_tile_init;
    g_obj.move_hl = move_hl;
    g_obj.run = run;
    g_obj.is_over = is_over;
    g_obj.print = print;
    g_flag = 0;
}

Grid* NewGrid(int size) {  // new一个对象
    Grid* g = (Grid*)malloc(sizeof(Grid));
    if (g_flag)
        CreateGrid();
    memcpy(g, &g_obj, sizeof(Grid));
    g->size = size;
    g->length = size * size;
    g->tiles = (int*)calloc(g->length, sizeof(int));
    return g;
}

#endif
