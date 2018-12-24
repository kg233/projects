'''
Created on Oct 11, 2018

@author: kevin
'''
# hang_model
import random
_DEBUG = False

class Game:
    def __init__(self):
        self.diffi = {1:"Easy", 2:"Medium"}
        self.word = ''
        self.solvespace = []
        self.lives = 6
        self.running = True
    
    def getDiff(self) -> int:
        diff = None
        while not( type(diff) == int and diff in (1,2)):
            try:
                diff = int(input("please type the difficulty level (1 2):"))
            except:
                None
        print(self.diffi[diff], "difficulty selected")
        return diff
        
    def get_words(self, lvl: int):
        self.wordset = set()
        for line in open(self.diffi[lvl] + '.txt'):
            self.wordset.add(line[:-1])
            
        if _DEBUG == True:
            for word in self.wordset: 
                if len(word)  < 2:
                    print("ERROR", word)
            print("debug empty str done")
            
    def pick_word(self):
        '''pick a word from the self.wordset'''
        assert len(self.wordset) != 0, "ERROR, No word found"
        self.word = random.sample(self.wordset, 1)[0]
        self._create_underline()
        
    def _create_underline(self):
        for _ in range(len(self.word)):
            self.solvespace.append('_')
            
    def get_solvespace(self):
        return ' '.join(self.solvespace) + '\n'
    
    def prompt_char(self):
        char = ''
        while len(char) < 1:
            char = input("enter a valid character:")
            if char in self.solvespace:
                char = ''
                
        return char
    
    def game_end(self):
        print("GAME FINISHED, answer was", self.word)
        self.running = False
        return None
    
    def check(self):
        '''checks if letter is in the word, modifies self.word'''
        char = self.prompt_char()
        if char == self.word:
            self.game_end()
        else:
           
            old = self.solvespace[:]
            if len(char) == 1:
                self._char_finder(self.word, char, 0)
            if old == self.solvespace:
                if _DEBUG:
                    print(old)
                    print(self.solvespace)
                self.lives -= 1
                print("-1 life, now", self.lives, "life")
        self.check_dead()
        
    def check_dead(self):
        if self.lives == 0:
            self.game_end()
    def _char_finder(self, mod, char, ini_ind):
        ind = mod.find(char)
        if ind == -1:
            return None
        else:
            self.solvespace[ini_ind + ind] = char
            return self._char_finder(mod[ind+1:], char, ind+ini_ind+1)
        
    def get_selfword(self):
        """debug only, shows the chosen word"""
        if _DEBUG == True:
            print(self.word)