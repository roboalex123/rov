import time
import pygame
import pygame.camera
from pygame.locals import *

import socket
import pickle

def main():
    pygame.init()
    pygame.camera.init()

    windowSize = (1280, 720)

    screen = pygame.display.set_mode(windowSize)
    pygame.display.set_caption("Crush DEPTH")

    camList = pygame.camera.list_cameras()
    if camList:
        # camera res (1280, 720) or (1920, 1080)
        cam = pygame.camera.Camera(camList[0], (1280, 720))
        cam.start()
    else:
        raise ValueError("No cam")
    
    camImage = cam.get_image()

    running = True
    while running:
        pygame.event.pump()
        key = pygame.key.get_pressed()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
        
        if key[pygame.K_j]:
            running = False
        
        if cam.query_image():
            camImage = pygame.transform.flip(cam.get_image(), False, True)
        
        screen.blit(camImage, (0, 0))

        pygame.display.flip()
    
    cam.stop()
    pygame.quit()

if __name__ == "__main__":
    main()