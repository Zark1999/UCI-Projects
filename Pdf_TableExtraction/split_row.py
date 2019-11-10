from collections import defaultdict
from Row import Row
import xml.etree.ElementTree as ET
import re

def xml2dict(xml_filename):
    """returns {page_id:[row],...}"""
    
    height_diff_tolerance = 3
    
    tree = ET.parse(xml_filename)
    root = tree.getroot()
    
    pages = defaultdict(list)
    page_id = 1

    for page in root.iter('page'):
        for textline in page.iter('textline'):

            line_coord = textline.attrib['bbox'].split(',')    # get the coordinates for the textline
            r = Row(line_coord[1],line_coord[3],[i for i in textline.iter('text')])     # create a Row
    
            if pages[page_id] == []:   # if it is empty means r is the first row add to the page[page_id]
                pages[page_id].append(r)
                continue
            
            min_diff = 1000     # set a default min_diff
            
            for row in pages[page_id]:      # find the nearest existing Row in pages[page_id]
                diff = abs(row.min_val()-r.min_val())
                if diff < min_diff:
                    min_diff = diff
                    which_row = row
            
            if min_diff > height_diff_tolerance:    # check the y difference between current Row and its existing nearest Row
                pages[page_id].append(r)        # if difference is larger than tolerance, then the current Row is a single Row
                
            else:
                which_row.add(r)    # if difference is smaller than tolerance, then the current Row should be in the same Row of the existing one
        
        for i in pages.keys():  # sort the Rows of each page by their minimum y
            pages[i].sort(key = lambda x:x.min_val(),reverse = True)
    
        page_id += 1
    
    with open('c1.txt','w',encoding = 'utf-8')as newfile:   # save as txt for debugging
        for i in pages.keys():
            newfile.write("page id = " + str(i) + '\n')
            pages[i].sort(key = lambda x:x.min_val(),reverse = True)
            for j in pages[i]:
                newfile.write(str(j) + '\n')
    
    return pages


def split_column(x):
    clear_symbols = []  # clear all the unnecessary symbols
    for i in x:
        i = re.sub(r"[^a-zA-Z0-9,.* ]+",r"",i.strip())
        if i != "":
            clear_symbols.append(i)

    clear_dots = []     # clear all the dots
    for i in clear_symbols:
        for j in re.split("[•\-'·_.,]{3,}", i.strip()):
            if j.strip() != "" and not bool(re.match(r"^[•\-'·_., ]+$",j.strip())):
                clear_dots.append(j)

    clear_num = []      # clear the dollar sign
    for i in clear_dots:
        for j in i.strip().split('$'):
            if j.strip() != "":
                clear_num.append(j)


    clear_num2 = []     # split the number if there are 3 or more characters between ','
    for i in clear_num:
        w = []
        if isint(i):
            l = re.sub(r" ",r",",i.strip()).split(',')
            if len(l) >= 2:
                for j in l:
                    if j.strip() != "":
                        if len(j.strip()) > 3:
                            w.append(j[:3])
                            clear_num2.append(",".join(w))
                            w = [j[3:]]
                        else:
                            w.append(j)
                clear_num2.append(",".join(w))
            else:
                clear_num2.append(i.strip())
        else:
            clear_num2.append(i.strip())

    return clear_num2

def formatting_even(x):
    l1 = []     # for all the names (strings)
    l2 = []     # for all the numbers
    index = -1
    if len(x) != 0:
        while (len(x) > 0 and len(x[0]) <=2):   # delete the row number and other weird characters if exists
            del x[0]
        for i in range(len(x)):     # find the first number in a row (help to split names and numbers)
            if isint(x[i]):
                index = i
                l1 = x[:i]
                l2 = x[i:]
                break

        if index == -1:     # if there is no number in a row return an empty list
            return []

        while len(l1) < 3:      # align the first three columns as names
            l1.append("")

        l1 = l1[:3]

        while len(l2) < 3:      # align the even page in 6 columns
            l2.append("")

    return l1 + l2      # combine name and number lists in a row


def formatting_odd(x):
    l = []
    if len(x) >= 3:     # only return if the length of a row is larger than 3 and they are all numbers (check the first one or the last element)
        if isint(x[0]) or isint(x[-1]):
            return x
    return l


def isint(s):   # check whether a string is a number
    x1 = 0
    x2 = 0
    for i in s:
        if i.isnumeric():
            x1 += 1
        elif i.isalpha():
            x2 += 1
    if (x1 + x2)!= 0 and x1/(x1+x2) >= 0.5:
        return True
    else:
        return False