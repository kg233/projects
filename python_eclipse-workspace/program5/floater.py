# A Floater is Prey; it updates by moving mostly in
#   a straight line, but with random changes to its
#   angle and speed, and displays as ufo.gif (whose
#   dimensions (width and height) are computed by
#   calling .width()/.height() on the PhotoImage


# from PIL.ImageTk import PhotoImage
from prey import Prey
from random import random


class Floater(Prey): 
    radius = 5
    def __init__(self, x, y):
        Prey.__init__(self, x, y, self.radius*2, self.radius*2, 0, 5)
        self.randomize_angle()
        
    def update(self ,model):
        dec = random()
        if 0 <= dec <= 0.30:
            delta_spd = random() - 0.5
            delta_ang = random() - 0.5
            if not(self.get_speed()+ delta_spd > 7 or self.get_speed() + delta_spd < 3):
                self.set_speed(self.get_speed() + delta_spd)
            self.set_angle(self.get_angle() + delta_ang)
            
        self.move()
        self.wall_bounce()
    
    def display(self, canvas):
        
        canvas.create_oval(self.get_location()[0] - self.radius, self.get_location()[1] - self.radius, self.get_location()[0] + self.radius, self.get_location()[1] + self.radius, fill = '#FF0000')