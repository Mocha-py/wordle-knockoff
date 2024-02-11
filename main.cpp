#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <Windows.h>
#include <algorithm>
#include <bits/stdc++.h>
#include <conio.h>
#include <random>

using namespace std;

const int ATTEMPTS = 6;

/**
 * @brief Checks whether the player has won and displays appropriate message.
 * 
 * @param won_var Boolean indicating whether the player has won.
 * @param h Handle to the console.
 * @param word Pointer to the word that was guessed.
 */
void check_won(bool won_var, HANDLE h, char* word) {
    if (won_var == true) {
        SetConsoleTextAttribute(h,10);
        cout << endl << "Congrats! You won! Thanks for playing!" << endl;
    } else {
        SetConsoleTextAttribute(h,4);
        cout << endl << "Sorry! You didn't guess the word!" << endl;
        cout << endl << "The word was:" << endl;
        SetConsoleTextAttribute(h,7);
        for (int i = 0; i<5; i++) {
            cout << *(word+i) << " ";
        }
    }
    cout << endl << endl << "Press ENTER to exit.";
    getch();
    return;
}

/**
 * @brief Generates a random word from a wordlist file.
 * 
 * @return Pointer to the generated word.
 */
char* generate_word() {
    vector<string> words;
    ifstream file("wordlist.txt");
    if (file.is_open()) {
        string word;
        while (getline(file, word)) {
            words.push_back(word);
        }
        file.close();
    } else {
        cout << "Unable to open file: wordlist.txt" << endl;
    }
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::mt19937 gen(seed);
    std::uniform_int_distribution<> distr(0, words.size() - 1);
    int randomIndex = distr(gen);
    static char true_word[5];
    strcpy(true_word, words[randomIndex].c_str());
    return true_word;
}

/**
 * @brief Main game loop where the player guesses the word.
 * 
 * @param word Pointer to the word to be guessed.
 * @param won_status Boolean indicating whether the player has won.
 * @param attempts Number of attempts remaining.
 * @param h Handle to the console.
 * @return Boolean indicating whether the player has won.
 */
bool game_loop(char* word, bool won_status, int attempts, HANDLE h) {
    vector<char> remaining_letters(word, word + 5);
    remaining_letters.resize(5);
    while (true) {
        for (int x = 0; x < 5; x++) {
        remaining_letters[x] = *(word+x);
        }
        bool needs_reset = false;
        string user_input;
        int correct = 0;
        cout << endl;
        cin >> user_input;
        cout << endl;
        vector<char> word_input(user_input.begin(), user_input.end());
        for (int i = 0; i < 5; i++) {
            if (isalpha(word_input[i]) == 0) {
                cout << "Your word contains invalid characters! Please try again!" << endl;
                needs_reset = true;
                break;
            } else {
                word_input[i] = tolower(word_input[i]);
            }
        }
        if (user_input.length() != 5) {
            cout << "Your word is not the correct length! Please try again!" << endl;
            continue;
        } else if (needs_reset == true) {
            continue;
        } else {
            attempts--;
            for (int i = 0; i<5; i++) {
                if (*(word+i) == word_input[i]) {
                    remaining_letters[i] = '0';
                }
            }
            for (int i = 0; i<5; i++) {
                if (*(word+i) == word_input[i]) {
                    correct++;
                    SetConsoleTextAttribute(h, 10);
                    cout << word_input[i] << " ";
                } else {
                    for (int x = 0; x < 5; x++) {
                        vector <char>::iterator it = find(remaining_letters.begin(), remaining_letters.end(), word_input[i]);
                        if (it != remaining_letters.end()) {
                            SetConsoleTextAttribute(h, 14);
                        } else {
                            SetConsoleTextAttribute(h, 4);
                        }
                    }
                    cout << word_input[i] << " ";
                }
                if (i == 4) {
                    SetConsoleTextAttribute(h, 7);
                    cout << endl;
                }
            }
            if (correct == 5) {
                won_status = true;
                break;
            } else if (attempts == 0) {
                break;
            } else {
                continue;
            }
        }
    }
    cout << endl << "----------------------------" << endl;
    return won_status;
}

/**
 * @brief Main function to run the game.
 * 
 * @return Integer indicating the success of the program.
 */
int main() {
    bool won = false;
    HANDLE h= GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(h,10);
    cout << " _ _ _ _____ _____ ____  __    _____ " << endl
    << "| | | |     | __  |    \\|  |  |   __|" << endl
    << "| | | |  |  |    -|  |  |  |__|   __|" << endl
    << "|_____|_____|__|__|____/|_____|_____|" << endl;
    SetConsoleTextAttribute(h,7);
    int user_attempts = ATTEMPTS;
    char* word_pointer = generate_word();
    won = game_loop(word_pointer, won, user_attempts, h);
    check_won(won, h, word_pointer);
    SetConsoleTextAttribute(h,7);
    return 1;
}