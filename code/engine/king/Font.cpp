// (C) king.com Ltd 2021
#include "Font.h"

namespace King
{
Glyph Font[95] = {{' ', 0, 0, 6, 1, 0, 0, 8},     {'!', 1, 56, 4, 9, 0, 0, 6},   {'"', 6, 56, 5, 3, 0, 0, 7},
                  {'#', 12, 56, 6, 9, 0, 0, 8},   {'$', 19, 56, 6, 9, 0, 0, 8},  {'%', 26, 56, 6, 9, 0, 0, 8},
                  {'&', 33, 56, 6, 9, 0, 0, 8},   {'\'', 5, 66, 6, 9, 0, 0, 8},  {'(', 43, 56, 3, 9, 0, 0, 5},
                  {')', 47, 56, 3, 9, 0, 0, 5},   {'*', 51, 56, 6, 6, 0, 0, 8},  {'+', 58, 57, 6, 6, 0, 1, 8},
                  {',', 65, 63, 2, 3, 0, 7, 4},   {'-', 68, 59, 6, 1, 0, 3, 8},  {'.', 75, 63, 2, 2, 0, 7, 4},
                  {'/', 79, 56, 6, 9, 0, 0, 8},   {'0', 1, 46, 6, 9, 0, 0, 8},   {'1', 8, 46, 6, 9, 0, 0, 8},
                  {'2', 15, 46, 6, 9, 0, 0, 8},   {'3', 22, 46, 6, 9, 0, 0, 8},  {'4', 29, 46, 6, 9, 0, 0, 8},
                  {'5', 36, 46, 6, 9, 0, 0, 8},   {'6', 43, 46, 6, 9, 0, 0, 8},  {'7', 50, 46, 6, 9, 0, 0, 8},
                  {'8', 57, 46, 6, 9, 0, 0, 8},   {'9', 64, 46, 6, 9, 0, 0, 8},  {':', 86, 60, 2, 5, 0, 4, 4},
                  {';', 89, 60, 2, 6, 0, 4, 4},   {'<', 92, 56, 6, 8, 0, 1, 8},  {'=', 99, 58, 6, 4, 0, 3, 8},
                  {'>', 106, 56, 6, 8, 0, 1, 8},  {'?', 113, 55, 6, 9, 0, 0, 8}, {'@', 120, 55, 6, 9, 0, 0, 8},
                  {'A', 1, 1, 6, 9, 0, 0, 8},     {'B', 8, 1, 6, 9, 0, 0, 8},    {'C', 15, 1, 6, 9, 0, 0, 8},
                  {'D', 22, 1, 6, 9, 0, 0, 8},    {'E', 29, 1, 6, 9, 0, 0, 8},   {'F', 36, 1, 6, 9, 0, 0, 8},
                  {'G', 43, 1, 6, 9, 0, 0, 8},    {'H', 50, 1, 6, 9, 0, 0, 8},   {'I', 57, 1, 3, 9, 0, 0, 5},
                  {'J', 61, 1, 6, 9, 0, 0, 8},    {'K', 68, 1, 6, 9, 0, 0, 8},   {'L', 75, 1, 6, 9, 0, 0, 8},
                  {'M', 82, 1, 6, 9, 0, 0, 8},    {'N', 89, 1, 6, 9, 0, 0, 8},   {'O', 96, 1, 6, 9, 0, 0, 8},
                  {'P', 103, 1, 6, 9, 0, 0, 8},   {'Q', 110, 1, 6, 9, 0, 0, 8},  {'R', 117, 1, 6, 9, 0, 0, 8},
                  {'S', 1, 11, 6, 9, 0, 0, 8},    {'T', 8, 11, 6, 9, 0, 0, 8},   {'U', 15, 11, 6, 9, 0, 0, 8},
                  {'V', 22, 11, 6, 9, 0, 0, 8},   {'W', 29, 11, 6, 9, 0, 0, 8},  {'X', 36, 11, 6, 9, 0, 0, 8},
                  {'Y', 43, 11, 6, 9, 0, 0, 8},   {'Z', 50, 11, 6, 9, 0, 0, 8},  {'[', 1, 66, 3, 9, 0, 0, 8},
                  {'\\', 5, 66, 6, 9, 0, 0, 8},   {']', 12, 66, 3, 9, 0, 0, 8},  {'^', 16, 66, 6, 9, 0, 0, 8},
                  {'_', 23, 66, 6, 11, 0, 0, 8},  {'`', 30, 66, 2, 9, 0, 0, 8},  {'a', 1, 21, 6, 9, 0, 0, 8},
                  {'b', 8, 21, 6, 9, 0, 0, 8},    {'c', 15, 21, 6, 9, 0, 0, 8},  {'d', 22, 21, 6, 9, 0, 0, 8},
                  {'e', 29, 21, 6, 9, 0, 0, 8},   {'f', 36, 21, 6, 9, 0, 0, 8},  {'g', 42, 24, 6, 10, 0, 3, 8},
                  {'h', 49, 21, 6, 9, 0, 0, 8},   {'i', 56, 21, 3, 9, 0, 0, 5},  {'j', 60, 24, 6, 10, 0, 3, 8},
                  {'k', 67, 21, 6, 9, 0, 0, 8},   {'l', 74, 21, 3, 9, 0, 0, 5},  {'m', 78, 21, 7, 9, 0, 0, 9},
                  {'n', 86, 21, 6, 9, 0, 0, 8},   {'o', 92, 21, 6, 9, 0, 0, 8},  {'p', 99, 24, 6, 10, 0, 3, 8},
                  {'q', 106, 24, 6, 10, 0, 3, 8}, {'r', 113, 21, 4, 9, 0, 0, 6}, {'s', 118, 21, 6, 9, 0, 0, 8},
                  {'t', 1, 32, 5, 9, 0, 0, 7},    {'u', 7, 32, 5, 9, 0, 0, 7},   {'v', 13, 32, 6, 9, 0, 0, 8},
                  {'w', 20, 32, 6, 9, 0, 0, 8},   {'x', 27, 32, 5, 9, 0, 0, 7},  {'y', 33, 35, 6, 10, 0, 3, 8},
                  {'z', 40, 35, 5, 6, 0, 3, 7},   {'{', 33, 66, 3, 9, 0, 0, 5},  {'|', 37, 66, 1, 9, 0, 0, 3},
                  {'}', 39, 66, 3, 9, 0, 0, 5},   {'~', 43, 69, 5, 3, 0, 3, 8}};
}
