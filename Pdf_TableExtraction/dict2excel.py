from split_row import *
from openpyxl import Workbook
from pathlib import Path
import os

pdf_folder = Path("/Users/zirongxu/Desktop/Project/test/brian/national_banks/document/")    # the file path of all the pdf
xml_folder = Path("/Users/zirongxu/Desktop/Project/test/brian/national_banks/xml2/")    # the file path of all the xml

# for pdf in pdf_folder.iterdir():  # transfer all the pdf to xml
#     os.system("/Library/Frameworks/Python.framework/Versions/3.6/bin/pdf2txt.py -t xml -o " + str(xml_folder) + "/" + pdf.stem + ".txt " + str(pdf))
#     print(pdf)


for xml in xml_folder.iterdir():    # iterate through the all the xml files
    wb = Workbook()     # create a wordbook (excel)
    xml_filename = xml

    state = "NULL"  # initialize the state and district
    district = "NULL"

    if xml.stem == "1929":
        try:
            pages = xml2dict(xml_filename)  # {page_id:[row,...],...}
        except:
            print(xml_filename)     # for debugging
            continue

        del wb["Sheet"]     # delete the first sheet

        l_even = []     # list of rows for even page
        l_odd = []      # list of rows for odd page

        for page_num in pages.keys():
            if page_num < 4 or page_num % 2 == 1:
                wb.create_sheet("page id" + str(page_num))      # create a sheet
                s = wb["page id" + str(page_num)]       # load into the sheet just created

            page = pages[page_num]  # a list of Rows

            row = 0

            if page_num >= 4 and page_num % 2 == 1:
                s.append(['State','District','Location and name of bank','President','Cashier','Loans and discounts, including overdrafts',
                          'United states government securities owned','Otherbonds, stocks, and securities, etc., owned',
                          'Cash and exchange, including reserve with Federal reserve bank', 'Other assets',
                          'Total resources', 'Capital', 'Surplus', 'Undivided profit', 'Circulation',
                          'Total deposits', 'Bills payable and rediscounts', 'Other liahilities'])

            while (row != len(page)):
                if len(page[row].toList()) != 0:
                    x = page[row].toList()      # transfer a row to a list of strings

                    if len(x) != 0:

                        if len(x) > 3:      # if there are more than 3 elements in one row and the coordinates between two of the elements are larger than 35, add stars to show that one column is empty.
                            new_x = []
                            for i in range(len(page[row].text)-1):
                                if (page[row].text[i+1].min - page[row].text[i].max > 35) and isint(str(page[row].text[i])):
                                    new_x.append(x[i])
                                    new_x.append("********")
                                else:
                                    new_x.append(x[i])
                            new_x.append(x[-1])
                            x = new_x

                        if len(x) <= 2:     # if there are less or equal to 2 words in a row, check whether the row is state
                            curr_x = [re.sub(r" ",r"","".join(x))]
                            if curr_x[0].split('—')[0].strip().isupper() and len(curr_x[0]) > 3 and curr_x[0].split('—')[0].strip().isalpha() \
                                    and not curr_x[0].lower().startswith("district"):   # check whether the row is just state name
                                state = curr_x[0].split('—')[0].strip()
                                if row != len(page) - 1:
                                    y = page[row + 1].toList()
                                    if len(y) >= 1 and y[0].lower().startswith("district"):     # check whether the next row contains the district name
                                        district = "".join(y).split('—')[0]
                                        # print(page_num,state,district)
                                        row += 2
                                        continue
                                row += 1
                                continue


                        if x[0].lower().startswith("district"):     # check whether the current row is district name (without state name)
                            district = "".join(x).split('—')[0]     # only change the district name
                            row += 1
                            continue



                        if row != len(page) - 1 and len(page[row+1].toList()) == 1 and not page[row+1].toList()[0].split('—')[0].strip().isupper():
                            """combine two rows"""
                            if row == len(page)-2 or (row < len(page) -2 and len(page[row+2].toList()) != 1):
                                # if there is only one word in the next row and it is not state or district, combine the next row with this row
                                page[row].append_text(page[row+1].text[0])
                                x = page[row].toList()
                                row += 1


                        if page_num >= 4 and len(page) >= 5:
                            l = split_column(x)     # clean all the unnecessary characters and split words in a row

                            if page_num % 2 == 0:
                                l = formatting_even(l)
                            else:
                                l = formatting_odd(l)

                            if len(l) == 0:
                                row += 1
                                continue

                            if page_num % 2 == 0:
                                # print(page_num,state,district)
                                l_even.append([state,district] + l)
                            else:
                                l_odd.append(l)

                        else:
                            l = split_column(x)
                            if len(l) == 0:
                                row += 1
                                continue
                            try:
                                s.append(l)
                            except:
                                print(page_num)

                row += 1

            if page_num % 2 == 1 and page_num >= 4:     # combine even and odd page row by rowde

                min_len = min(len(l_even),len(l_odd))
                for i in range(min_len):
                    s.append(l_even[i]+l_odd[i])

                if len(l_even) != len(l_odd):
                    print(xml.stem,page_num)
                    if len(l_even) > len(l_odd):
                        for i in range(min_len,len(l_even)):
                            s.append(l_even[i])
                    else:
                        for i in range(min_len, len(l_odd)):
                            s.append([""]*8 + l_odd[i])


                l_even = []
                l_odd = []

        wb.save(xml.stem+".xlsx")
