
class Base:
    WINDOW_W = 700
    WINDOW_H = 550
    GAME_WH = 500
    SIZE = 5
    FPS = 60
    DEBUG = False
    COLORS = {
        '0': (205, 193, 180),
        '2': (238, 228, 218),
        '4': (237, 224, 200),
        '8': (242, 177, 121),
        '16': (245, 149, 99),
        '32': (246, 124, 95),
        '64': (246, 94, 59),
        '128': (237, 207, 114),
        '256': (237, 204, 97),
        '512': (237, 200, 80),
        '1024': (237, 197, 63),
        '2048': (200, 63, 63),
        '4096': (170, 30, 70),
        '8192': (150, 30, 90),
        '16384': (120, 30, 110)
    }

class SupperFast(Base):
    STEP_TIME = 0
    ANIMATION = False

class Fast(Base):
    STEP_TIME = 0.3
    ANIMATION = True

class Watch(Base):
    STEP_TIME = 0.9
    ANIMATION = True

class Development(Base):
    STEP_TIME = 1.5
    ANIMATION = True
    DEBUG = True








