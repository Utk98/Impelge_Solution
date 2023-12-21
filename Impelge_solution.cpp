
#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_set>
#include <chrono>
using namespace std;
using namespace chrono;

class TrieNode {
public:
    TrieNode(char c) : value(c), isEndOfWord(false) {}

    char value;
    bool isEndOfWord;
    unordered_set<string> words;
    unordered_set<TrieNode*> children;

    TrieNode* findChild(char c) {
        for (auto child : children) {
            if (child->value == c) {
                return child;
            }
        }
        return nullptr;
    }
};


class Trie {
public:
    Trie() : root(new TrieNode('\0')) {}

    void insertWord(const string& word) {
        TrieNode* current = root;
        for (char c : word) {
            TrieNode* child = current->findChild(c);
            if (child == nullptr) {
                child = new TrieNode(c);
                current->children.insert(child);
            }
            current = child;
            current->words.insert(word);
        }
        current->isEndOfWord = true;
    }

    bool contain(const string& word) const {
        TrieNode* current = root;
        for (char c : word) {
            TrieNode* child = current->findChild(c);
            if (child == nullptr) {
                return false;
            }
            current = child;
        }
        return current->isEndOfWord;
    }

    vector<string> getAllPrefixes(const string& word) const {
        vector<string> prefixes;
        TrieNode* current = root;
        string prefix;
        for (char c : word) {
            TrieNode* child = current->findChild(c);
            if (child == nullptr) {
                break;
            }
            current = child;
            prefix += c;
            if (current->isEndOfWord) {
                prefixes.push_back(prefix);
            }
        }
        return prefixes;
    }

private:
    TrieNode* root;
};


struct WordSuffix {
    string word;
    string suffix;
};

int main() {
    
    string fileName="C:\\Users\\UTKARSH SINGH\\Input_02.txt";
    auto startTime = high_resolution_clock::now();
   
    Trie trie;

    string longestCompoundWord = "";
    string secondLongestCompoundWord = "";

  
    queue<WordSuffix> wordSuffixes;

    try {
        ifstream inputFile(fileName);
        string word;

        
        while (getline(inputFile, word)) {
            vector<string> prefixes = trie.getAllPrefixes(word);
            if (!prefixes.empty()) {
                for (const string& prefix : prefixes) {
                    wordSuffixes.push({word, word.substr(prefix.length())});
                }
            }
            trie.insertWord(word);
        }

      
        inputFile.close();
    } catch (exception& e) {
        cerr << "Error while reading file line by line: " << e.what() << endl;
        return 1;
    }
    
auto endTime = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(endTime - startTime);

    while (!wordSuffixes.empty()) {
    
        WordSuffix wordSuffix = wordSuffixes.front();
        wordSuffixes.pop();

        string word = wordSuffix.word;
        string suffix = wordSuffix.suffix;

        if (trie.contain(suffix)) {

            if (word.length() >= longestCompoundWord.length()) {
                secondLongestCompoundWord = longestCompoundWord;
                longestCompoundWord = word;
            }
        } else {
           
            vector<string> prefixes = trie.getAllPrefixes(suffix);

            
            if (!prefixes.empty()) {
                for (const string& prefix : prefixes) {
                    wordSuffixes.push({word, suffix.substr(prefix.length())});
                }
            }
        }
    }
  
    // Result
    cout << "Longest Compound Word: " << longestCompoundWord << endl;
    cout << "Second Longest Compound Word: " << secondLongestCompoundWord << endl;
    cout << "Time taken to open the file: " << duration.count() << " milliseconds" << endl;
    return 0;
}
