"""
This module houses the ManualAI which is used when running the shell with the manual run options.

We are following the javadoc docstring format which is:
@param tag describes the input parameters of the function
@return tag describes what the function returns
@raise tag describes the errors this function can raise
"""

from Move import Move
from BoardClasses import Board
from random import randint
import math
import copy

class ManualAI():
    """
    This class describes the ManualAI.
    """
    def __init__(self,col,row,p):
        """
        Intializes manualAI
        @param row: no of rows in the board
        @param col: no of columns in the board
        @param k: no of rows to be filled with checker pieces at the start
        @return :
        @raise :
        """
        self.col = col
        self.row = row
        self.p = p
        self.board = Board(col,row,p)
        self.board.initialize_game()
        self.color = 2
        self.opponent = {1:2,2:1} # to switch turns after each tur

    # def get_move(self,move):
    #     """
    #     get_move function for manualAI called from the gameloop in the main module.
    #     @param move: A Move object describing the move.
    #     @return res_move: A Move object describing the move manualAI wants to make. This move is basically console input.
    #     @raise :
    #     """
    #     if move.seq:
    #         # if move.seq is not an empty list
    #         self.board.make_move(move,self.opponent[self.color])
    #     else:
    #         self.color = 1
    #     moves = self.board.get_all_possible_moves(self.color)
    #     while True:
    #         try:
    #             for i,checker_moves in enumerate(moves):
    #                 print(i,':[',end="")
    #                 for j, move in enumerate(checker_moves):
    #                     print(j,":",move,end=", ")
    #                 print("]")
    #             index,inner_index = map(lambda x: int(x), input("Select Move {int} {int}: ").split()) # input is from console is handled here.
    #             res_move = moves[index][inner_index]
    #         except KeyboardInterrupt:
    #             raise KeyboardInterrupt
    #         except:
    #             print('invalid move')
    #             continue
    #         else:
    #             break
    #     self.board.make_move(res_move, self.color)
    #     return res_move

    def get_move(self, move):
        if len(move) != 0:
            self.board.make_move(move, self.opponent[self.color])
        else:
            self.color = 1

        moves = self.board.get_all_possible_moves(self.color)

        depth = 4
        alpha = -math.inf
        beta = math.inf
        best_moves = []
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(self.board)
                board_copied.make_move(move, self.color)
                curr = self.MinValue(board_copied, depth - 1, alpha, beta)
                if curr > alpha:
                    alpha = curr
                    best_moves = [move]
                elif curr == alpha:
                    best_moves.append(move)
        best_move = best_moves[randint(0, len(best_moves) - 1)]
        self.board.make_move(best_move, self.color)
        # print("Moves: ", moves)
        # print("Best: ", best_move)
        # print("Score: ", max_score)
        return best_move

    def MaxValue(self, board, depth, alpha, beta):
        moves = board.get_all_possible_moves(self.color)
        if depth == 0:
            # print(self.evaluate(board))
            return self.evaluate(board)
        elif len(moves) == 0:
            if board.is_win(self.color):
                return math.inf - 1
            else:
                return -math.inf + 1

        val = -math.inf
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(board)
                board_copied.make_move(move, self.color)
                val = max(val, self.MinValue(board_copied, depth - 1, alpha, beta))
                if val >= beta:
                    return val
                alpha = max(alpha, val)
        return val

    def MinValue(self, board, depth, alpha, beta):
        moves = board.get_all_possible_moves(self.opponent[self.color])
        if depth == 0:
            # print(self.evaluate(board))
            return self.evaluate(board)
        elif len(moves) == 0:
            if board.is_win(self.color):
                return math.inf - 1
            else:
                return -math.inf + 1

        val = math.inf
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(board)
                board_copied.make_move(move, self.opponent[self.color])
                val = min(val, self.MaxValue(board_copied, depth - 1, alpha, beta))
                if val <= alpha:
                    return val
                beta = min(beta, val)
        return beta

    def evaluate(self,board):
        if self.color == 1:
            return board.black_count - board.white_count
        else:
            return board.white_count - board.black_count