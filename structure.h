#include <iostream>
#include <fstream>

using namespace std;

class Character;

struct partOfSpeechDataset {
    public:
        string getFullNameOf (string abbr) {
            if (abbr == "n") return "Noun";
            if (abbr == "adj") return "Adjective";
            if (abbr == "adv") return "Adverb";
            if (abbr == "v") return "Verb";
            return "";
        }
};

partOfSpeechDataset *pOFDataset = new partOfSpeechDataset();

class Charset {
    public:
        char key;
        Character *data;
        Charset *left, *right;
        Charset(char k, Character* value): key(k), data(value), left(NULL), right(NULL) {}

        ~Charset () {
            delete data;
            if (left != NULL) delete left;
            if (right != NULL) delete right;
        }
};

class CharsetTree {
    public:
        Charset* add (char key, Character* &value) {
            Charset *newNode = new Charset(key, value);
            if (data == NULL) return data = newNode;
            Charset *prev = NULL, *cur = data;
            while (cur != NULL) {
                if (key == cur->key) {
                    delete value;
                    delete newNode;
                    return cur;
                }
                prev = cur;
                if (key < cur->key) cur = cur->left;
                else cur = cur->right;
            }
            if (key < cur->key) return prev->left = newNode;
            return prev->right = newNode;
        }

        Charset* find (char key) {
            Charset *cur = data;
            while (cur != NULL) {
                if (key < cur->key) cur = cur->left;
                else if (key > cur->key) cur = cur->right;
                else break;
            }
            return cur;
        }

        ~CharsetTree () {
            if (data != NULL) delete data;
        }
    private:
        Charset *data = NULL;
};

class Character {
    public:
        Character *parent;
        CharsetTree* nextCharacters = new CharsetTree();
        bool isLastCharacter;
        string partOfSpeech = "", definition = "", topic = "";
        Character () {};
        Character (Character *parent, string pOF, string def, string tpc): parent(parent), partOfSpeech(pOF), definition(def), topic(tpc), isLastCharacter(true) {};
        Character (Character *parent): parent(parent), partOfSpeech(""), definition(""), topic(""), isLastCharacter(false) {};

        void display () {
            cout << "Part Of Speech: (" << partOfSpeech << ") " << pOFDataset->getFullNameOf(partOfSpeech) << endl;
            cout << "Definition: " << definition << endl;
            cout << "Topic/Field: " << topic << endl; 
        }

        Character* next (char f) {
            Charset* found = nextCharacters->find(f);
            if (found == NULL) return NULL;
            return found->data;
        }

        ~Character () {
            delete nextCharacters;
        }
};

class Dictionary {
    public:
        string name = "";

        void add (string word, string partOfSpeech, string definition, string topic) {
            Character *node = data;
            int len = word.length();
            for (int i = 0; i < len; ++i) {
                int key = word[i];
                Character* nextChar;
                if (i < len - 1) nextChar = new Character(node);
                else nextChar = new Character(node, partOfSpeech, definition, topic);
                node = node->nextCharacters->add(key, nextChar)->data;
            }
            node->isLastCharacter = true;
        }

        Character* find(string word) {
            Character *node = data;
            for (int i = 0; i < word.length(); ++i) {
                node = node->next(word[i]);
                if (node == NULL) return NULL;
            }
            if (!node->isLastCharacter) return NULL;
            return node;
        }

        bool wordExists (string word) {
            return find(word) != NULL;
        }

        void display (string word) {
            Character *result = find(word);
            if (result == NULL) cout << "No results found for the word '" << word << "'. Maybe you wish to add it as a custom word?";
            else {
                cout << "Definition of the word '" << word << "':" << endl;
                result->display();
            }
        }

        void import (string pathToFile) {

        }

        Dictionary (string namae): name(namae) {};

        ~Dictionary () {
            delete data;
        }
    private:
        Character *data = new Character(NULL);
};