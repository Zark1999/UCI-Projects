# usage: python gendigit.py NUMBER_OF_DIGITS DIGIT_TO_PRINT
# use > redirection to create files
import sys

i = int(sys.argv[1])
digit = str(int(sys.argv[2]))
while i > 0:
    sys.stdout.write(digit)
    i -= 1
