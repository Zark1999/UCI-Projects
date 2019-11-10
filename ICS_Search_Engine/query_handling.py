import time
import math
from tokenizing import tokenizing
from collections import defaultdict
from pathlib import Path
from bs4 import BeautifulSoup


def query_handling(index,s):
    '''returns (docid,normalized,link)'''
    query = s
    bookkeeping_path ='/Users/zirongxu/Desktop/CS_Assignment/CS_121/proj3/WEBPAGES_RAW/bookkeeping.tsv'
     #{term:{docID:[tf-idf,tf,tag],...}, ...], ...}
    query_dict = tokenizing(query)
    
    result_dict = defaultdict(list) #{docid:[normalized,{tag}]}
    
    if len(query_dict) == 1:
        try:
            term = list(query_dict)[0]
            for docid in index[term]:
                result_dict[docid] = [index[term][docid][1]]
        except:
            pass
    
    else:
        for term in query_dict:
            try:
                for docid in index[term]:
                    tag = index[term][docid][2]
                    tf_idf = (1 + math.log10(query_dict[term])) * math.log10(index['***']['doc_number']/len(index[term]))
                    normalized = tf_idf * (index[term][docid][0]/index['%%%'][docid])
                    if result_dict[docid] == []:
                        result_dict[docid] = [normalized,{tag}]
                    else:
                        if tag in result_dict[docid][1]:
                            result_dict[docid][0] += normalized * 1.05
                        else:
                            result_dict[docid][0] += normalized
                            result_dict[docid][1].add(tag)
 
            except:
                pass
        

    result_tuple = sorted(result_dict.items(),key=lambda x:x[1][0],reverse=True)[:20] #(id,similarity)
    
    with open(bookkeeping_path,'r',encoding='utf-8') as file:
        ids = set(i[0] for i in result_tuple)
        for line in file:
            l = line.split()
            if l[0] in ids:
                for num in range(len(result_tuple)):
                    if result_tuple[num][0] == l[0]:
                        result_tuple[num] = (result_tuple[num][0],result_tuple[num][1][0],l[1])
                        
    return result_tuple

def find_title(docid):
    '''returns title in the file if exists otherwise "No title available"'''
    file_path = Path("/Users/zirongxu/Desktop/CS_Assignment/CS_121/proj3/WEBPAGES_RAW/") /docid
    with open(file_path,'r',encoding='utf-8') as f:
        soup = BeautifulSoup(f,"lxml")
        result = soup.find('title')
        if result:
            return result.text
        else:
            return 'No title available.'
