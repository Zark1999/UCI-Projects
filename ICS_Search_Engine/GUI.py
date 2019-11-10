from tkinter import *
from tkinter import scrolledtext
from query_handling import *
from time import time
import pandas as pd

page = Tk()

box = Entry(page)
result = scrolledtext.ScrolledText()
index = pd.read_pickle('index.pkl')

def add():
    result.delete(1.0,END)
    query = box.get()
    t1 = time()
    s = query_handling(index,query)
    t2 = time()
    text = "Search Time: "+ str(t2-t1) + '\n\n'
    text += '\n'.join('{}\n{}\n'.format(find_title(i[0]),i[2]) for i in s)
    result.insert(INSERT,text)
    print(time()-t2)
#     result.insert(INSERT,"Search Time: "+ str(t2-t1) + '\n\n') 
#     for i in s:
#         result.insert(INSERT,'{}\n{}\n\n'.format(find_title(i[0]),i[2]))
    
search = Button(page,text="Search",command=add)


box.grid(row=0,column=0,columnspan=2,sticky=N+E+W)
search.grid(row=0,column=2,sticky=N+E+W)
result.grid(row=1,column=0,columnspan=3,sticky=N+S+E+W)

page.columnconfigure(0, weight=1)
page.columnconfigure(1, weight=1)
page.columnconfigure(2, weight=1)
page.rowconfigure(0,weight=1)
page.rowconfigure(1,weight=1)


page.mainloop()