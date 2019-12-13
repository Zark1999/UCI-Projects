import asyncio
import sys
import time
import signal

global limit

class Test:

    counter = 1

    def __init__(self):
        data = {"testAI": {"1_win": 0, "1_lose": 0, "tie": 0}}
        with open('data.txt', "w+") as f:
            f.write(str(data))

    async def run(self, cmd):
        # Create the subprocess; redirect the standard output
        # into a pipe.
        proc = await asyncio.create_subprocess_shell(
            cmd,
            stdout=asyncio.subprocess.PIPE,
            stderr=asyncio.subprocess.PIPE)

        stdout, stderr = await proc.communicate()

        if stdout:
            print("#" + str(self.counter), end=" ")
            self.counter += 1
            print(f'{stdout.decode()}')
        if stderr:
            print(f'{stderr.decode()}')

    async def run_all(self, s):
        # await asyncio.gather(
        #     self.run("python3 main.py 9 8 2 m " + s),
        #     self.run("python3 main.py 9 8 2 m " + s),
        #     self.run("python3 main.py 9 8 2 m " + s),
        #     self.run("python3 main.py 9 8 2 m " + s),
        #     self.run("python3 main.py 9 8 2 m " + s),
        # )

        await asyncio.gather(
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
            self.run("python3 main.py 7 7 2 m " + s),
        )


def signal_handler(sig, frame):
    global limit
    with open('data.txt', "r") as file:
        result = eval(file.read().strip())
        print(result["testAI"])
        if (result["testAI"]["1_win"] + result["testAI"]["1_lose"]) != 0:
            print("Player1 win rate: %" + str(
                result["testAI"]["1_win"] / (result["testAI"]["1_win"] + result["testAI"]["1_lose"]) * 100))
            print("Player2 win rate: %" + str(
                result["testAI"]["1_lose"] / (result["testAI"]["1_win"] + result["testAI"]["1_lose"]) * 100))
        print("Time out of limit: " + str(limit))
    sys.exit(0)


if __name__ == "__main__":
    global limit
    t = Test()
    limit = 0
    limits = []
    signal.signal(signal.SIGINT, signal_handler)
    for i in range(1):
        t1 = time.time()
        asyncio.run(t.run_all("1"))
        t2 = time.time() - t1
        if t2 > 8 * 60:
            limit += 5
            limits.append(t2)

    with open('data.txt', "r") as file:
        result = eval(file.readline().strip())

        print(result["testAI"])
        if (result["testAI"]["1_win"]+result["testAI"]["1_lose"]) != 0:
            print("Player1 win rate: %" + str(result["testAI"]["1_win"]/(result["testAI"]["1_win"]+result["testAI"]["1_lose"])*100))
            print("Player2 win rate: %" + str(result["testAI"]["1_lose"]/(result["testAI"]["1_win"]+result["testAI"]["1_lose"])*100))
        print("Time out of limit: " + str(limit))


