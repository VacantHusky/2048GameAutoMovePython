try:
    import pygame, os, time
except:
    print('cmd run: pip3 install pygame -i https://mirrors.aliyun.com/pypi/simple')
    exit()
from pygame.locals import *
from game import Game, Ai
from config import *

config = SupperFast()

FPS = config.FPS
SIZE = config.SIZE
DEBUG = config.DEBUG
colors = config.COLORS
GAME_WH = config.GAME_WH
WINDOW_W = config.WINDOW_W
WINDOW_H = config.WINDOW_H

# 格子中的字体
font_h_w = 2 / 1
g_w = GAME_WH / SIZE * 0.9

class Main():
    def __init__(self):
        global FPS
        pygame.init()
        os.environ['SDL_VIDEO_WINDOW_POS'] = "%d,%d" % (100, 50)
        self.set_win_wh(WINDOW_W, WINDOW_H, title='2048')
        self.state = 'start'
        self.fps = FPS
        self.catch_n = 0
        self.clock = pygame.time.Clock()
        self.game = Game(SIZE)
        self.ai = Ai(SIZE)
        self.step_time = config.STEP_TIME
        self.next_f = ''
        self.last_time = time.time()
        self.jm = -1

    def start(self):
        # 加载按钮
        self.button_list = [
            Button('start', '重新开始', (GAME_WH + 50, 150)),
            Button('ai', '电脑托管', (GAME_WH + 50, 250)),
        ]
        self.run()

    def run(self):
        while self.state != 'exit':
            if self.game.get_state() in ['over', 'win']:
                self.state = self.game.get_state()
            self.my_event()
            if self.next_f != '' and (
                    self.state == 'run' or self.state == 'ai' and time.time() - self.last_time > self.step_time):
                self.game.run(self.next_f)
                self.next_f = ''
                self.last_time = time.time()
            elif self.state == 'start':
                self.game.start()
                self.state = 'run'
            self.set_bg((187, 173, 160))
            self.draw_info()
            self.draw_button(self.button_list)
            self.draw_map()
            self.update()
        print('退出游戏')

    def draw_map(self):
        for y in range(SIZE):
            for x in range(SIZE):
                self.draw_block((x, y), self.game.get_titles(x, y))
        if self.state == 'over':
            pygame.draw.rect(self.screen, (0, 0, 0, 0.5),
                             (0, 0, GAME_WH, GAME_WH))
            self.draw_text('游戏结束！', (GAME_WH / 2, GAME_WH / 2), size=25, center='center')
        elif self.state == 'win':
            pygame.draw.rect(self.screen, (0, 0, 0, 0.5),
                             (0, 0, GAME_WH, GAME_WH))
            self.draw_text('胜利！', (GAME_WH / 2, GAME_WH / 2), size=25, center='center')

    # 画一个方格
    def draw_block(self, xy, number):
        one_size = GAME_WH / SIZE
        dx = one_size * 0.05
        x, y = xy[0] * one_size, xy[1] * one_size
        color = colors.get(str(int(number)), (0, 0, 255))
        pygame.draw.rect(self.screen, color,
                         (x + dx, y + dx, one_size - 2 * dx, one_size - 2 * dx))
        color = (20, 20, 20) if number <= 4 else (250, 250, 250)
        if number != 0:
            ln = len(str(number))
            if ln == 1:
                size = one_size * 1.2 / 2
            elif ln <= 3:
                size = one_size * 1.2 / ln
            else:
                size = one_size * 1.5 / ln

            self.draw_text(str(int(number)), (x + one_size * 0.5, y + one_size * 0.5 - size / 2), color, size, 'center')

    def draw_info(self):
        self.draw_text('分数：{}'.format(self.game.get_score()), (GAME_WH + 50, 40))
        if self.state == 'ai':
            self.draw_text('间隔：{}'.format(self.step_time), (GAME_WH + 50, 60))
            self.draw_text('评分：{}'.format(self.jm), (GAME_WH + 50, 80))

    def set_bg(self, color=(255, 255, 255)):
        self.screen.fill(color)

    def catch(self, filename=None):
        if filename is None:
            filename = "./catch/catch-{:04d}.png".format(self.catch_n)
        pygame.image.save(self.screen, filename)
        self.catch_n += 1

    def draw_button(self, buttons):
        for b in buttons:
            if b.is_show:
                pygame.draw.rect(self.screen, (180, 180, 200),
                                 (b.x, b.y, b.w, b.h))
                self.draw_text(b.text, (b.x + b.w / 2, b.y + 9), size=18, center='center')

    def draw_text(self, text, xy, color=(0, 0, 0), size=18, center=None):
        font = pygame.font.SysFont('simhei', round(size))
        text_obj = font.render(text, 1, color)
        text_rect = text_obj.get_rect()
        if center == 'center':
            text_rect.move_ip(xy[0] - text_rect.w // 2, xy[1])
        else:
            text_rect.move_ip(xy[0], xy[1])
        # print('画文字：',text,text_rect)
        self.screen.blit(text_obj, text_rect)

    # 设置窗口大小
    def set_win_wh(self, w, h, title='python游戏'):
        self.screen2 = pygame.display.set_mode((w, h), pygame.DOUBLEBUF, 32)
        self.screen = self.screen2.convert_alpha()
        pygame.display.set_caption(title)

    def update(self):
        self.screen2.blit(self.screen, (0, 0))
        # 刷新画面
        # pygame.display.update()
        pygame.display.flip()
        time_passed = self.clock.tick(self.fps)

    # 侦听事件
    def my_event(self):
        if self.state == 'ai' and self.next_f == '':
            # self.next_f, self.jm = self.ai.get_next(self.game.grid.tiles)
            self.next_f = self.ai.get_next()
        for event in pygame.event.get():
            if event.type == QUIT:
                self.state = 'exit'
            if event.type == KEYDOWN:
                if event.key == K_ESCAPE:
                    self.state = 'exit'
                elif event.key in [K_LEFT, K_a] and self.state == 'run':
                    self.next_f = 'L'
                elif event.key in [K_RIGHT, K_d] and self.state == 'run':
                    self.next_f = 'R'
                elif event.key in [K_DOWN, K_s] and self.state == 'run':
                    self.next_f = 'D'
                elif event.key in [K_UP, K_w] and self.state == 'run':
                    self.next_f = 'U'
                elif event.key in [K_k, K_l] and self.state == 'ai':
                    if event.key == K_k and self.step_time > 0:
                        self.step_time *= 0.9
                    if event.key == K_l and self.step_time < 10:
                        if self.step_time != 0:
                            self.step_time *= 1.1
                        else:
                            self.step_time = 0.01
                    if self.step_time < 0:
                        self.step_time = 0

            if event.type == MOUSEBUTTONDOWN:
                for i in self.button_list:
                    if i.is_click(event.pos):
                        self.state = i.name
                        if i.name == 'ai':
                            i.name = 'run'
                            i.text = '取消托管'
                        elif i.name == 'run':
                            i.name = 'ai'
                            i.text = '电脑托管'
                        break


def run():
    Main().start()


# 按钮类
class Button(pygame.sprite.Sprite):
    def __init__(self, name, text, xy, size=(100, 50)):
        pygame.sprite.Sprite.__init__(self)
        self.name = name
        self.text = text
        self.x, self.y = xy[0], xy[1]
        self.w, self.h = size
        self.is_show = True

    def is_click(self, xy):
        return (self.is_show and
                self.x <= xy[0] <= self.x + self.w and
                self.y <= xy[1] <= self.y + self.h)


if __name__ == '__main__':
    run()
