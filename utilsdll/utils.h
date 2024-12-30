#ifndef UTILS_H
#define UTILS_H

#include <fstream>

// Export macros for DLL creation
#ifdef UTILS_EXPORTS
#define UTILS_API __declspec(dllexport)
#else
#define UTILS_API __declspec(dllimport)
#endif

// Function declarations
UTILS_API void save_score(int player_1_score, int player_2_score);
UTILS_API void load_score(int& player_1_score, int& player_2_score);
UTILS_API void save_highscore(int highscore);
UTILS_API void load_highscore(int& highscore);
UTILS_API void update_highscore(int score_1, int score_2);
UTILS_API int clamp(int min, int val, int max);

#endif // UTILS_H