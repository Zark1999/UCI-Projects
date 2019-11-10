// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"



WordChecker::WordChecker(const Set<std::string>& words)
    : words{words}
{
}


bool WordChecker::wordExists(const std::string& word) const
{
    return words.contains(word);
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
	std::vector<std::string> result;
	std::vector<std::string> v1 = swapping(word);
	std::vector<std::string> v2 = inserting(word);
	std::vector<std::string> v3 = deleting(word);
	std::vector<std::string> v4 = replacing(word);
	std::vector<std::string> v5 = spliting(word);
	
	for(unsigned int i=0; i<v1.size(); i++)
	{if(not contains(result,v1[i])){result.push_back(v1[i]);}}
	for(unsigned int i=0; i<v2.size(); i++)
	{if(not contains(result,v2[i])){result.push_back(v2[i]);}}
	for(unsigned int i=0; i<v3.size(); i++)
	{if(not contains(result,v3[i])){result.push_back(v3[i]);}}
	for(unsigned int i=0; i<v4.size(); i++)
	{if(not contains(result,v4[i])){result.push_back(v4[i]);}}
	for(unsigned int i=0; i<v5.size(); i++)
	{if(not contains(result,v5[i])){result.push_back(v5[i]);}}
	
    return result;
}

std::vector<std::string> WordChecker::swapping(const std::string& word) const
{
	std::vector<std::string> s;
	for (unsigned int i=0; i<word.size()-1; i++)
	{
		std::string x = word;
		std::swap(x[i],x[i+1]);
		if (words.contains(x))
		{
			s.push_back(x);
		}
	}
	return s;
}

std::vector<std::string> WordChecker::inserting(const std::string& word) const
{
	std::vector<std::string> s;
	std::string w = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (unsigned int i=0; i<word.size()+1; i++)
	{
		for (unsigned int j=0; j<26; j++)
		{
			std::string x = word;
			x.insert(x.begin()+i,w[j]);
			if (words.contains(x))
			{
				s.push_back(x);
			}
		}
	}
	return s;
}

std::vector<std::string> WordChecker::deleting(const std::string& word) const
{
	std::vector<std::string> s;
	for (unsigned int i=0; i<word.size(); i++)
	{
		std::string x = word;
		x.erase(x.begin()+i);
		if (words.contains(x))
		{
			s.push_back(x);
		}
	}
	return s;
}

std::vector<std::string> WordChecker::replacing(const std::string& word) const
{
	std::vector<std::string> s;
	std::string w = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	for (unsigned int i=0; i<word.size(); i++)
	{
		for (unsigned int j=0; j<26; j++)
		{
			std::string x = word;
			x.replace(i,1,1,w[j]);
			if (words.contains(x))
			{
				s.push_back(x);
			}
		}
	}
	return s;
}

std::vector<std::string> WordChecker::spliting(const std::string& word) const
{
	std::vector<std::string> s;
	for (unsigned int i=1; i<word.size(); i++)
	{
		std::string x = word;
		if (words.contains(x.substr(0,i)) and words.contains(x.substr(i)))
		{
			x.insert(i," ");
			s.push_back(x);
		}
	}
	return s;
}

bool WordChecker::contains(std::vector<std::string> s, std::string word) const
{
	for (unsigned int i=0; i<s.size(); i++)
	{
		if (s[i] == word)
		{
			return true;
		}
	}
	return false;
}
