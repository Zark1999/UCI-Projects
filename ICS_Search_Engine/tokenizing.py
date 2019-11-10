import nltk
import re
from collections import defaultdict
    
def replace_nonalphanumeric(s):
    '''substitute the non-alphanumeric characters with empty space in the string
    time complexity: O(N), where N is the size of the string'''
    return re.sub(r'[^a-zA-Z0-9]+', r' ', s.lower())

def stem_word(w):
    stemmer = nltk.stem.PorterStemmer()
    return stemmer.stem(w)


def tokenizing(s):
    """returns in a format {stem:frequency}"""
    dic = defaultdict(int)
    word_list = replace_nonalphanumeric(s).split()
    for word in word_list:
        w = stem_word(word)
        dic[w] += 1
    return dic

