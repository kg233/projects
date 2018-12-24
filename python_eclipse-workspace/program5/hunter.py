# Hunter is derived from the Mobile_Simulton/Pulsator classes: i.e., it updates
#   like a Pulsator, but it also moves (either in a straight line
#   or in pursuit of Prey), and displays as a Pulsator.


from prey import Prey
from pulsator import Pulsator
from mobilesimulton import Mobile_Simulton
from math import atan2


class Hunter(Pulsator, Mobile_Simulton):
    vision = 200
    def __init__(self, x, y):
        Pulsator.__init__(self, x, y)
        Mobile_Simulton.__init__(self, x, y, 20, 20, 0, 5)
        self.randomize_angle()
    
    def update(self, model):
        temp = None
        for item in model.find(lambda x: True if isinstance(x, Prey) else False):
            if self.distance(item.get_location()) <= self.vision and (temp == None or self.distance(item.get_location()) < self.distance(temp.get_location())):
                temp = item
        if temp != None:
            self.set_angle(atan2(temp.get_location()[1] - self.get_location()[1], temp.get_location()[0] - self.get_location()[0]))
        self.move()
        self.wall_bounce()
        return Pulsator.update(self, model)