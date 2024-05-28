import sys
import json 
import time
import pygame
import math #needed for joystick
from pygame.rect import Rect
#import widgets
import serial #needed to talk with Arduino

pygame.init()

ser = serial.Serial(port='COM5', baudrate=9600, timeout=.1,dsrdtr=True)#dsrdtr=True stops Arduino Mega from autoresetting

joystick = None
if pygame.joystick.get_count()==0:
    print ('No joystick Detected')
else:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()
    print('Joystick Detected')

while True:
    pygame.event.pump()
    key = pygame.key.get_pressed()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False
    
    commands = {}

    if joystick is not None:
        val = joystick.get_axis(1)
        val2 = joystick.get_axis(0)
        
    
    x=(val2*math.cos(math.pi/-4))-(val*math.sin(math.pi/-4)) #horizontal left
    y=(val2*math.sin(math.pi/-4))+(val*math.cos(math.pi/-4))
    print("vals "+ str(val) + " " + str(val2) + " x,y " + str(y) + " " + str(x)) 

    commands['test'] = y

    MESSAGE = json.dumps(commands)
    ser.write(bytes(MESSAGE, 'utf-8'))
    ser.flush()

    time.sleep(0.01)

gamepad.quit()