'''
Created on Oct 10, 2018

@author: kevin
'''
import hang_model as model
if __name__ == '__main__':
        game = model.Game()
        game.get_words(game.getDiff())
        game.pick_word()
        game.get_selfword()
        while game.running:
            print(game.get_solvespace())
            game.check()
            
