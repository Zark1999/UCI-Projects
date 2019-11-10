#Zirong Xu 91574614

class GameOverError(Exception):
    pass


class current_state:   
    def __init__(self,column_num,row_num):
        '''column: number of columns
           row: number of rows
           board: current status of the board
           drop_state: current situation of the game
           faller: new faller that is gonna dropped
           faller_headpointer: the coordinate of the first one cell in the faller'''
        self.column = column_num
        self.row = row_num
        self.board = []
        self.drop_state = 'freeze'
        self.faller = []
        self.faller_headpointer = [0,0]
##################################################
# class functions for beginning the game
    def new(self):
        '''create an empty board'''
        for each_column in range(self.column * 3):
            row_list = []
            for each_row in range(self.row):
                row_list.append(' ')
            self.board.append(row_list)
##################################################
# commmon class functions
    def _fill(self):
        '''all jewels fill empty space below them'''
        for column_num in range(1,len(self.board),3):
            for row_num in range(1,self.row):
                if self.board[column_num][row_num-1] == ' ' and self.board[column_num][row_num] != ' ':
                    self.board[column_num][row_num-1] = self.board[column_num][row_num]
                    self.board[column_num-1][row_num-1] = self.board[column_num-1][row_num]
                    self.board[column_num+1][row_num-1] = self.board[column_num+1][row_num]
                    self.board[column_num][row_num] = ' '
                    self.board[column_num-1][row_num] = ' '
                    self.board[column_num+1][row_num] = ' '
##################################################
# class functions for finding and checking matching when board freezes
    def check_matching(self):
        '''check whether there are matchings in the board, 3 different situations: in one row/in one column/in one diagonal'''
        for column_num1 in range(1,len(self.board),3):
            for row_num1 in range(self.row-2):
                if self.board[column_num1][row_num1] == self.board[column_num1][row_num1+1] == self.board[column_num1][row_num1+2] != ' ':
                    self.board[column_num1-1][row_num1] = '*'
                    self.board[column_num1+1][row_num1] = '*'
                    self.board[column_num1-1][row_num1+1] = '*'
                    self.board[column_num1+1][row_num1+1] = '*'
                    self.board[column_num1-1][row_num1+2] = '*'
                    self.board[column_num1+1][row_num1+2] = '*'
                    self.drop_state = 'matching'
                    
        for row_num2 in range(self.row):
            for column_num2 in range(1,len(self.board)-6,3):
                if self.board[column_num2][row_num2] == self.board[column_num2+3][row_num2] == self.board[column_num2+6][row_num2] != ' ':
                    self.board[column_num2-1][row_num2] = '*'
                    self.board[column_num2+1][row_num2] = '*'
                    self.board[column_num2+2][row_num2] = '*'
                    self.board[column_num2+4][row_num2] = '*'
                    self.board[column_num2+5][row_num2] = '*'
                    self.board[column_num2+7][row_num2] = '*'
                    self.drop_state = 'matching'

        for column_num3 in range(4,len(self.board)-3,3):
            for row_num3 in range(1,self.row-1):
                if self.board[column_num3][row_num3] == self.board[column_num3-3][row_num3-1] == self.board[column_num3+3][row_num3+1] != ' ':
                    self.board[column_num3-1][row_num3] = '*'
                    self.board[column_num3+1][row_num3] = '*'
                    self.board[column_num3-4][row_num3-1] = '*'
                    self.board[column_num3-2][row_num3-1] = '*'
                    self.board[column_num3+2][row_num3+1] = '*'
                    self.board[column_num3+4][row_num3+1] = '*'
                    self.drop_state = 'matching'
                    
                if self.board[column_num3][row_num3] == self.board[column_num3-3][row_num3+1] == self.board[column_num3+3][row_num3-1] != ' ':
                    self.board[column_num3-1][row_num3] = '*'
                    self.board[column_num3+1][row_num3] = '*'
                    self.board[column_num3-4][row_num3+1] = '*'
                    self.board[column_num3-2][row_num3+1] = '*'
                    self.board[column_num3+2][row_num3-1] = '*'
                    self.board[column_num3+4][row_num3-1] = '*'
                    self.drop_state = 'matching'

    def clear_matching(self):
        '''clear all the matchings'''
        for column_num in range(1,len(self.board),3):
            for row_num in range(self.row):
                if self.board[column_num-1][row_num] == '*' and self.board[column_num+1][row_num] == '*':
                    self.board[column_num-1][row_num] = ' '
                    self.board[column_num][row_num] = ' '
                    self.board[column_num+1][row_num] = ' '
        for each_turn in range(self.row):
            self._fill()

        while self.faller_headpointer[1] > self.row - len(self.faller) and self.board[self.faller_headpointer[0]][self.row - 1] == ' ':
            self._fill()
            self.board[self.faller_headpointer[0]][self.row - 1] = self.faller[self.row - self.faller_headpointer[1]]
            self.faller_headpointer[1] -= 1
            self.clear_matching()
            
        for each_turn in range(self.row):
            self._fill()
