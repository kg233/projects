import mechanics as m
import pygame
import random

class Game:
    def __init__(self):
        self._game = m.GameField(m.create_empty_list(13,6))
        self._running = True
        self._colors = [pygame.Color(44,219,116), pygame.Color(240,65,65), pygame.Color(247,132,54), pygame.Color(0,255,229), pygame.Color(242,0,255),pygame.Color(25,0,255), pygame.Color(245, 255,0)]
        self._faller_list = [random_faller(),random_faller()]
        self._clock = pygame.time.Clock()
        self._FRAMERATE = 30
        
    def run(self) -> None:
        '''run the game'''
        pygame.init()
        self._font = pygame.font.SysFont("Arial", 25)
        self._end_font = pygame.font.SysFont("Arial", 95)
        self._set_display((469, 675))
        tick = 0
        while self._running:
            self._clock.tick(self._FRAMERATE)
            tick += 1
            if tick == self._FRAMERATE:
                self._tick()
                if not self._game.hasfaller:
                    self._game.check_all()
                    try:
                        self._check_game_over()
                    except m.GameOverError:
                        self._game_over_ui()
                        return
                    else:
                        try:
                            self._make_faller()
                        except m.GameOverError:
                            self._game_over_ui()
                            return
                tick = 0
            self._handle_events()
            self._draw_frame()

        pygame.quit()
    

    def _check_game_over(self) -> None:
        '''check for game over'''
        self._game.blockfield.check_over()

    def _game_over_ui(self) -> None:
        '''the ui when the game is over, exits when a key is pressed'''
        done = False
        while not done:
            self._clock.tick(self._FRAMERATE)
            self._draw_frame()
            self._draw_end_score()
            pygame.display.flip() # two flips
            for event in pygame.event.get():
                if event.type == pygame.QUIT or event.type == pygame.KEYDOWN:
                    done = True
                elif event.type == pygame.VIDEORESIZE:
                    self._set_display(event.size)
        pygame.quit()
            
            
        

    def _handle_events(self) -> None:
        '''handle events'''
        if self._running:
            for event in pygame.event.get():
                self._handle_event(event)

    def _handle_event(self, event) -> None:
        '''handle a single event'''
        if event.type == pygame.QUIT:
            self._end_game()
        elif event.type == pygame.VIDEORESIZE:
            self._set_display(event.size)
            #print(event.size)
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_LEFT:
                self._game.move_left()
            elif event.key == pygame.K_RIGHT:
                self._game.move_right()
            elif event.key == pygame.K_SPACE:
                self._game.rotate()
            elif event.key == pygame.K_DOWN:
                self._tick()
            
    def _draw_frame(self) -> None:
        '''draw the frame'''
        self._surface.fill(pygame.Color(132,171,186))
        self._draw_board()
        self._draw_future()
        self._draw_scores()
        pygame.display.flip()
            
    def _set_display(self, size: (int,int)) -> None:
        '''set the RESIZABLE display'''
        self._surface = pygame.display.set_mode(size, pygame.RESIZABLE)
        
    def _draw_board(self) -> None:
        '''draw the board from a list'''
        truncate_list = truncate(self._game.list)
        for ind1 in range(len(truncate_list)):
            col_frac_coord = col_fraction_coord(ind1)
            for ind2 in range(len(truncate_list[0])):
                row_frac_coord = row_fraction_coord(ind2)
                self._draw_one_cell(col_frac_coord, row_frac_coord, ind1, ind2, truncate_list)

    def _draw_one_cell(self, f_cord1, f_cord2, ind1, ind2, l:list) -> None:
        '''draw one box'''
        total_width = self._surface.get_width() * 0.7
        total_height = self._surface.get_height()
        width = total_width * (1/6)
        height = total_height * (1/13)
        pixel_pos_x = total_width * (f_cord1 - 1/12)
        pixel_pos_y = total_height * (f_cord2 - 1/26)
        pygame.draw.rect(self._surface, pygame.Color(37,115,69), pygame.Rect(pixel_pos_x, pixel_pos_y, width, height),1)
        self._draw_type(pixel_pos_x, pixel_pos_y, width, height, ind1, ind2, l)

    def _draw_type(self, pixel_pos_x, pixel_pos_y, width, height, ind1, ind2, l:list):
        '''draw the cell whether it is a lander, faller, or block'''
        if l[ind1][ind2][0] == '|':
            pygame.draw.rect(self._surface, pygame.Color(132,131,186), pygame.Rect(pixel_pos_x+1, pixel_pos_y+1, width-2, height-2))
        elif l[ind1][ind2][0] == '*':
            pygame.draw.rect(self._surface, pygame.Color(255,255,255), pygame.Rect(pixel_pos_x+1, pixel_pos_y+1, width-2, height-2)) 
        if l[ind1][ind2][1] != ' ':
            pygame.draw.rect(self._surface, self._colors[int(l[ind1][ind2][1])], pygame.Rect(pixel_pos_x + 6, pixel_pos_y + 6, width - 12, height - 12))

    def _tick(self) -> None:
        '''time'''
        self._game.age()

    def _draw_future(self) -> None:
        '''draw future faller'''
        self._draw_future_box()
        self._draw_future_faller()

    def _draw_future_box(self) ->None:
        '''draw the boxes for future faller'''
        width = self._surface.get_width() * (1/9)
        height = self._surface.get_height() * (1/13)
        pygame.draw.rect(self._surface, pygame.Color(37,115,69), pygame.Rect(self._surface.get_width() * 0.8, self._surface.get_height() * 0.1 , width, height),1)
        pygame.draw.rect(self._surface, pygame.Color(37,115,69), pygame.Rect(self._surface.get_width() * 0.8, self._surface.get_height() * 0.1 + height, width, height),1)
        pygame.draw.rect(self._surface, pygame.Color(37,115,69), pygame.Rect(self._surface.get_width() * 0.8, self._surface.get_height() * 0.1 + 2*height, width, height),1)

    def _draw_future_faller(self) -> None:
        '''draw the type of future faller'''
        if len(self._faller_list) > 1:
            column, color1, color2, color3 = self._faller_list[0]
            width = self._surface.get_width() * (1/9)
            height = self._surface.get_height() * (1/13)
            pygame.draw.rect(self._surface, self._colors[int(color3)], pygame.Rect(self._surface.get_width() * 0.8 + 6, self._surface.get_height() * 0.1 + 6 , width - 12, height -12))
            pygame.draw.rect(self._surface, self._colors[int(color2)], pygame.Rect(self._surface.get_width() * 0.8 + 6, (self._surface.get_height() * 0.1 + height) + 6, width -12, height-12))
            pygame.draw.rect(self._surface, self._colors[int(color1)], pygame.Rect(self._surface.get_width() * 0.8 + 6, (self._surface.get_height() * 0.1 + 2*height) + 6, width - 12, height - 12))

    def _draw_scores(self) -> None:
        '''scores'''
        self._draw_score_banner()
        self._draw_score()

    def _draw_score_banner(self) -> None:
        '''words for scores'''
        score_banner_surface = self._font.render('Score:', True, pygame.Color(0,0,0), pygame.Color(132,171,186))
        self._surface.blit(score_banner_surface, (self._surface.get_width() * 0.75,self._surface.get_height() * 0.6))
    def _draw_score(self) -> None:
        '''score number'''
        score_surface = self._font.render(str(self._game._score), True, pygame.Color(0,0,0), pygame.Color(132,171,186))
        self._surface.blit(score_surface, (self._surface.get_width() * 0.75,self._surface.get_height() * 0.65))

    def _draw_end_score(self) -> None:
        '''end game score'''
        self._draw_end_game_banner()
        self._draw_end_game_score()

    def _draw_end_game_banner(self):
        '''end game banner'''
        score_banner_surface = self._end_font.render('Score:', False, pygame.Color(255,0,0))
        self._surface.blit(score_banner_surface, (self._surface.get_width() * 0.15,self._surface.get_height() * 0.1))
        score_banner = self._font.render('Press Any Key To Quit', False, pygame.Color(255,0,0))
        self._surface.blit(score_banner, (self._surface.get_width() * 0.2,self._surface.get_height() * 0.6))

    def _draw_end_game_score(self):
        '''end game score number'''
        score = self._end_font.render(str(self._game._score), False, pygame.Color(255,0,0))
        self._surface.blit(score, (self._surface.get_width() * 0.3,self._surface.get_height() * 0.3))
        

    def _make_faller(self) -> None:
        '''make a faller'''
        if not self._game.matching:
            column, x, y, z = self._faller_list.pop(0)
            self._make_faller_list()
            self._game.make_faller(column, x, y, z)
            

    def _end_game(self) -> None:
        '''end game, '''
        self._running = False

    def _make_faller_list(self) -> None:
        '''make a list of future faller'''
        self._faller_list.append(random_faller())
        
def col_fraction_coord(ind: int) -> float:
    '''given a index, find the right ratio on the board'''
    return 1/12 + ind * 1/6

def row_fraction_coord(ind:int) -> float:
    '''given a index, find the right ratio on the board'''
    return 1/26 + ind * 1/13

def truncate(l:list) -> list:
    '''cuts the first two elemnts from the list'''
    result = []
    norm_list = m.transform_list_perspective(l)
    for ind in range(2, len(norm_list)):
        result.append(norm_list[ind])
    return m.transform_list_perspective(result)

def random_once() -> int:
    '''randomly chose a number between one to six'''
    return random.randrange(0,7)

def random_faller() -> tuple:
    '''randomly choose a column and colors for a faller'''
    return random.randrange(0,6), random_once(), random_once(), random_once()

if __name__ == '__main__':
    game = Game()
    game.run()
