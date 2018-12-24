#special is class inherit from mobile simulton
# it is not a prey, but if initially touched by the black hole, pulsator, or hunter, it disappear
# but will have no added bonus to the pulsator or hunter since its not a prey
# it move slowly but ramp up in speed and size
# once it get to a certain size threshhold it will began to consume blackhole classes regardless of their size
# each consumtion will reduce special's size by a set amount, meaning that it will need time before
# it can consume the next black hole

# starts with size 4 and speed 3, random angle, gain 1 to size and 0.5 to speed every 5 updates
# will be able to consume black hole classes at size 50
# after which it will lose 60% of its size and 80% of its speed
 
from mobilesimulton import Mobile_Simulton
from blackhole import Black_Hole
class Special(Mobile_Simulton):
    tick_max = 5
    eat_threshold = 30
    radius = 2
    speed = 3
    def __init__(self, x, y):
        Mobile_Simulton.__init__(self, x, y, self.radius*2, self.radius*2, 0, self.speed)
        self.randomize_angle()
        self._tick = 0
    
    def update(self, model):
        for item in model.find(lambda x: True if isinstance(x, Black_Hole) else False):
            if self.distance(item.get_location()) < self.get_dimension()[0]/2:
                if self.get_dimension()[0] < self.eat_threshold:
                    return {self}
                else:
                    self.set_speed(self.get_speed()*0.2)
                    self.set_dimension(self.get_dimension()[0]*0.4, self.get_dimension()[1]*0.4)
                    return {item}
        self._tick += 1
        if self._tick == self.tick_max:
            self.change_dimension(1,1)
            self.set_speed(self.get_speed()+ 0.5)
            self._tick = 0
        self.move()
        self.wall_bounce()
        
    def display(self, canvas):
        canvas.create_oval(self.get_location()[0] - self.get_dimension()[0]/2, \
                           self.get_location()[1] - self.get_dimension()[1]/2, \
                           self.get_location()[0] + self.get_dimension()[0]/2, \
                           self.get_location()[1] + self.get_dimension()[1]/2, fill = "#00FF00")