##################################################
# class functions for droping a new faller in the board and checking the status of the faller.
    def drop_new_column(self):
        '''drop a new column'''
        if self.faller_headpointer[1] == self.row:
            if self.board[self.faller_headpointer[0]][self.row-2] == ' ':
                self.board[self.faller_headpointer[0]-1][self.row-1] = '['
                self.board[self.faller_headpointer[0]][self.row-1] = self.faller[self.row - self.faller_headpointer[1]]
                self.board[self.faller_headpointer[0]+1][self.row-1] = ']'
                self.faller_headpointer[1] -= 1
            else:
                self.board[self.faller_headpointer[0]-1][self.row-1] = '|'
                self.board[self.faller_headpointer[0]][self.row-1] = self.faller[self.row - self.faller_headpointer[1]]
                self.board[self.faller_headpointer[0]+1][self.row-1] = '|'
                self.faller_headpointer[1] -=1 
                self.drop_state = 'land'
            
        elif self.faller_headpointer[1] > self.row - len(self.faller):
            if  self.faller_headpointer[1] != 0 and self.board[self.faller_headpointer[0]][self.faller_headpointer[1]-1] == ' ':
                self._fill()
                self.board[self.faller_headpointer[0]-1][self.row-1] = '['
                self.board[self.faller_headpointer[0]][self.row-1] = self.faller[self.row - self.faller_headpointer[1]]
                self.board[self.faller_headpointer[0]+1][self.row-1] = ']'
                self.faller_headpointer[1] -= 1
                self._check_landing()
            else:
                self._check_landing()
        else:
            if self.drop_state == 'drop':
                self._fill()
                self.faller_headpointer[1] -= 1
            self._check_landing()
           
    def _check_landing(self):
        '''check whether the faller is droping or landing'''
        if self.faller_headpointer[1] == 0 or self.board[self.faller_headpointer[0]][self.faller_headpointer[1] - 1] != ' ':
            if self.faller_headpointer[1] + len(self.faller) > self.row:
                upperbound = self.row
            else:
                upperbound = self.faller_headpointer[1] + len(self.faller)
            for row_num in range(self.faller_headpointer[1],upperbound):
                self.board[self.faller_headpointer[0]-1][row_num] = '|'
                self.board[self.faller_headpointer[0]+1][row_num] = '|'
            self.drop_state = 'land'
        else:
            if self.faller_headpointer[1] + len(self.faller) > self.row:
                upperbound = self.row
            else:
                upperbound = self.faller_headpointer[1] + len(self.faller)
            for row_num in range(self.faller_headpointer[1],upperbound):
                self.board[self.faller_headpointer[0]-1][row_num] = '['
                self.board[self.faller_headpointer[0]+1][row_num] = ']'
            self.drop_state = 'drop'
