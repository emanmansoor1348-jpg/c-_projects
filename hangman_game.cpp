#include <iostream>
#include <string>
#include <random>
using namespace std;
random_device rd;
mt19937 rng(rd());
int randomInt(int lo, int hi) {
    return uniform_int_distribution<int>(lo, hi)(rng);
}
class WordCategory {
protected:
    string words[10];
    int    count = 0;
private:
    static int totalCategoriesCreated;
public:
    WordCategory()          { totalCategoriesCreated++; }
    virtual ~WordCategory() {}

    virtual string getCategoryName() = 0;
    virtual string getHint()         = 0;

    string getRandomWord(int lastIndex, int& usedIndex) {
        int idx, tries = 0;
        do { idx = randomInt(0, count - 1); } while (idx == lastIndex && ++tries < 10);
        usedIndex = idx;
        return words[idx];
    }
    static int getTotalCategoriesCreated() { return totalCategoriesCreated; }
};
int WordCategory::totalCategoriesCreated = 0;
class ConcreteCategory : public WordCategory {
    string name, hint;
public:
    ConcreteCategory(string n, string h, string list[10]) : name(n), hint(h) {
        for (int i = 0; i < 10; i++) words[i] = list[i];
        count = 10;
    }
    string getCategoryName() override { return name; }
    string getHint()         override { return hint; }
};
class GameLogger;
class Gallows {
    int wrongGuesses, maxWrong;
    friend class GameLogger;
public:
    Gallows() : wrongGuesses(0), maxWrong(6) {}
    void addWrong()   { if (wrongGuesses < maxWrong) wrongGuesses++; }
    bool isGameOver() { return wrongGuesses >= maxWrong; }
    void reset()      { wrongGuesses = 0; }
    void draw() {
        cout << "\n  +--------+\n  |        |\n"
             << "  |        " << (wrongGuesses>=1?"O":" ") << "\n"
             << "  |       " << (wrongGuesses>=3?"/":" ") << (wrongGuesses>=2?"|":" ") << (wrongGuesses>=4?"\\":" ") << "\n"
             << "  |       " << (wrongGuesses>=5?"/":" ") << " " << (wrongGuesses>=6?"\\":" ") << "\n"
             << "  |\n  +---\n\n  Lives  : ";
        for (int i = 0; i < maxWrong; i++) cout << (i < maxWrong - wrongGuesses ? "[*]" : "[ ]");
        cout << "\n  Wrong  : " << wrongGuesses << " / " << maxWrong << "\n";
    }
};
class GameLogger {
    static int totalRoundsLogged;
public:
    void logRoundResult(Gallows& g, const string& word, bool won) {
        totalRoundsLogged++;
        cout << "\n  +---------------------------+"
             << "\n  |        ROUND SUMMARY      |"
             << "\n  +---------------------------+"
             << "\n  | Word      : " << word
             << "\n  | Result    : " << (won ? "WIN" : "LOSS")
             << "\n  | Wrong     : " << g.wrongGuesses << " / " << g.maxWrong
             << "\n  | Round No  : " << totalRoundsLogged
             << "\n  | Categories: " << WordCategory::getTotalCategoriesCreated()
             << "\n  +---------------------------+\n";
    }
    static int getTotalRoundsLogged() { return totalRoundsLogged; }
};
int GameLogger::totalRoundsLogged = 0;
class GuessTracker {
    char guessed[26];
    int  count = 0;
public:
    GuessTracker() : count(0) {}
    bool alreadyGuessed(char c) {
        for (int i = 0; i < count; i++) if (guessed[i] == c) return true;
        return false;
    }
    void addGuess(char c) { if (!alreadyGuessed(c) && count < 26) guessed[count++] = c; }
    bool isGuessed(char c) { return alreadyGuessed(c); }
    void reset()           { count = 0; }
    void showStatus(const string& secret) {
        cout << "  Wrong letters: ";
        bool any = false;
        for (int i = 0; i < count; i++)
            if (secret.find(guessed[i]) == string::npos) { cout << guessed[i] << " "; any = true; }
        cout << (any ? "\n" : "none\n");
    }
};
class WordDisplay {
    string        secretWord;
    GuessTracker& tracker;
public:
    WordDisplay(GuessTracker& t) : tracker(t) {}
    void   setWord(const string& w) { secretWord = w; }
    string getWord()                { return secretWord; }
    void show() {
        cout << "\n  Word   : ";
        for (char c : secretWord) cout << (tracker.isGuessed(c) ? string(" ")+c+" " : " _ ");
        cout << "\n  Letters: " << secretWord.size() << "\n";
    }
    bool isSolved() {
        for (char c : secretWord) if (!tracker.isGuessed(c)) return false;
        return true;
    }
};
class ScoreBoard {
    string playerName;
    int    wins = 0, losses = 0, totalGames = 0;
    static int totalPlayers;
public:
    ScoreBoard(const string& n) : playerName(n) { totalPlayers++; }
    void addWin()  { wins++;   totalGames++; }
    void addLoss() { losses++; totalGames++; }
    static int getTotalPlayers() { return totalPlayers; }
    void display() {
        cout << "\n  +---------------------------+"
             << "\n  |        SCOREBOARD         |"
             << "\n  +---------------------------+"
             << "\n  | Player : " << playerName
             << "\n  | Wins   : " << wins
             << "\n  | Losses : " << losses
             << "\n  | Games  : " << totalGames
             << "\n  | Players: " << totalPlayers
             << "\n  +---------------------------+\n";
    }
};
int ScoreBoard::totalPlayers = 0;
class Game {
    GuessTracker tracker;
    Gallows      gallows;
    WordDisplay  display;
    ScoreBoard*  score;
    GameLogger   logger;
    string       playerName;
    int          lastWordIndex = -1;
    static const int  CAT_COUNT = 5;
    string catNames[CAT_COUNT] = {"Animals","Technology","Countries","Sports","Food"};
    string catHints[CAT_COUNT] = {
        "Think of a wild or domestic animal!",
        "Think of a computer or tech related word!",
        "Think of a country name!",
        "Think of a sport or athletic activity!",
        "Think of a dish or food item!"
    };
    string catWords[CAT_COUNT][10] = {
        {"elephant","dolphin","crocodile","butterfly","penguin","giraffe","kangaroo","cheetah","flamingo","rhinoceros"},
        {"algorithm","compiler","database","keyboard","variable","function","software","hardware","internet","processor"},
        {"pakistan","australia","argentina","portugal","indonesia","malaysia","singapore","netherlands","switzerland","bangladesh"},
        {"cricket","football","basketball","volleyball","swimming","badminton","wrestling","athletics","baseball","gymnastics"},
        {"biryani","spaghetti","croissant","lasagna","dumpling","hummus","barbecue","pancake","burrito","cheesecake"}
    };
    WordCategory* chooseCategory() {
        int c = 0;
        while (true) {
            for (int i = 0; i < CAT_COUNT; i++) cout << "  " << i+1 << ". " << catNames[i] << "\n";
            cout << "  Choice: ";
            if (cin >> c && c >= 1 && c <= CAT_COUNT) { cin.ignore(1000,'\n'); break; }
            cin.clear(); cin.ignore(1000,'\n');
            cout << "  Invalid! Please enter 1 to " << CAT_COUNT << ".\n";
        }
        return new ConcreteCategory(catNames[c-1], catHints[c-1], catWords[c-1]);
    }
    bool isLetter(char c)    { return (c>='a'&&c<='z')||(c>='A'&&c<='Z'); }
    char toLower(char c)     { return (c>='A'&&c<='Z') ? c+32 : c; }
public:
    Game() : display(tracker), score(nullptr) {}
    ~Game() { delete score; }
    void run() {
        cout << "Enter name: ";
        cin >> playerName;
        cin.ignore(1000,'\n');
        score = new ScoreBoard(playerName);
        char again = 'y';
        while (toLower(again) == 'y') {
            WordCategory* cat = chooseCategory();
            int usedIndex = -1;
            string word = cat->getRandomWord(lastWordIndex, usedIndex);
            lastWordIndex = usedIndex;
            display.setWord(word);
            tracker.reset();
            gallows.reset();
            while (!gallows.isGameOver() && !display.isSolved()) {
                gallows.draw();
                display.show();
                cout << "  Hint: " << cat->getHint() << "\n";
                tracker.showStatus(word);
                cout << "\n  Enter letter: ";
                string line;
                getline(cin, line);
                if (line.size() != 1 || !isLetter(line[0])) {
                    cout << "  Invalid input! Please enter a single letter.\n";
                    continue;
                }
                char input = toLower(line[0]);
                if (tracker.alreadyGuessed(input)) { cout << "  Already guessed! Try another.\n"; continue; }
                tracker.addGuess(input);
                if (word.find(input) == string::npos) gallows.addWrong();
            }
            gallows.draw();
            bool won = display.isSolved();
            cout << "\n  " << (won ? "YOU WON!" : "GAME OVER!") << " Word was: " << word << "\n";
            won ? score->addWin() : score->addLoss();

            delete cat; cat = nullptr;
            logger.logRoundResult(gallows, word, won);
            score->display();

            cout << "\nPlay again? (y/n): ";
            string againLine;
            getline(cin, againLine);
            again = againLine.empty() ? 'n' : againLine[0];
        }
    }
};
int main() {
    Game game;
    game.run();
    return 0;
}