# A Pulsator is a Black_Hole; it updates as a Black_Hole
#   does, but also by growing/shrinking depending on
#   whether or not it eats Prey (and removing itself from
#   the simulation if its dimension becomes 0), and displays
#   as a Black_Hole but with varying dimensions


from blackhole import Black_Hole


class Pulsator(Black_Hole): 
    max_count = 30
    def __init__(self, x, y):
        Black_Hole.__init__(self, x, y)
        self._counter = 0
        
    def update(self, model):
        temp = Black_Hole.update(self, model)
        if len(temp) != 0:
            for _ in range(len(temp)):
                self.change_dimension(1, 1)
            self._counter = 0
            return temp
        else:
            self._counter += 1
            if self._counter == self.max_count:
                self._counter = 0
                self.change_dimension(-1, -1)
                if self.get_dimension() == (0, 0):
                    return {self}
                return set()
