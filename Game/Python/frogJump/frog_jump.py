import math
import os
from random import randint
from collections import deque
import time

import pygame
from pygame.locals import *

FPS = 60
WIN_WIDTH = 284
WIN_HEIGHT = 512

def frames_to_ms(frames, fps = FPS):
    return 1000.0 * frames / fps

def ms_to_frames(ms, fps = FPS):
    return fps * ms / 1000.0

def image_load(path):
    img = pygame.image.load(path)
    img.convert()
    return img

class Frog(pygame.sprite.Sprite):

    WIDTH = HEIGHT = 32
    FULL_UP_TIME = 1000 # ms, the time frog takes to reach the top position in a one jump
    X_MOVE_SPEED = 20
    UP_PARAMETER = 9
    
    def __init__(self, x, y, frog_image, down_speed, up_speed, ms_to_full_up):
        super(Frog, self).__init__()
        self.x, self.y = x, y
        self.image = frog_image
        self.down_speed = down_speed
        self.up_speed = up_speed
        self.ms_to_full_up = ms_to_full_up
        self.mask = pygame.mask.from_surface(self.image)

    def update(self, valid_platforms, platforms, pf_up_speed, delta_frames = 1):
        
        # up
        if self.ms_to_full_up > 0:
            up_part = 1 - self.ms_to_full_up / Frog.FULL_UP_TIME
            up_value = self.up_speed * frames_to_ms(delta_frames) * (math.cos(up_part * math.pi))
            self.y -= up_value
                
            self.ms_to_full_up -= frames_to_ms(delta_frames)
        #down
        else:
            down_value = self.down_speed * frames_to_ms(delta_frames)
            self.y += down_value
 
    def moveup(self, valid_platforms, platforms):
        self.y -= Frog.UP_PARAMETER
        self.ms_to_full_up = Frog.FULL_UP_TIME - 10

    def moveleft(self):
        self.x -= Frog.X_MOVE_SPEED
        if self.x + Frog.WIDTH <=0:
            self.x = WIN_WIDTH - 1

    def moveright(self):
        self.x += Frog.X_MOVE_SPEED
        if self.x >= WIN_WIDTH:
            self.x = 0

    @property
    def rect(self):
        return Rect(self.x, self.y, Frog.WIDTH, Frog.HEIGHT)

    @property
    def _x(self):
        return self.x

    @property
    def _y(self):
        return self.y
    

class Platform(pygame.sprite.Sprite):

    HEIGHT = 12
    WIDTH = 64
    
    def __init__(self, platform_image, down_bound, interval, up_speed):
        self.x = randint(1, WIN_WIDTH - Platform.WIDTH)
        self.y = down_bound - interval
        #print self.x
        #print self.y
        self.counted = False
        self.image = platform_image
        self.up_speed = up_speed
        self.mask = pygame.mask.from_surface(self.image)

    @property
    def rect(self):
        return Rect(self.x, self.y, Platform.WIDTH, Platform.HEIGHT)

    @property
    def visible(self):
        return -Platform.HEIGHT < self.y < WIN_HEIGHT

    # very important, test collision
    def collide(self, frog):
        higher = (-Platform.HEIGHT) / 2 <= (frog._y + Frog.HEIGHT - self.y) <= 1
        inside = frog._x + Frog.WIDTH >= self.x and frog.x <= self.x + self.WIDTH
        return (higher and inside)

    @property
    def _x(self):
        return self.x
    
    @property
    def _y(self):
        return self.y

    def update(self, delta_frames = 1):
        self.y += self.up_speed * frames_to_ms(delta_frames)

    def set_counted(self):
        self.counted = True
        
def main():
    pygame.init()

    # hard options
    hard = [30, 100, 200]
    up_speed = [0.3, 0.5, 0.6]
    down_speed = [0.2, 0.4, 1]
    interval = [35, 100, 200]
    platform_add_gap = [1500, 1700, 500]
    pf_up_speed = [0.15, 0.24, 0.5]
    scores = [1, 3, 100]

    # hard level
    level = 2

    display_surface = pygame.display.set_mode((WIN_WIDTH, WIN_HEIGHT))
    pygame.display.set_caption('Frog Jumping')

    clock = pygame.time.Clock()
    score_font = pygame.font.SysFont(None, 32, bold = True)

    background = image_load('images/background.png')
    platform_image = image_load('images/platform.png')
    frog_image = image_load('images/frog.png')

    platforms = deque()
    valid_platforms = deque()
    

    frame_clock = 0
    score = 0

    # initial state
    pf = Platform(platform_image, WIN_HEIGHT - 100, interval[level], pf_up_speed[level])
    platforms.append(pf)
    valid_platforms.append(pf)

    frog = Frog(platforms[0]._x + Platform.WIDTH, platforms[0]._y - Frog.HEIGHT, frog_image, down_speed[level], up_speed[level], 0)
    
    display_surface.blit(background, (0, 0))
    display_surface.blit(frog.image, frog.rect)
    display_surface.blit(pf.image, pf.rect)

    # a pause, press 'space' to start the game
    while True:
        flag = False
        for event in pygame.event.get():
            if event.type ==KEYDOWN:
                if event.key == K_SPACE:
                    flag = True
                    break
        if flag:
            break
                
    done = False    
    # main loop
    while not done:
        clock.tick(FPS)

        if frog._y > WIN_HEIGHT:
            done = True
            continue

        # add platforms
        if not (frame_clock % ms_to_frames(platform_add_gap[level])):
            #print 'add a platform'
            pf = Platform(platform_image, valid_platforms[0]._y, interval[level], pf_up_speed[level])
            platforms.append(pf)
            valid_platforms.append(pf)
            valid_platforms.popleft()

        # move left or right
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
            elif event.type == KEYDOWN:
                if event.key == K_LEFT:
                    frog.moveleft()
                elif event.key == K_RIGHT:
                    frog.moveright()

        # check collisions
        for pf in platforms:
            if pf.collide(frog):
                if pf.counted == False:
                    score += scores[level]
                    pf.set_counted()
                frog.moveup(valid_platforms, platforms)
                break

        # pop unvisible platforms
        while platforms and not platforms[0].visible:
            platforms.popleft()
        
        # draw background
        display_surface.blit(background, (0, 0))

        # draw platforms
        for pf in platforms:
            pf.update(pf_up_speed[level])
            display_surface.blit(pf.image, pf.rect)

        # draw frog
        frog.update(valid_platforms, platforms, pf_up_speed[level])
        display_surface.blit(frog.image, frog.rect)
        
        # draw score
        score_surface = score_font.render(str(score), True, (255, 255, 255))
        score_x = WIN_WIDTH / 2 - score_surface.get_width() / 2
        score_y = score_surface.get_height() + 20
        display_surface.blit(score_surface, (score_x, score_y))

        pygame.display.flip()
        frame_clock += 1

    print('Game Over! Your score is %i' % score)
    pygame.quit()

if __name__ == '__main__':
    main()
