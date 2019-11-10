# Zirong Xu 91574614

import pygame

class special_effect:
    def __init__(self, board_state, surface, cell_width, cell_height):
        '''_board_state represents the current board
           surface is the surface of pygame
           cell_width/height is the width/height of each cell(brick)'''
        self._board_state = board_state
        self._surface = surface
        self._cell_width = cell_width
        self._cell_height = cell_height

    def run_effect(self):
        '''display all the effect'''
        self._mathching_effect()
        self._landing_effect()

#################################################
    def _mathching_effect(self):
        '''display the match effect'''
        for column_num in range(1,18,3):
            for row_num in range(13):
                if self._board_state.board[column_num-1][row_num] == '*' and self._board_state.board[column_num+1][row_num] == '*':
                    self._show_pixel_matching(self._surface, int(column_num/3)*self._cell_width, self._cell_height*13-(row_num+1)*self._cell_height)

        
    def _show_pixel_matching(self,surface,x_topleft,y_topleft):
        '''show effect if the bricks/cells match'''
        cell_width = self._cell_width
        cell_height = self._cell_height
        x_topleft -= 1
        y_topleft -= 1
        pygame.draw.polygon(surface, (255,128,0),[[x_topleft,y_topleft],[x_topleft+cell_width,y_topleft],
                                              [x_topleft+cell_width,y_topleft+cell_height],[x_topleft,y_topleft+cell_height]],
                            5)
#################################################
    def _landing_effect(self):
        '''display the land effect'''
        cell_width = self._cell_width
        cell_height = self._cell_height
        if self._board_state.drop_state == 'land':
            x_bottomleft = (int(self._board_state.faller_headpointer[0]/3)) * cell_width
            y_bottomleft = cell_height*13 - (self._board_state.faller_headpointer[1]) * cell_height
            self._show_pixel_landing(self._surface,x_bottomleft,y_bottomleft)

    def _show_pixel_landing(self,surface,x_bottomleft,y_bottomleft):
        '''show effect if the bricks/cells land'''
        cell_width = self._cell_width
        cell_height = self._cell_height
        x_topleft = x_bottomleft - cell_width/50
        y_topleft = y_bottomleft - cell_height*3 - cell_height/50
        pygame.draw.polygon(surface, (170,168,0),[[x_topleft,y_topleft],[x_topleft+cell_width,y_topleft],
                                              [x_topleft+cell_width,y_topleft+cell_height*3],[x_topleft,y_topleft+cell_height*3]],
                            5)

#################################################
    def background_music(self):
        '''display the background music'''
        pygame.mixer.music.load("background_music.mp3")
        pygame.mixer.music.play(loops = -1)
