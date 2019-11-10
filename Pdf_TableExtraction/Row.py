from section import Section
import re

class Row:
    width_diff_tolerance = 3
    
    def __init__(self,min_y,max_y,textline):    # initialize the class, takes minimum and maximum y coordinates and a list of text as arguments.
        self.min = float(min_y)
        self.max = float(max_y)
        self.text_list = textline               # initially a list of text (e.g. [<text font="Times-Bold" bbox="164.400,546.156,172.324,559.992" colourspace="DeviceGray" ncolour="None" size="13.836">T</text>])
        self.text = []                          # a list of class Section
        
        self._add_text(textline)                # add the text_list to self.text as Sections
            
    def __str__(self):
        return str([str(i) for i in self.text]) # allow print (hardly use)

    def min_val(self):
        return self.min     # for sorting
    
    def textline(self):
        return self.text_list
    
    def length(self):
        return len(self.text)   # return how many Sections (words) in a Row (line)
    
    def toList(self):
        return [str(i) for i in self.text]      # return a list of words
    
    def add(self,other):
        self._add_text(other.textline())        # takes another Row class as input and add its textline to the current text
        
    def _add_text(self,textline):      # saves the content in textline to text as a list of Sections.
        previous_x_max = -1
        min_x = 0
        max_x = 0
        word = ''
        for character in textline:      # iterate through the textline list, every iteration is <text font="Times-Bold" bbox="164.400,546.156,172.324,559.992" colourspace="DeviceGray" ncolour="None" size="13.836">T</text>
            try:    # in case empty text
                coords = character.attrib['bbox'].split(',')    # get the coordinates
                if previous_x_max == -1:        # if previous_x_max is -1, it means that the current text list is empty
                    previous_x_max = float(coords[2])   # set the previous_x_max
                    min_x = float(coords[0])    # update accordingly
                    max_x = float(coords[2])
                    word += character.text      # add the first character in the word
                    continue    # continue to the next loop since it is the first character in the list

                x_coord_min = float(coords[0])
                x_coord_diff = abs(x_coord_min-previous_x_max)

                if x_coord_diff >= Row.width_diff_tolerance:    # check whether the difference exceeds the tolerance
                    if word != '':      # if the word is not empty, add new Section to the text list
                        self.text.append(Section(min_x,max_x,word))
                    previous_x_max = float(coords[2])   # update the previous_x_max
                    min_x = float(coords[0])
                    max_x = float(coords[2])
                    word = character.text   # update word

                else:
                    word += character.text      # difference less than tolerance means the word hasn't finished yet
                    max_x = float(coords[2])
                    previous_x_max = float(coords[2])
            except:
                pass

        if word != '':  # after iteration add the word in the text list as a Section
            self.text.append(Section(min_x,max_x,word))
        
        self.text.sort(key=lambda x:x.min_val())    # sort the text list by the min value of Section

    def append_text(self,r):    # this function is used for adding one Section to a Section in the text list based on their min difference
        min_x = r.min
        which_sec = 0
        min_diff = 100
        for i in range(len(self.text)):
            if abs(self.text[i].min - min_x) < min_diff:
                min_diff = abs(self.text[i].min - min_x)
                which_sec = i

        self.text[which_sec].add(" " + r.text)
