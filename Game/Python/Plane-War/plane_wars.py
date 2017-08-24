# -*- coding: utf-8 -*-
from random import randint
import time

import pygame
from pygame.locals import *

from agents import *

def image_load(path):
    img = pygame.image.load(path)
    img.convert()
    return img

def main():
    pygame.init()

    # 和难度相关的系数
    planes_interval = [2000]
    fire_interval = [2000]
    attack = [200]
    scores = [1]

    # 难度
    level = 0

    # 初始化
    display_surface = pygame.display.set_mode((WIN_WIDTH, WIN_HEIGHT))
    pygame.display.set_caption('Planes War')

    clock = pygame.time.Clock()

    background = image_load('images/universe.jpg')
    battery_img = image_load('images/battery.png')
    bullet_img = image_load('images/bullet.png')
    plane_img = image_load('images/plane.png')

    battery = Battery(randint(0, WIN_WIDTH - Battery.WIDTH), battery_img, bullet_img)
    bullets = []
    planes = []
                             
    # 初始状态
    display_surface.blit(background, (0, 0))
    display_surface.blit(battery.image, battery.rect)
    pygame.display.flip()
    
    # 按空格开始游戏
    while True:
        flag = False
        for event in pygame.event.get():
            if event.type ==KEYDOWN:
                if event.key == K_SPACE:
                    flag = True
                    break
        if flag:
            break
    
    fail = False
    frame_clock = 0
    score = 0
    
    # 游戏主循环
    while not fail:
        clock.tick(FPS)

        if not (frame_clock % ms_to_frames(planes_interval[level])):
            planes.append(Plane(randint(0,WIN_WIDTH), 1, plane_img, bullet_img))

        # 敌机开火
        if not (frame_clock % ms_to_frames(fire_interval[level])):
            for p in planes:
                p.fire(bullets)
            
        # 鼠标控制炮台移动
        parameter = pygame.mouse.get_pos()[0] - (battery._x + Battery.WIDTH / 2)
        if parameter < 0:
            battery.moveleft(-parameter)
        else:
            battery.moveright(parameter)
        
        for event in pygame.event.get():
            if event.type == QUIT:
                pygame.quit()
            if event.type == MOUSEBUTTONDOWN:
                battery.fire(bullets)

        # 飞机自动移动
        for p in planes:
            x_distance = p._x - battery._x - Battery.WIDTH / 2
            y_distance = battery._y - p._y

            # 攻击模式
            if y_distance < attack[level]:
                if x_distance < -4:
                    p.moveright()
                elif x_distance > 4:
                    p.moveleft()

            # 闪避模式
            else:
                if x_distance < -4:
                    p.moveleft()
                elif x_distance > 4:
                    p.moveright()

        # 检查碰撞
        for b in bullets:
            for p in planes:
                if p.collide(b) and not b._enemy:
                    p.bom()
                    score += 1
            if battery.collide(b):
                fail = True
                continue

        # 刷新画面
        display_surface.blit(background, (0, 0))
        display_surface.blit(battery_img, battery.rect)
        
        for b in bullets:
            b.update()
            if b._bomb:
                bullets.remove(b)
            else:
                display_surface.blit(bullet_img, b.rect)

        for p in planes:
            p.update()
            if p._bomb:
                planes.remove(p)
            else:
                display_surface.blit(plane_img, p.rect)
        
        pygame.display.flip()
        frame_clock += 1
        
    print 'Game Over! Your score is ', score
    pygame.quit()

if __name__ == '__main__':
    main()
