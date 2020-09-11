// WordChecker.hpp
//
// ICS 46 Winter 2019
// Project #3: Set the Controls for the Heart of the Sun
//
// The WordChecker class can check the spelling of single words and generate
// suggestions for words that have been misspelled.
//
// You can add anything you'd like to this class, but you will not be able
// to modify the declarations of any of its member functions, since the
// provided code calls into this class and expects it to look as originally
// given.
//
// You are permitted to use the C++ Standard Library in this class.

#ifndef WORDCHECKER_HPP
#define WORDCHECKER_HPP

#include <string>
#include <vector>
#include "Set.hpp"



class WordChecker
{
public:
    // The constructor requires a Set of words to be passed into it.  The
    // WordChecker will store a reference to a const Set, which it will use
    // whenever it needs to look up a word.
    WordChecker(const Set<std::string>& words);


    // wordExists() returns true if the given word is spelled correctly,
    // false otherwise.
    bool wordExists(const std::string& word) const;


    // findSuggestions() returns a vector containing suggested alternative
    // spellings for the given word, using the five algorithms described in
    // the project write-up.
    std::vector<std::string> findSuggestions(const std::string& word) const;


private:
    const Set<std::string>& words;
    std::vector<std::string> swapping_algorithm(std::vector<std::string> suggestions, const std::string& word) const;
    std::vector<std::string> insertion_algorithm(std::vector<std::string> suggestions, const std::string& word) const;
    std::vector<std::string> deletion_algorithm(std::vector<std::string> suggestions, const std::string& word) const;
    std::vector<std::string> replace_algorithm(std::vector<std::string> suggestions, const std::string& word) const;
    std::vector<std::string> splitting_algorithm(std::vector<std::string> suggestions, const std::string& word) const;
    std::string drop_const(const std::string& word) const;
    bool contain_helper(std::vector<std::string> suggestions, std::string word) const;
};



#endif // WORDCHECKER_HPP

