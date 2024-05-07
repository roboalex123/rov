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
    joystick.init()#initalize joystick
    joystick1 = pygame.joystick.Joystick(1)
    joystick1.init()#initalize joystick
    print(joystick.get_numaxes())
    print(joystick.get_numbuttons())


while True:
        # Get input from joystick and keyboard
    pygame.event.pump()
    key = pygame.key.get_pressed()
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False


# Commands to send ROV
    commands = {} #define python dictionary
    if joystick is not None:
        x_new=joystick.get_axis(0)#left joystick -1 is left to +1 is right (left thruster)
        y_new=joystick.get_axis(1) #left joystick -1 is up +1 is down (right thruster)
        z_new=joystick.get_axis(4) #right joystick x-axis, used for vertical
        r_new=joystick.get_axis(2) #right joystick y-axis, used for rotation
        r_new=-0.125*r_new #reduce rotation speed

        stepX = joystick1.get_axis(0)
        stepY = joystick1.get_axis(1)
        stepZ = joystick1.get_axis(5)

    if abs(y_new)<.01: #define a dead zone
        y_new=0
    if abs(x_new)<.01: #define a dead zone
        x_new=0
    if abs(z_new)<.01: #define a dead zone
        z_new=0
    if abs(r_new)<.01: #define a dead zone
        r_new=0

    if abs(stepX)<.01: #define a dead zone
        stepX=0
    if abs(stepY)<.01: #define a dead zone
        stepY=0
    if abs(stepZ)<.01: #define a dead zone
        stepZ=0
    
    #rotate x and y axis of joystick 45 degrees
    
    x_new2=(x_new*math.cos(math.pi/-4))-(y_new*math.sin(math.pi/-4)) #horizontal left
    y_new2=(x_new*math.sin(math.pi/-4))+(y_new*math.cos(math.pi/-4)) #horizontal right

    fl = ((-1*y_new2)+r_new) #front left thruster
    fr = ((-1*x_new2)+r_new) #front right thruster
    bl = (x_new2+r_new) #back left thruster
    br = ((-y_new2)+r_new) #back right thruster

    # clamp values to -1
    fl = max(-1, min(1, fl))
    fr = max(-1, min(1, fr))
    bl = max(-1, min(1, bl))
    br = max(-1, min(1, br))

    commands['frontLeft'] = fl
    commands['frontRight'] = fr
    commands['backLeft'] = bl
    commands['backRight'] = br
    commands['midLeft'] = z_new**3
    commands['midRight'] = z_new**3

    commands['X'] = stepX**3
    commands['Y'] = stepY**3
    commands['Z'] = stepZ**3

    MESSAGE=json.dumps(commands)#puts python dictionary in Json format
    ser.write(bytes(MESSAGE, 'utf-8'))#byte format sent to arduino
    ser.flush()

    time.sleep(0.01)

pygame.quit()
