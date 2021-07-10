from ctypes import cdll

nmap = {0: 'U', 1: 'R', 2: 'D', 3: 'L'}
fmap = dict([val, key] for key, val in nmap.items())

C2048 = cdll.LoadLibrary("./2048.so")
C2048.random_init(44444)


class Ai:
    def __init__(self, grid_size=4):
        C2048.ai_init(grid_size)

    def get_next(self):
        return chr(C2048.get_next())


class Game:
    def __init__(self, grid_size=4):
        self.grid_size = grid_size
        C2048.game_init(grid_size)

    # 开始或重新开始
    def start(self):
        C2048.game_start()

    # 运行一步
    def run(self, direction):
        if isinstance(direction, int):
            direction = nmap[direction]
        C2048.game_run(ord(direction))

    def get_state(self):
        return ["start", "run", "over"][C2048.game_get_state()]

    def get_score(self):
        return C2048.game_get_score()

    def get_titles(self, x, y):
        return C2048.game_get_titles(x, y)
