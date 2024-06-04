import sys
import json 
import time
import pygame
import math #needed for joystick
from pygame.rect import Rect
#import widgets
import serial #needed to talk with Arduino

pygame.init()

ser = serial.Serial(port = 'COM5', baudrate = 9600, timeout = .1, dsrdtr = True) #dsrdtr = True stops Arduino Mega from autoresetting

vert_mod = False

joystick = None
if pygame.joystick.get_count()==0:
    print ('No joystick Detected')
else:
    joystick = pygame.joystick.Joystick(0)
    joystick.init()#initalize joystick
    joystick1 = pygame.joystick.Joystick(1)
    joystick1.init()#initalize joystick

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

        zx_new = (joystick1.get_axis(0)*-0.75)
        zy_new = (joystick1.get_axis(1)*-0.75)

        z_mod = joystick1.get_axis(5)
        if joystick.get_button(7):
            vert_mod = not vert_mod

        r_new=(joystick1.get_axis(2)*-0.25)

        frontCam = joystick.get_axis(7)

        clawOpenAxis = joystick.get_axis(5)
        clawSpinAxis = joystick1.get_axis(7)

    deadZone = 0.0001
    if abs(y_new)<deadZone: #define a dead zone
        y_new=0
    if abs(x_new)<deadZone: #define a dead zone
        x_new=0
    
    if abs(zx_new)<deadZone: #define a dead zone
        zx_new=0
    if abs(zy_new)<deadZone: #define a dead zone
        zy_new=0
    
    if abs(r_new)<deadZone: #define a dead zone
        r_new=0
    
    #rotate x and y axis of joystick 45 degrees
    
    x_new2=(x_new*math.cos(math.pi/-4))-(y_new*math.sin(math.pi/-4)) #horizontal left
    y_new2=(x_new*math.sin(math.pi/-4))+(y_new*math.cos(math.pi/-4)) #horizontal right

    #rotate thumb axes for up/down and roll movements
    zx_new2=(zx_new*math.cos(math.pi/-4))-(zy_new*math.sin(math.pi/-4)) #horizontal left
    zy_new2=(zx_new*math.sin(math.pi/-4))+(zy_new*math.cos(math.pi/-4)) #horizontal right

    if not vert_mod:
        z_mod = 0

    fl = ((-1*y_new2)+r_new) #front left thruster
    fr = ((-1*x_new2)+r_new) #front right thruster
    bl = (x_new2+r_new) #back left thruster
    br = ((-y_new2)+r_new) #back right thruster

    # clamp values to -1
    fl = max(-1, min(1, fl))
    fr = max(-1, min(1, fr))
    bl = max(-1, min(1, bl))
    br = max(-1, min(1, br))
    #zx_new = max(-1, min(1, zx_new))
    #zy_new = max(-1, min(1, zy_new))
    clawOpenAxis = max(-1, min(1, clawOpenAxis))
    clawSpinAxis = max(-1, min(1, clawSpinAxis))

    commands['frontLeft'] = fl
    commands['frontRight'] = fr
    commands['backLeft'] = bl
    commands['backRight'] = br
    commands['midLeft'] = (-1*zy_new2)-z_mod
    commands['midRight'] = zx_new2+z_mod
    commands['frontCamera'] = -1*frontCam

    commands['clawOpenAxis'] = clawSpinAxis
    commands['clawSpinAxis'] = clawSpinAxis

    MESSAGE=json.dumps(commands)#puts python dictionary in Json format
    ser.write(bytes(MESSAGE, 'utf-8'))#byte format sent to arduino
    ser.flush()

    time.sleep(0.01)

pygame.quit()