# Black_Hole is derived from Simulton: i.e., it updates by finding and removing
#   any Prey whose center is contained within its radius
#  (returning a set of all eaten simultons), and
#   displays as a black circle with a radius of 10
#   (width/height 20).
# Calling get_dimension for the width/height (for
#   containment and displaying) will facilitate
#   inheritance in Pulsator and Hunter

from simulton import Simulton
from prey import Prey

class Black_Hole(Simulton):
    radius = 10
    def __init__(self, x, y):
        Simulton.__init__(self, x, y, self.radius*2, self.radius*2)
        
    def update(self, model):
        temp = set()
        for item in model.find(lambda x: True if isinstance(x, Prey) else False):
            if self.contains(item):
                temp.add(item)
        return temp
            
        
    def contains(self, prey):
        return self.distance(prey.get_location()) < self.get_dimension()[0]/2
    
    def display(self, canvas):
        canvas.create_oval(self.get_location()[0] - self.get_dimension()[0]/2, \
                           self.get_location()[1] - self.get_dimension()[1]/2, \
                           self.get_location()[0] + self.get_dimension()[0]/2, \
                           self.get_location()[1] + self.get_dimension()[1]/2, fill = "#000000")
