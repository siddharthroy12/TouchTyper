#include <algorithm>
#include "Context.hpp"
#include "helpers.hpp"
#include "../libs/raylib/src/raylib.h"

void Context::load() {
    Theme arch;
    arch.name = "Arch";
    arch.background = {12, 13, 17, 255};
    arch.text = {92, 96, 133, 255};
    arch.cursor = {153, 214, 234, 255};
    arch.wrong = RED;
    arch.correct = {126, 186, 181, 255};
    arch.highlight = RAYWHITE;
    this->themes.push_back(arch);

    Theme black;
    black.name = "Black";
    black.background = {17, 17, 17, 255};
    black.text = {96, 96, 96, 255};
    black.cursor = {238, 238, 238, 255};
    black.wrong = {218, 51, 51, 255};
    black.correct = black.cursor;
    black.highlight = black.cursor;
    this->themes.push_back(black);

    Theme white;
    white.name = "White";
    white.background = {197, 200, 171, 255};
    white.text = {69, 68, 56, 255};
    white.cursor = {172, 175, 148, 255};
    white.wrong = {209, 97, 67, 255};
    white.correct = black.background;
    white.highlight = black.background;

    this->themes.push_back(white);

    std::string base = GetApplicationDirectory();

    this->fonts.typingTestFont.size = 32;
    this->fonts.typingTestFont.font = LoadFontEx((base+"assets/fonts/JetBrainsMono-Regular.ttf").c_str(),
            this->fonts.typingTestFont.size, nullptr, 0);
    this->fonts.titleFont.size = 40;
    this->fonts.titleFont.font = LoadFontEx((base+"assets/fonts/LexendDeca-Regular.ttf").c_str(),
            this->fonts.titleFont.size, nullptr, 0);
    this->fonts.tinyFont.size = 18;
    this->fonts.tinyFont.font = LoadFontEx((base+"assets/fonts/JetBrainsMono-Regular.ttf").c_str(),
            this->fonts.tinyFont.size, nullptr, 0);
    this->fonts.bigFont.size = 90;
    this->fonts.bigFont.font = LoadFontEx((base+"assets/fonts/JetBrainsMono-Regular.ttf").c_str(),
            this->fonts.bigFont.size, nullptr, 0);

    int numberOfFiles;
    char **files = GetDirectoryFiles((base+"assets/word_lists/").c_str(), &numberOfFiles);

    for (int i = numberOfFiles-1; i > -1; i--) {
        if (files[i][0] != '.') {
            WordList wordList;
            std::string name = files[i];
            name.replace(name.find(".txt"), sizeof(".txt") - 1, "");
            name.replace(name.find("_"), sizeof("_") - 1, " ");
            name[0] = toupper(name[0]);
            wordList.name = name;
            getFileContent((base+"assets/word_lists/"+files[i]).c_str(), wordList.words);
            this->wordsLists.push_back(wordList);
        }
    }

    this->selectedTheme = loadStorageValue(0, 0);
    this->selectedWordList = loadStorageValue(1, -1);
    if (this->selectedWordList == -1) {
        int index;

        for (int i = 0; i < this->wordsLists.size(); i++) {
            if (this->wordsLists[i].name == "English 200") {
                index = i;
                break;
            }
        }
        this->selectedWordList = index;
    }
    this->testSettings.selectedAmount = loadStorageValue(2, 1);
    this->testSettings.usePunctuation = loadStorageValue(3, 0);
    this->testSettings.useNumbers = loadStorageValue(4, 0);
    this->testSettings.testMode = (TestMode)loadStorageValue(5, 0);
}

void Context::unload() {
    UnloadFont(this->fonts.typingTestFont.font);
    UnloadFont(this->fonts.tinyFont.font);
    UnloadFont(this->fonts.bigFont.font);

    saveStorageValue(0, this->selectedTheme);
    saveStorageValue(1, this->selectedWordList);
    saveStorageValue(2, this->testSettings.selectedAmount);
    saveStorageValue(3, this->testSettings.usePunctuation);
    saveStorageValue(4, this->testSettings.useNumbers);
    saveStorageValue(5, (int)this->testSettings.testMode);
}