##################################################
# class functions for rotating and moving the faller while the board is not freezing
    def faller_rotate(self):
        '''rotate the faller'''
        self.faller.append(self.faller[0])
        del self.faller[0]
        if self.faller_headpointer[1] + len(self.faller) > self.row:
            self.board[self.faller_headpointer[0]][self.faller_headpointer[1]] = ' '
            if self.faller_headpointer[1] == self.row - 1:
                self.board[self.faller_headpointer[0]][self.row-1] = self.faller[self.row - self.faller_headpointer[1] - 1]
            else:
                self._fill()
                self.board[self.faller_headpointer[0]-1][self.row-1] = self.board[self.faller_headpointer[0]-1][self.row-2]
                self.board[self.faller_headpointer[0]][self.row-1] = self.faller[self.row - self.faller_headpointer[1] - 1]
                self.board[self.faller_headpointer[0]+1][self.row-1] = self.board[self.faller_headpointer[0]+1][self.row-2]             
        else:
            upperbound = self.faller_headpointer[1] + len(self.faller)
            element = self.board[self.faller_headpointer[0]][self.faller_headpointer[1]]
            self.board[self.faller_headpointer[0]][self.faller_headpointer[1]] = ' '
            self._fill()
            self.board[self.faller_headpointer[0]-1][upperbound-1] = self.board[self.faller_headpointer[0]-1][upperbound-2]
            self.board[self.faller_headpointer[0]][upperbound-1] = element
            self.board[self.faller_headpointer[0]+1][upperbound-1] = self.board[self.faller_headpointer[0]+1][upperbound-2]
               
                
    def move_right(self):
        '''move the faller to the right'''
        if self.faller_headpointer[0] < self.column*3-2 and self.board[self.faller_headpointer[0]+3][self.faller_headpointer[1]] == ' ':
            for row_num in range(self.faller_headpointer[1],self.row):
                self.board[self.faller_headpointer[0]+2][row_num] = self.board[self.faller_headpointer[0]-1][row_num]
                self.board[self.faller_headpointer[0]+3][row_num] = self.board[self.faller_headpointer[0]][row_num]
                self.board[self.faller_headpointer[0]+4][row_num] = self.board[self.faller_headpointer[0]+1][row_num]
                self.board[self.faller_headpointer[0]][row_num] = ' '
                self.board[self.faller_headpointer[0]-1][row_num] = ' '
                self.board[self.faller_headpointer[0]+1][row_num] = ' '
            self.faller_headpointer[0] += 3
            self._check_landing()
                
    
    def move_left(self):
        '''move the faller to the left'''
        head_coordinate = [self.faller_headpointer[0],self.faller_headpointer[1]]
        if head_coordinate[0] > 1 and self.board[head_coordinate[0]-3][head_coordinate[1]] == ' ':
            for row_num in range(self.faller_headpointer[1],self.row):
                self.board[self.faller_headpointer[0]-2][row_num] = self.board[self.faller_headpointer[0]+1][row_num]
                self.board[self.faller_headpointer[0]-3][row_num] = self.board[self.faller_headpointer[0]][row_num]
                self.board[self.faller_headpointer[0]-4][row_num] = self.board[self.faller_headpointer[0]-1][row_num]
                self.board[self.faller_headpointer[0]][row_num] = ' '
                self.board[self.faller_headpointer[0]-1][row_num] = ' '
                self.board[self.faller_headpointer[0]+1][row_num] = ' '
            self.faller_headpointer[0] -= 3
            self._check_landing()
################################################## 
# class function for checking game over
    def check_game_over(self):
        '''check whether game is over'''
        if self.faller_headpointer[1] > self.row - len(self.faller) and self.drop_state == 'freeze':
            raise GameOverError

##################################################   
def handle_command(state:current_state,command = str) -> current_state:
    '''handle all the command(user input)'''
    if command.startswith('F') and state.drop_state == 'freeze':
        drop_column_num = int(command.split()[1])
        state.faller = command.split()[2:][::-1]
        state.drop_state = 'drop'
        state.faller_headpointer = [drop_column_num*3-2,state.row]
        if state.board[state.faller_headpointer[0]][state.row-1] != ' ':
            raise GameOverError
        state.drop_new_column()
        
    elif command == '':
        if state.drop_state == 'matching':
            state.clear_matching()
            state.drop_state = 'freeze'
            state.check_matching()

        elif state.drop_state == 'drop':
            state.drop_new_column()
            
        elif state.drop_state == 'land':
            state.drop_state = 'freeze'           
            for column_num in range(1,len(state.board),3):
                for row_num in range(state.row):
                    state.board[column_num-1][row_num] = ' '
                    state.board[column_num+1][row_num] = ' '
            state.check_matching()
            
    elif state.drop_state != 'freeze' and state.drop_state != 'matching':
        if command == 'R':
            state.faller_rotate()
        elif command == '>':
            state.move_right()
        elif command == '<':
            state.move_left()
        
    return state
