import math
def convert(angle:float, degrees:bool = True):
    if(degrees):
        angle = math.degrees(angle)
    angle = angle/0.293255132
    value = math.ceil(angle)
    return value