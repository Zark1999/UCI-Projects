# UCI-Projects

First-year Projects
  
	Map_API (Python)
		It uses a free map api from mapquestapi. It takes two input and generate a path from one place to another. Relevant details like latitude, distance and time are shown.
	
	Columns_Game (Python)
		Simple implementation of the Columns game. The UI is designed using the Pygame library.
   
   
Second-year Projects
  
	Maze (C++)
		Able to generate a m*n maze and able to solve a maze using iteration/backtracking.
  
	Word_Checker (C++)
		When the user types a word, find the possible words that the user is meaning if the original word is not found.
		Three different stroing and searching algorithms are written -- AVL set, Hash set and Skiplist set.
	
	Simple_Shell (C)
		Implementation of a simple linux shell. It can handle foreground, backgroud (&) and pipe (|).
		Fork is mainly used here.
	
	Memory Allocator (C)
		Implementation of malloc, realloc and free function in c;
	
	Thread (C)
		Count huge amount of data in the text file using thread, then show the result as a histogram. 
		Concurrent programming (thread), Lock (Mutex) are used.
	
	Pdf_TableExtraction (Python)
		Scan the pdf using OCR software, then utilize pdfminer.six library (from Github) to extract the coordinates of characters. Analyzing and rearranging the coordinates to form tables and store them as Excels.
		
	ICS_SearchEngine (Python)
		Scrap all links from ics.uci.edu and store them as local htmls (this is from the previous project which I did not upload here).
		Analyze the corpus provided and extracted/tokenized stems from the html, then stored the posting list (document ID with tf-idf) in file as database using Pandas.Tokenized the query and found corresponds posting list in database.
		Calculate the score utilizing Cosine Similarity and ranked the result.
		
		
Third-year Projects (updating...)

	Checkers_AI (Python)
		Utilize alpha-beta pruning as the basic game AI alogrithm.
		Add evaluation functions to make the program "smarter".
		Add asyncio lib to concurrently run 10 games at the same time (modified to 20 in the openlab(VM)).
		Over 90% win-rate against depth-4 alpha-beta pruning AI.
	
	AndroidStudio_AnimeList (Java, Android Studio, Firebase)
		Use three Class, acitivity and listAdapter to perform three pages, where a list view is shown on each page.
		First page shows category. Second page shows the Anime in that category. Third page shows the episodes of that anime.
		All data are saved in firebase Database. CRUD are implemented.
