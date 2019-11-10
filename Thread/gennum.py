# usage: python gennum.py NUMBER_OF_DIGITS
# use > redirection to create files
import random
import sys

i = int(sys.argv[1])
while i > 0:
    sys.stdout.write(str(random.randint(0,9)))
    i -= 1
