class Section:
    def __init__(self,min_x,max_x,text):
        self.min = min_x
        self.max = max_x
        self.text = text
        
    def __str__(self):
        return self.text

    def add(self,t):
        self.text += t
    
    def min_val(self):
        return self.min