import copy
import math
import random
from BoardClasses import Move
from BoardClasses import Board


#The following part should be completed by students.
#Students can modify anything except the class name and exisiting functions and varibles.
class StudentAI():

    def __init__(self,col,row,p):
        self.row = row
        self.col = col
        self.p = p
        self.board = Board(col,row,p)
        self.board.initialize_game()
        self.color = ''
        self.opponent = {1:2,2:1}
        self.color = 2
        self.turn_color = {1: "B", 2: "W"}

    def get_move(self, move):
        if len(move) != 0:
            self.board.make_move(move, self.opponent[self.color])
        else:
            self.color = 1

        moves = self.board.get_all_possible_moves(self.color)

        curr_distance = self.king_distance(self.board, self.turn_color[self.color])
        aggressive = False
        late_game = False
        if self.board.black_count + self.board.white_count <= 8:
            late_game = True

        if self.color == 1 and self.board.black_count >= self.board.white_count:
            aggressive = True
        if self.color == 2 and self.board.white_count >= self.board.black_count:
            aggressive = True

        depth = 4
        alpha = -math.inf
        beta = math.inf
        best_moves = []
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(self.board)
                board_copied.make_move(move, self.color)
                curr = self.MinValue(board_copied, depth-1, alpha, beta)

                if late_game:
                    distance_diff = self.king_distance(board_copied, self.turn_color[self.color]) - curr_distance
                    if aggressive:
                        curr += distance_diff/1000
                    else:
                        curr -= distance_diff/1000

                if curr > alpha:
                    alpha = curr
                    best_moves = [move]
                elif curr == alpha:
                    best_moves.append(move)

        best_move = random.choice(best_moves)

        self.board.make_move(best_move, self.color)

        return best_move

    def MaxValue(self, board, depth, alpha, beta):
        moves = board.get_all_possible_moves(self.color)
        if depth == 0:
            # print(self.evaluate(board))
            return self.evaluate(board)
        elif len(moves) == 0:
            if self.checkWinner(board.board, self.color):
                # print("1", self.color, depth)
                return 999
            else:
                # print("2", self.color, depth)
                return -999

        val = -math.inf
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(board)
                board_copied.make_move(move, self.color)
                val = max(val, self.MinValue(board_copied, depth-1, alpha, beta))
                alpha = max(alpha, val)
                if alpha >= beta:
                    return val
        return val

    def MinValue(self, board, depth, alpha, beta):
        moves = board.get_all_possible_moves(self.opponent[self.color])
        if depth == 0:
            # print(self.evaluate(board))
            return self.evaluate(board)
        if len(moves) == 0:
            if self.checkWinner(board.board, self.color):
                # print("3", self.color, depth)
                return 999
            else:
                # print("4", self.color, depth)
                return -999

        val = math.inf
        for row in moves:
            for move in row:
                board_copied = copy.deepcopy(board)
                board_copied.make_move(move, self.opponent[self.color])
                val = min(val, self.MaxValue(board_copied, depth-1, alpha, beta))
                beta = min(beta, val)
                if alpha >= beta:
                    return val
        return val

    def evaluate(self,board):
        if self.color == 1:
            return board.black_count - board.white_count + self.boardEval1(board, "b")/100
        else:
            return board.white_count - board.black_count + self.boardEval1(board, "w")/100

    def boardEval1(self, board, color):
        val = 0
        for i, row in enumerate(board.board):
            for j, col in enumerate(row):

                extra = 0
                if j == 0 or j == len(row):
                    extra = 4

                if color == "b":
                    pawn_val = 5 + i + extra
                    king_val = 5 + len(board.board) + 2 + extra
                    if i == 0:
                        pawn_val = 10 + extra
                else:
                    pawn_val = 5 + (len(board.board) - 1 - i) + extra
                    king_val = 5 + len(board.board) + 2 + extra
                    if i == len(board.board) - 1:
                        pawn_val = 10 + extra

                curr_color = board.board[i][j].get_color().lower()
                if curr_color != '.':
                    if curr_color == color:
                        king = board.board[i][j].is_king
                        if king:
                            val += king_val
                        else:
                            val += pawn_val

                    else:
                        king = board.board[i][j].is_king
                        if king:
                            val -= king_val
                        else:
                            val -= pawn_val

        return val

    def checkWinner(self, board, color):
        my_color = self.turn_color[color]
        oppo_color = self.turn_color[self.opponent[color]]
        for row in range(self.row):
            for col in range(self.col):
                checker = board[row][col]
                if checker.color == my_color:
                    return True
                elif checker.color == oppo_color:
                    return False


    def king_distance(self, board, color):
        k1 = []
        k2 = []
        min_distance = 100
        for row in range(board.row):
            for col in range(board.col):
                checker = board.board[row][col]
                if checker.color != ".":
                    if checker.is_king and checker.color == color:
                        k1.append([row, col])
                    elif checker.color != color:
                        k2.append([row, col])
        for i in k1:
            for j in k2:
                d = self.cal_distance(i,j)
                if self.cal_distance(i,j) < min_distance:
                    min_distance = d
        # print(k1, k2, min_distance)
        return min_distance

    def cal_distance(self, p1, p2):
        return math.sqrt(math.pow(p1[0]-p2[0], 2) + math.pow(p1[1]-p2[1], 2))


# 1. 每过数层，减去平局值以下的分支
# 2. 边和角的权值增加
# 3. ？ 优势时优先进攻，劣势优先防守
# 4. iterative deepen, 超时abort
