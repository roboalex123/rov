import sys
import json 
import time
import pygame
import math #needed for joystick
from pygame.rect import Rect
#import widgets
import serial #needed to talk with Arduino

def clamp(value, min_value, max_value):
    return max(min_value, min(value, max_value))

def rotate45(xInputValue, yInputValue):
    xValue = (xInputValue * math.cos(math.pi / -4))
    yValue = (yInputValue * math.sin(math.pi / -4))
    return (xValue - yValue), (xValue + yValue)

def offset(inputValue, offsetValue):
    return inputValue + offsetValue

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


def main():
    pygame.init()

    ser = serial.Serial(port = 'COM3', baudrate = 9600, timeout = .1, dsrdtr = True) #dsrdtr = True stops Arduino Mega from autoresetting

    vert_mod = False

    clawOpenRamp = Ramp()
    clawSpinRamp = Ramp()

    rightJoystick = None
    if pygame.joystick.get_count()==0:
        print ('No joystick Detected')
        exit(1)
    else:
        rightJoystick = pygame.joystick.Joystick(0)
        rightJoystick.init()#initalize joystick
        leftJoystick = pygame.joystick.Joystick(1)
        leftJoystick.init()#initalize joystick

    if rightJoystick is None or leftJoystick is None:
        print("Joystick error")
        exit(1)
    while True:
            # Get input from joystick and keyboard
        pygame.event.pump()
        key = pygame.key.get_pressed()
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False


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

        fl = offset(-y_new2, r_new)
        fr = offset(-x_new2, r_new)
        bl = offset(x_new2, r_new)
        br = offset(-y_new2, r_new)

        ml = offset(-zy_new2, -z_mod)
        mr = offset(zx_new2, z_mod)

        commands['frontLeft'] = clamp(fl, -1, 1)
        commands['frontRight'] = clamp(fr, -1, 1)
        commands['backLeft'] = clamp(bl, -1, 1)
        commands['backRight'] = clamp(br, -1, 1)
        commands['midLeft'] = clamp(ml, -1, 1)
        commands['midRight'] = clamp(mr, -1, 1)
        commands['frontCamera'] = -frontCam

        # claw code
        commands['openClaw'] = clawOpen * clawOpenRamp.linearRamp(clawOpen)

        commands['spinClaw'] = clawSpin * clawSpinRamp.linearRamp(clawSpin)

        MESSAGE=json.dumps(commands)#puts python dictionary in Json format
        ser.write(bytes(MESSAGE, 'utf-8'))#byte format sent to arduino
        ser.flush()

        time.sleep(0.01)

    pygame.quit()

if __name__ == "__main__":
    main()