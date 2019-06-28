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
    if (words.contains(word)){

    	return true;
    }
    return false;
}


std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const
{
    std::vector<std::string> result{};
    swapAdj(word, result);
    inBetween(word, result);
    delChar(word, result);
    replaceChar(word, result);
    splitChar(word, result);
    // std::cout<<result.size()<<std::endl;
    return result;
	// return std::vector<std::string>{};
}


bool WordChecker::found(const std::vector<std::string> sset, const std::string& targ) const{
	return std::find(sset.begin(), sset.end(), targ) != sset.end();
}

void WordChecker::swapAdj(const std::string& word, std::vector<std::string>& result) const{
	for (int i = 0; i < word.size()-1; ++i){
		// std::cout << "HI" << std::endl;
		std::string temp = word;
		std::swap(temp[i], temp[i+1]);

		if (wordExists(temp) and not found(result, temp)){
			result.push_back(temp);
		}
	}
}

void WordChecker::inBetween(const std::string& word, std::vector<std::string>& result) const{
	for (int ascii = 65; ascii < 91; ++ascii){
		for (int i = 0; i<word.size(); i++){
			std::string temp = word;
			temp.insert(i, 1, char(ascii));
			// std::cout << temp << std::endl;
			if (wordExists(temp) and not found(result, temp)){
				result.push_back(temp);
			}
		}
		std::string temp = word;
		temp.append(1, char(ascii));
		// std::cout << temp << std::endl;
		if (wordExists(temp) and not found(result, temp)){
			result.push_back(temp);
		}

	}
}
void WordChecker::delChar(const std::string& word, std::vector<std::string>& result) const{
	for (int i = 0; i < word.size(); i++){
		std::string temp = word;
		temp.erase(i, 1);
		// std::cout<<temp<<std::endl;
		if (wordExists(temp) and not found(result, temp)){
			result.push_back(temp);
		}
	}
}

void WordChecker::replaceChar(const std::string& word, std::vector<std::string>& result) const{
	for (int ascii = 65; ascii < 91; ++ascii){
		for (int i = 0; i<word.size(); i++){
			std::string temp = word;
			temp.replace(i, 1, 1, char(ascii));
			// std::cout<<temp<<std::endl;
			if (wordExists(temp) and not found(result, temp)){
				result.push_back(temp);
			}
		}
	}
}
void WordChecker::splitChar(const std::string& word, std::vector<std::string>& result) const{
	for (int i = 1; i < word.size(); i ++){
		std::string left = word.substr(0, i);
		std::string right = word.substr(i);
		// std::cout << "left: " << left << "; right: " << right <<";" <<std::endl;
		if (wordExists(left) and wordExists(right)){
			result.push_back(left + " " + right);
		}
	}
}