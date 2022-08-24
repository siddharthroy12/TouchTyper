#ifndef HELPERS_HPP
#define HELPERS_HPP
#include "Context.hpp"
#include "../libs/raylib/src/raylib.h"
#include <string>
#include <vector>
#include <fstream>
#include <iostream>

Vector2 getCenter(int width, int height);

void drawMonospaceText(Font font, std::string text, Vector2 position, float fontSize, Color color);

double getTimeInMin();

void restartTest(Context &context);

void endTest(Context &context);

bool getFileContent(std::string fileName, std::vector<std::string> & vecOfStrs);

std::string generateSentence(Context &context, int words);

#endif
