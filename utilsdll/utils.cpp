#include "pch.h"
#include "utils.h"
#include <fstream>


void save_score(int player_1_score, int player_2_score) {
    std::ofstream score_file("score.txt");
    if (score_file.is_open()) {
        score_file << player_1_score << " " << player_2_score;
        score_file.close();
    }
}

void load_score(int& player_1_score, int& player_2_score) {
    std::ifstream score_file("score.txt");
    if (score_file.is_open()) {
        score_file >> player_1_score >> player_2_score;
        score_file.close();
    }
    else {
        player_1_score = 0;
        player_2_score = 0;
    }
}

void save_highscore(int highscore) {
    std::ofstream highscore_file("highscore.txt");
    if (highscore_file.is_open()) {
        highscore_file << highscore;
        highscore_file.close();
    }
}

void load_highscore(int& highscore) {
    std::ifstream highscore_file("highscore.txt");
    if (highscore_file.is_open()) {
        highscore_file >> highscore;
        highscore_file.close();
    }
    else {
        highscore = 0;
    }
}

void update_highscore(int score_1, int score_2) {
    int highscore;
    load_highscore(highscore);
    if (score_1 > highscore) {
        highscore = score_1;
        save_highscore(highscore);
    }
    else if (score_2 > highscore) {
        highscore = score_2;
        save_highscore(highscore);
    }
}

inline int clamp(int min, int val, int max) {
    if (val < min) return min;
    if (val > max) return max;
    return val;
}