# Zirong Xu 91574614
import pygame
import random
import columns_game
import show_effect

COLORS = ['A','B','C','D','E','F','G']
COLORS_DICT = {'A':[255,0,0],'B':[0,255,0],'C':[0,0,255],'D':[255,0,255],'E':[128,0,255],
               'F':[0,255,255],'G':[255,255,0],' ':[0,0,0]}


class graphic_board:
    def __init__(self):
        '''running is used to check whether the user quit the game
           board_state represents the current board
           size is the size of the pygame surface'''
        self._running = True 
        self.board_state = columns_game.current_state(6,13)
        self._size = (300,650)
        
    def run(self):
        '''the main part of the program, run the whole process'''
        pygame.init()
        pygame.display.set_mode(self._size,pygame.RESIZABLE)
        clock = pygame.time.Clock()
        self._board_game_start()
        show_effect.special_effect(pygame.display.get_surface(),self.board_state,0,0).background_music()
        time = 0
        while self._running:
            cell_width = self._size[0] * 1/6
            cell_height = self._size[1] * 1/13
            try:               
                clock.tick(30)
                time += 1
                self.board_state.check_game_over()
                self._events()
                self._draw_board(cell_width,cell_height)
                self._game_process(time)
            except columns_game.GameOverError:
                self._game_over()
                
        pygame.quit()
        
    def _board_game_start(self):
        '''start an empty board'''
        self.board_state.new()
        
    def _events(self):
        '''check the user interruption (press the keys or quit the game)'''
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                self._running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_LEFT:
                    self.board_state = columns_game.handle_command(self.board_state,'<')
                elif event.key == pygame.K_RIGHT:
                    self.board_state = columns_game.handle_command(self.board_state,'>')
                elif event.key == pygame.K_SPACE:
                    self.board_state = columns_game.handle_command(self.board_state,'R')
                elif event.key == pygame.K_DOWN:
                    self.board_state = columns_game.handle_command(self.board_state,'')
##        keys = pygame.key.get_pressed()
##        if keys[pygame.K_RIGHT]:
##            self.board_state = columns_game.handle_command(self.board_state,'>')
##        if keys[pygame.K_LEFT]:
##            self.board_state = columns_game.handle_command(self.board_state,'<')
##        if keys[pygame.K_SPACE]:
##            self.board_state = columns_game.handle_command(self.board_state,'R')
##        if keys[pygame.K_DOWN]:
##            self.board_state = columns_game.handle_command(self.board_state,'')

            
    def _draw_board(self,cell_width,cell_height):
        '''draw the current board in pygame surface using different effect(colors)'''
        surface = pygame.display.get_surface()
        for column_num in range(1,18,3):
            for row_num in range(12,-1,-1):
                self._draw_rect(surface, (int(column_num/3))*cell_width,
                                self._size[1]-(row_num+1)*cell_height,
                                COLORS_DICT[self.board_state.board[column_num][row_num]],
                                cell_width,cell_height)
        self._show_effect(cell_width,cell_height)
        pygame.display.flip()
        
        
    def _draw_rect(self,surface, topleft_pixel_x, topleft_pixel_y, pixel_color, cell_width, cell_height):
        '''draw a rect for given position'''
        pygame.draw.rect(surface, (255,255,255),
                         (topleft_pixel_x, topleft_pixel_y,cell_width,cell_height),0)
        new_color = pixel_color
        for increasing_rate in range(2,25):
            ratio = increasing_rate/50
            pygame.draw.rect(surface, self._change_color(new_color),
                             pygame.Rect(topleft_pixel_x+cell_width*ratio, topleft_pixel_y+cell_height*ratio,
                                         cell_width-2*cell_width*ratio, cell_height-2*cell_height*ratio),0)
            new_color = self._change_color(new_color)


    def _change_color(self,color):
        '''make the color of rect look special'''
        r = color[0]
        g = color[1]
        b = color[2]
        if r == g == b == 0:
            return (0,0,0)
        if r != 255 and r != 128:
            r += 10
        if g != 255 and g != 128:
            g += 10
        if b != 255 and b != 128:
            b += 10
        return (r,g,b)
        
        
    def _show_effect(self,cell_width,cell_height):
        '''display(add) effect to the surface of pygame'''
        surface = pygame.display.get_surface()
        self._effect = show_effect.special_effect(self.board_state, surface,cell_width,cell_height)
        self._effect.run_effect()

        
    def _game_process(self,time):
        '''proceed the game, if no faller in the board then drop a new faller
                             if time reaches 1 sec then make the faller fall one time'''
        if self.board_state.drop_state == 'freeze':
            command = 'F '+ str(random.randrange(1,7)) + ' ' +\
                      random.choice(COLORS) + ' ' + random.choice(COLORS)\
                      + ' '+ random.choice(COLORS)
            self.board_state = columns_game.handle_command(self.board_state,command)
                  
        if time % 30 == 0:
            self.board_state = columns_game.handle_command(self.board_state,'')


    def _game_over(self):
        '''display the game over sign when the game is over'''
        textfont = pygame.font.Font('freesansbold.ttf',50)
        surface = textfont.render('Game Over',True ,(128,128,128))
        rect = surface.get_rect()
        rect.center = (self._size[0]/2,self._size[1]/2)
        pygame.display.get_surface().blit(surface,rect)
        while self._running:
            self._events()
            pygame.display.flip()
            
if __name__ == '__main__':
    graphic_board().run()
