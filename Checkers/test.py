import os
import pandas as pd

data = {"testAI": {"1_win": 0, "1_lose": 0, "tie": 0}}
num_games = 10

stored = pd.Series(data)
stored.to_pickle('data.pkl')


for i in range(num_games):
    print("#" + str(i+1), end=" ")
    os.system("python3 main.py 7 7 2 m 0")


result = pd.read_pickle("data.pkl")

print(result["testAI"])
print("Player1 win rate: %" + result["testAI"]["1_win"]/(result["testAI"]["1_win"]+result["testAI"]["1_lose"])*100)
print("Player2 win rate: %" + result["testAI"]["1_lose"]/(result["testAI"]["1_win"]+result["testAI"]["1_lose"])*100)



