import sys
import json 
import time
import pygame
import math #needed for joystick
from pygame.rect import Rect
import widgets
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
        z_new=joystick.get_axis(5) #
        r_new=-0.125*(joystick.get_axis(2))# rotational axis value from joystick
        for i in range(joystick.get_numbuttons()):
            if joystick.get_button(i) or joystick1.get_button(i):
                print(f"Button {i} is pressed")
    if abs(y_new)<.01: #define a dead zone
        y_new=0
    if abs(x_new)<.01: #define a dead zone
        x_new=0
    if abs(z_new)<.01: #define a dead zone
        z_new=0
    if abs(r_new)<.01: #define a dead zone
        r_new=0
    
    #rotate x and y axis of joystick 45 degrees
    
    x_new2=((x_new*math.cos(math.pi/-4))-(y_new*math.sin(math.pi/-4))) #horizontal left
    y_new2=((x_new*math.sin(math.pi/-4))+(y_new*math.cos(math.pi/-4))) #horizontal right
    print(x_new2)
    print(y_new2)
    #x_new2 = (x_new2 + ((math.sqrt(2)-1)*x_new2))**3
    #y_new2 = (y_new2 + ((math.sqrt(2)-1)*y_new2))**3

    #limits joystick values to +/- 1
    fl = ((-1*y_new2)+r_new)
    fr = ((-1*x_new2)+r_new)
    bl = (x_new2+r_new)
    br = ((-1*y_new2)+r_new)
    if bl>1.0:
        bl=1.0
    if br>1.0:
        br=1.0
    if bl<-1.0:
        bl=-1.0
    if br<-1.0:
        br=-1.0
    if fl>1.0:
        fl=1.0
    if fr>1.0:
        fr=1.0
    if fl<-1.0:
        fl=-1.0
    if fr<-1.0:
        fr=-1.0
    
    #print(fl)
    #print(fr)
    #print(bl)
    #print(br)
    
    #add to dictionary
    #cube gives more control with lower power
    #commands['camAngle']= z_new**3
    commands['frontLeft']=fl
    commands['frontRight']=fr
    commands['midLeft']=z_new**3
    commands['midRight']=z_new**3
    commands['backLeft']=bl
    commands['backRight']=br
    """
    if x_new>1:
        x_new=1.0
    if y_new>1:
        y_new=1.0
    if x_new2<-1:
        x_new2=-1.0
    if y_new2<-1:
        y_new2=-1.0
    
    #add to dictionary
    #cube gives more control with lower power
    #commands['camAngle']= z_new**3
    commands['frontLeft']=((-1*y_new2)+r_new)**3
    commands['frontRight']=((-1*x_new2)+r_new)**3
    commands['midLeft']=z_new**3
    commands['midRight']=z_new**3
    commands['backLeft']=(x_new2+r_new)**3
    commands['backRight']=((-1*y_new2)+r_new)**3
    """
    MESSAGE=json.dumps(commands)#puts python dictionary in Json format
    ser.write(bytes(MESSAGE, 'utf-8'))#byte format sent to arduino
    ser.flush()

    time.sleep(0.01)

pygame.quit()