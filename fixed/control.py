import sys
import json 
import time
import pygame
import math #needed for joystick
from pygame.rect import Rect
#import widgets
import serial #needed to talk with Arduino

from utility import *

class Ramp:
    MAX = 0.75
    MIN = 0.0
    STEP = 0.10

    def __init__(self) -> None:
        self.value = Ramp.MIN
    
    def linearRamp(self, doRamp):
        if doRamp and self.value < Ramp.MAX:
            self.value = clamp((self.value + Ramp.STEP), Ramp.MIN, Ramp.MAX)
            return self.value
        elif not doRamp:
            self.value = Ramp.MIN
            return self.value
        else:
            return self.value

class Motor:
    MAX = 1.0
    MIN = -1.0

    def __init__(self, motorName, baseMotorValue, modifierMotorValue = 0, isNegative = False) -> None:
        self.motorName = motorName
        self.baseMotorValue = baseMotorValue
        self.modifierMotorValue = modifierMotorValue
        self.isNegative = isNegative

    def getMotorName(self):
        return self.motorName

    def getMotorValue(self):
        if self.isNegative:
            self.baseMotorValue = -self.baseMotorValue

        return clamp(offset(self.baseMotorValue, self.modifierMotorValue), Motor.MIN, Motor.MAX)

def main():
    pygame.init()

    screen = pygame.display.set_mode((200, 200))
    pygame.display.set_caption("Crush DEPTH")

    ser = serial.Serial(port = 'COM3', baudrate = 9600, timeout = .1, dsrdtr = True) #dsrdtr = True stops Arduino Mega from autoresetting

    vert_mod = False

    clawOpenRamp = Ramp()
    clawSpinRamp = Ramp()


    rightJoystick = None
    if pygame.joystick.get_count()==0:
        print ("No joystick Detected")
        exit(1)
    else:
        rightJoystick = pygame.joystick.Joystick(0)
        rightJoystick.init()#initalize joystick
        leftJoystick = pygame.joystick.Joystick(1)
        leftJoystick.init()#initalize joystick

    if rightJoystick is None or leftJoystick is None:
        print("Joystick error")
        exit(1)
    
    running = True
    while running:
            # Get input from joystick and keyboard
        pygame.event.pump()
        key = pygame.key.get_pressed()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False

        if key[pygame.K_j]:
            running = False
        
        screen.fill((255, 0, 0))
        pygame.display.flip()
        
    # Commands to send ROV
        commands = {} #define python dictionary

        x_new=rightJoystick.get_axis(0)#left joystick -1 is left to +1 is right (left thruster)
        y_new=rightJoystick.get_axis(1) #left joystick -1 is up +1 is down (right thruster)

        zx_new = (leftJoystick.get_axis(0)*-0.75)
        zy_new = (leftJoystick.get_axis(1)*-0.75)

        z_mod = leftJoystick.get_axis(5)
        if rightJoystick.get_button(17):
            vert_mod = not vert_mod

        r_new=(leftJoystick.get_axis(2)*-0.25)

        frontCam = rightJoystick.get_axis(7)

        # button claw code (Right Joystick - right 4 buttons)
        clawOpenButton = rightJoystick.get_button(4)
        clawCloseButton = rightJoystick.get_button(5)
        clawOpen = clawOpenButton - clawCloseButton

        clawSpinLeftButton = rightJoystick.get_button(6)
        clawSpinRightButton = rightJoystick.get_button(7)
        clawSpin = clawSpinLeftButton - clawSpinRightButton

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
        
        # rotate RIGHT joystck 45 degrees for translation 
        x_new2, y_new2 = rotate45(x_new, y_new)

        # rotate LEFT joystick 45 degrees for roll/vertical
        zx_new2, zy_new2 = rotate45(zx_new, zy_new)

        if not vert_mod:
            z_mod = 0

        # list of motors
        motors = [
            # (motorName, baseMotorValue, modifierMotorValue 'optional', isNegative 'optional')

            # xy_plane thrusters
            Motor('frontLeft', y_new2, r_new, True),
            Motor('frontRight', x_new2, r_new, True),
            Motor('backLeft', x_new2, r_new),
            Motor('backRight', y_new2, r_new),

            # z_axis thrusters
            Motor('midLeft', zy_new2, -z_mod, True),
            Motor('midRight', zx_new2, z_mod),

            # claw motors
            Motor('openClaw', (clawOpen * clawOpenRamp.linearRamp(clawOpen))),
            Motor('spinClaw', (clawSpin * clawSpinRamp.linearRamp(clawSpin))),

            # camera servos
            Motor('frontCamera', frontCam, isNegative = True)
            ]

        # fl = offset(-y_new2, r_new)
        # fr = offset(-x_new2, r_new)
        # bl = offset(x_new2, r_new)
        # br = offset(-y_new2, r_new)
        #
        # ml = offset(-zy_new2, -z_mod)
        # mr = offset(zx_new2, z_mod)

        for motor in motors:
            commands[motor.getMotorName()] = motor.getMotorValue()

        # commands['frontLeft'] = clamp(fl, -1, 1)
        # commands['frontRight'] = clamp(fr, -1, 1)
        # commands['backLeft'] = clamp(bl, -1, 1)
        # commands['backRight'] = clamp(br, -1, 1)
        # commands['midLeft'] = clamp(ml, -1, 1)
        # commands['midRight'] = clamp(mr, -1, 1)
        # commands['frontCamera'] = -frontCam

        # # claw code
        # commands['openClaw'] = clawOpen * clawOpenRamp.linearRamp(clawOpen)
        #
        # commands['spinClaw'] = clawSpin * clawSpinRamp.linearRamp(clawSpin)

        MESSAGE=json.dumps(commands)#puts python dictionary in Json format
        ser.write(bytes(MESSAGE, 'utf-8'))#byte format sent to arduino
        ser.flush()

        time.sleep(0.01)

    pygame.quit()

if __name__ == "__main__":
    main()
