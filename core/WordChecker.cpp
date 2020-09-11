// WordChecker.cpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// Replace and/or augment the implementations below as needed to meet
// the requirements.

#include "WordChecker.hpp"

// The constructor requires a Set of words to be passed into it.  The
// WordChecker will store a reference to a const Set, which it will use
// whenever it needs to look up a word.
WordChecker::WordChecker(const Set<std::string>& words): words{words} {}

// wordExists() returns true if the given word is spelled correctly,
// false otherwise.
bool WordChecker::wordExists(const std::string& word) const {
	return words.contains(word);
}

std::string WordChecker::drop_const(const std::string& word) const{
	std::string w = word.substr(0, word.length());
	return w;
}

bool WordChecker::contain_helper(std::vector<std::string> suggestions, std::string word) const {
	for(int i=0; i < suggestions.size(); i++) {
		if(word == suggestions[i]) {
			return true;
		}
	}
	return false;
}

std::vector<std::string> WordChecker::swapping_algorithm(std::vector<std::string> suggestions, const std::string& word) const {
	int size = word.length();
	for(int i=0; i < size-1; i++) {
		std::string w = drop_const(word);
		std::swap(w[i], w[i+1]);
		if(WordChecker::wordExists(w) && contain_helper(suggestions, w) == false) {
			suggestions.push_back(w);
		}
	}
	return suggestions;
}

std::vector<std::string> WordChecker::insertion_algorithm(std::vector<std::string> suggestions, const std::string& word) const {
	std::string abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int size = word.length();
	for(int i=0; i <= size; i++) {
		for(int j=0; j < abc.length(); j++) {
			std::string w = drop_const(word);
			w.insert(w.begin()+i, abc[j]);
			if(WordChecker::wordExists(w) && contain_helper(suggestions, w) == false) {
				suggestions.push_back(w);
			}
		}
	}
	return suggestions;
}

std::vector<std::string> WordChecker::deletion_algorithm(std::vector<std::string> suggestions, const std::string& word) const {
	int size = word.length();
	for(int i=0; i < size; i++) {
		std::string w = drop_const(word);
		w.erase(w.begin()+i);
		if(WordChecker::wordExists(w) && contain_helper(suggestions, w) == false) {
			suggestions.push_back(w);
		}
	}
	return suggestions;
}

std::vector<std::string> WordChecker::replace_algorithm(std::vector<std::string> suggestions, const std::string& word) const {
	std::string abc = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
	int size = word.length();
	for(int i=0; i < size; i++) {
		for(int j=0; j < abc.length(); j++) {
			std::string w = drop_const(word);
			w.erase(w.begin()+i);
			w.insert(w.begin()+i, abc[j]);
			if(WordChecker::wordExists(w) && contain_helper(suggestions, w) == false) {
				suggestions.push_back(w);
			}
		}
	}
	return suggestions;
}

std::vector<std::string> WordChecker::splitting_algorithm(std::vector<std::string> suggestions, const std::string& word) const {
	int size = word.length();
	for(int i=1; i < size; i++) {
		std::string w = drop_const(word);
		std::string w1 = w.substr(0,i);
		std::string w2 = w.substr(i,size);
		if(WordChecker::wordExists(w1) && WordChecker::wordExists(w2)) { //} && contain_helper(suggestions, w1) == false && ) {
			if(contain_helper(suggestions, w1) == false) {
				suggestions.push_back(w1);
			}
			if(contain_helper(suggestions, w2) == false) {
				suggestions.push_back(w2);
			}
		}
	}
	return suggestions;
}

// findSuggestions() returns a vector containing suggested alternative
// spellings for the given word, using the five algorithms described in
// the project write-up.
std::vector<std::string> WordChecker::findSuggestions(const std::string& word) const {
	std::vector<std::string> suggestions = {};
	suggestions = swapping_algorithm(suggestions, word);
	suggestions = insertion_algorithm(suggestions, word);
	suggestions = deletion_algorithm(suggestions, word);
	suggestions = replace_algorithm(suggestions, word);
	suggestions = splitting_algorithm(suggestions, word);
	return suggestions;
}

