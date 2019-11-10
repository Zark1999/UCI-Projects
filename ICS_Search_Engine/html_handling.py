from tokenizing import tokenizing
from bs4 import BeautifulSoup
from bs4.element import Comment
from pathlib import Path
from collections import defaultdict
import math
import pandas as pd
import time

wbpages_path = Path('/Users/zirongxu/Desktop/CS_Assignment/CS_121/proj3/WEBPAGES_RAW/')


def indexing():
    '''returns {term:{docID:[tf-idf,tf,tag],...}, ...], ...}
    uses lnc.ltc'''
    t1 = time.time()
    text_dict = defaultdict(dict)
    doclen_dict = defaultdict(float)
    
    doc_num = 0
    for path in wbpages_path.iterdir():
        if path.is_dir():
            print(path.stem)
            for file in path.iterdir():
                with open(file,'r',encoding='utf-8') as f:
                    try:
                        parsed_dict = html_parsing(f) #{stem:[frequency,tag],...}
                    except:
                        print(f)
                        continue
                    doc_num += 1
                    doc_id = path.stem+'/'+file.stem
                    for text in parsed_dict:
                        frequency = parsed_dict[text][0]
                        tf = float(1 + math.log10(frequency))
                        text_dict[text][doc_id] = [tf,frequency,parsed_dict[text][1]]
            
    for term in text_dict:
        
        for docid in text_dict[term]:
            idf = math.log10(doc_num / len(text_dict[term]))
            text_dict[term][docid][0] = text_dict[term][docid][0] * idf
            doclen_dict[docid] += math.pow(text_dict[term][docid][0],2)
        
    
    for docid in doclen_dict:
        doclen_dict[docid] = math.sqrt(doclen_dict[docid])
    
    text_dict['%%%'] = doclen_dict
    text_dict['***'] = {'doc_number':doc_num}
#         text_dict[term].sort(key=lambda x:(-x[1],x[0]))
    t2 = time.time()
    print(t2-t1)
    result = pd.Series(text_dict)
    result.to_pickle('index.pkl')
    t3 = time.time()
    print(t3-t2)
    with open('123.txt','w',encoding='utf-8') as f2:
        for i in text_dict:
            f2.write('{} & {}\n'.format(i,text_dict[i]))
        
        

def html_parsing(f):
    '''returns {stem:[frequency,tag],...}'''
    soup = BeautifulSoup(f,"html.parser")
    result = defaultdict(list)
    
    for t in soup.findAll(text=True):
        if not (t.parent.name in ['style', 'script', 'head', 'title', 'meta', '[document]'] or isinstance(t,Comment)):
            parsed = tokenizing(t.strip())
            for stem in parsed:
                if result[stem] == []:
                    result[stem] = [parsed[stem],t.parent.name]
                else:
                    result[stem][0] += parsed[stem]
                
    return result

indexing()