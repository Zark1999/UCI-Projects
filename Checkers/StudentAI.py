import copy
import math
from random import randint
from BoardClasses import Move
from BoardClasses import Board


#The following part should be completed by students.
#Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI():

    def __init__(self,row,col,p):
        self.row = row
        self.col = col
        self.p = p
        self.board = Board(row,col,p)
        self.board.initialize_game()
        self.color = ''
        self.opponent = {1:2,2:1}
        self.color = 2

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
                curr = self.MinValue(board_copied, depth-1, alpha, beta)
                if curr > alpha:
                    alpha = curr
                    best_moves = [move]
                elif curr == alpha:
                    best_moves.append(move)
        best_move = best_moves[randint(0, len(best_moves)-1)]
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
                return math.inf-1
            else:
                return -math.inf+1

        val = -math.inf
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(board)
                board_copied.make_move(move, self.color)
                val = max(val, self.MinValue(board_copied, depth-1, alpha, beta))
                if val >= beta:
                    return val
                alpha = max(alpha, val)
        return val

    def MinValue(self, board, depth, alpha, beta):
        moves = board.get_all_possible_moves(self.opponent[self.color])
        if depth == 0:
            # print(self.evaluate(board))
            return self.evaluate(board)
        if len(moves) == 0:
            if board.is_win(self.color):
                return math.inf-1
            else:
                return -math.inf+1

        val = math.inf
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(board)
                board_copied.make_move(move, self.opponent[self.color])
                val = min(val, self.MaxValue(board_copied, depth-1, alpha, beta))
                if val <= alpha:
                    return val
                beta = min(beta, val)
        return beta

    def evaluate(self,board):
        result = 0
        if self.color == 1:
            result += board.black_count - board.white_count + self.edgeValue(board, "b")
        else:
            result += board.white_count - board.black_count + self.edgeValue(board, "w")

        return result

    def edgeValue(self, board, color):
        val = 0
        for i, row in enumerate(board.board):
            for j, col in enumerate(row):
                if board.board[i][j].get_color().lower() == color:
                    king = board.board[i][j].is_king
                    if king:
                        val += 0.02
                    if i == 0 or j == 0 or i == self.row-1 or j == self.col -1:
                        val += 0.01
        return val


# 1. 每过数层，减去平局值以下的分支
# 2. 边和角的权值增加
# 3. ？ 优势时优先进攻，劣势优先防守
# 4. iterative deepen, 超时abort
