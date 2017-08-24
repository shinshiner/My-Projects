# -*- coding: utf-8 -*-
import pygame
from pygame.locals import *

FPS = 60
WIN_WIDTH = 1100
WIN_HEIGHT = 700

# 帧数和时间互相转换
def frames_to_ms(frames, fps = FPS):
    return 1000.0 * frames / fps

def ms_to_frames(ms, fps = FPS):
    return fps * ms / 1000.0

# 子弹类
class Bullet(object):

    WIDTH = HEIGHT = 12
    SPEED = 0.5

    '''
        x, y：   子弹坐标
        image：  子弹图片
        speed：  子弹速度
        bomb：   是否爆炸或飞出地图
        enemy：  子弹是我方的还是敌方的
        mask：   碰撞检测
    '''
    def __init__(self, x, y, image, enemy, speed = SPEED):
        self.x = x
        self.y = y
        self.image = image
        self.speed = speed
        self.bomb = False
        self.enemy = enemy
        self.mask = pygame.mask.from_surface(self.image)

    # 子弹上下移动
    def update(self, delta_frames = 1):
        if self.enemy:
            self.y += Bullet.SPEED * frames_to_ms(delta_frames)
        else:
            self.y -= Bullet.SPEED * frames_to_ms(delta_frames)
            
        if self.y < 0:
            self.bomb = True

    @property
    def _bomb(self):
        return self.bomb

    @property
    def _y(self):
        return self.y

    @property
    def _enemy(self):
        return self.enemy

    @property
    def rect(self):
        return Rect(self.x, self.y, Battery.WIDTH, Battery.HEIGHT)

# 炮台类，我方操控的对象
class Battery(pygame.sprite.Sprite):

    WIDTH = HEIGHT = 70
    BUFF = 0.02
    SPEED = 0.3

    '''
        x, y：       炮台坐标
        image：      炮台图片
        bullet_img： 子弹图片
        speed：      炮台移动速度
    '''
    def __init__(self, x, image, bullet_img, speed = SPEED):
        super(Battery, self).__init__()
        self.x = x
        self.y = WIN_HEIGHT - Battery.HEIGHT
        self.image = image
        self.bullet_img = bullet_img
        self.speed = speed

    # 炮台左移
    def moveleft(self, parameter, delta_frames = 1):
        buff = parameter * Battery.BUFF
        new_position = self.x - self.speed * frames_to_ms(delta_frames) * buff
        self.x = max(0, new_position)
        
    # 炮台右移
    def moveright(self, parameter, delta_frames = 1):
        buff = parameter * Battery.BUFF
        new_position = self.x + self.speed * frames_to_ms(delta_frames) * buff
        self.x = min(WIN_WIDTH - Battery.WIDTH, new_position)

    # 炮台开火
    def fire(self, bullets):
        bullets.append(Bullet(self.x + Battery.WIDTH / 2, self.y + 5, self.bullet_img, False))

    # 碰撞检测
    def collide(self, bullet):
        return pygame.sprite.collide_mask(self, bullet)

    @property
    def _x(self):
        return self.x
    
    @property
    def _y(self):
        return self.y
    
    @property
    def rect(self):
        return Rect(self.x, self.y, Battery.WIDTH, Battery.HEIGHT)

# 飞机类，敌人
class Plane(pygame.sprite.Sprite):

    WIDTH = HEIGHT = 60
    X_SPEED = 0.3
    Y_SPEED = 0.2

    '''
        x, y：               飞机坐标
        image：              飞机图片
        bullet_img：         子弹图片
        x_speed, y_speed：   x和y方向的速度
        bomb：               是否爆炸或飞出地图
        mask：               碰撞检测
    '''
    def __init__(self, x, y, image, bullet_img, x_speed = X_SPEED, y_speed = Y_SPEED):
        super(Plane, self).__init__()
        self.x = x
        self.y = y
        self.image = image
        self.bullet_img = bullet_img
        self.x_speed = x_speed
        self.y_speed = y_speed
        self.bomb = False
        self.mask = pygame.mask.from_surface(self.image)

    # 飞机左移
    def moveleft(self, delta_frames = 1):
        new_position = self.x - self.x_speed * frames_to_ms(delta_frames)
        self.x = max(0, new_position)

    # 飞机右移
    def moveright(self, delta_frames = 1):
        new_position = self.x + self.x_speed * frames_to_ms(delta_frames)
        self.x = min(WIN_WIDTH - Plane.WIDTH, new_position)

    # 碰撞检测
    def collide(self, bullet):
        return pygame.sprite.collide_mask(self, bullet)

    # 飞机开火
    def fire(self, bullets):
        bullets.append(Bullet(self.x + Plane.WIDTH / 2, self.y + Plane.HEIGHT + 5, self.bullet_img, True))

    # 飞机自动下移
    def update(self, delta_frames = 1):
        self.y += self.y_speed * frames_to_ms(delta_frames)
        if self.y > WIN_HEIGHT:
            self.bomb = True

    # 飞机爆炸
    def bom(self):
        self.bomb = True

    @property
    def _x(self):
        return self.x
    
    @property
    def _y(self):
        return self.y

    @property
    def _bomb(self):
        return self.bomb
    
    @property
    def rect(self):
        return Rect(self.x, self.y, Plane.WIDTH, Plane.HEIGHT)
