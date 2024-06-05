import math

def clamp(value, min_value, max_value):
    return max(min_value, min(value, max_value))

def rotate45(xInputValue, yInputValue):
    xValue = (xInputValue * math.cos(math.pi / -4)) - (yInputValue * math.sin(math.pi / -4))
    yValue = (xInputValue * math.sin(math.pi / -4)) + (yInputValue * math.cos(math.pi / -4))
    return xValue, yValue

def offset(inputValue, offsetValue):
    return inputValue + offsetValue
