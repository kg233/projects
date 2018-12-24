import controller, sys
import model   # We need a reference to this module to pass it to update methods

from ball      import Ball
from floater   import Floater
from blackhole import Black_Hole
from pulsator  import Pulsator
from hunter    import Hunter
from special import Special


# Global variables: declare them global in functions that assign to them: e.g., ... = or +=
running = False
cycle = 0
simultons = set()
obj = None

#return a 2-tuple of the width and height of the canvas (defined in the controller)
def world():
    return (controller.the_canvas.winfo_width(),controller.the_canvas.winfo_height())

#reset all module variables to represent an empty/stopped simulation
def reset ():
    global running, cycle, simultons, obj
    running = False
    cycle = 0
    simultons = set()
    obj = None


#start running the simulation
def start ():
    global running
    running = True



#stop running the simulation (freezing it)
def stop ():
    global running
    running = False


#tep just one update in the simulation
def step ():
    global running, cycle, simultons
    running = False
    cycle += 1
    eat = set()
    for item in simultons:
        temp = item.update(model)
        if temp != None:
            eat = eat | temp
    simultons = simultons - eat

    display_all()
    



#remember the kind of object to add to the simulation when an (x,y) coordinate in the canvas
#  is clicked next (or remember to remove an object by such a click)   
def select_object(kind):
    global obj
    obj = kind


#add the kind of remembered object to the simulation (or remove all objects that contain the
#  clicked (x,y) coordinate
def mouse_click(x,y):
    global simultons
    if obj != None:
        if obj != 'Remove':
            add(eval(obj)(x,y))
        else:
            temp = set()
            for item in simultons:
                if item.distance((x,y)) < item.get_dimension()[0]/2:
                    
                    temp.add(item)
            
            simultons = simultons-temp
            
        


#add simulton s to the simulation
def add(s):
    simultons.add(s)
    

# remove simulton s from the simulation    
def remove(s):
    simultons.remove(s)
    

#find/return a set of simultons that each satisfy predicate p    
def find(p):
    temp = set()
    for item in simultons:
        if p(item):
            temp.add(item)
    return temp
        


#call update for every simulton in the simulation (pass each model)
#this function should loop over one set containing all the simultons
#  and should not call type or isinstance: let each simulton do the
#  right thing for itself, without this function knowing what kinds of
#  simultons are in the simulation
def update_all():
    global cycle, simultons
    if running:
        cycle += 1
        eat = set()
        for item in simultons:
            temp = item.update(model)
            if temp != None:
                eat = eat | temp
        simultons = simultons - eat
    


#delete every simulton being simulated from the canvas; next call display on every
#  simulton being simulated to add it back to the canvas, possibly in a new location, to
#  animate it; also, update the progress label defined in the controller
#this function should loop over one set containing all the simultons
#  and should not call type or isinstance: let each simulton do the
#  right thing for itself, without this function knowing what kinds of
#  simultons are in the simulation
def display_all():
    for o in controller.the_canvas.find_all():
        controller.the_canvas.delete(o)
        
    for item in simultons:
        item.display(controller.the_canvas)
        
    controller.the_progress.config(text=str(cycle)+" updates/" + str(len(simultons))+" simultons")
