#include "Context.hpp"
#include "helpers.hpp"

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

    this->fonts.typingTestFont.size = 32;
    this->fonts.typingTestFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
            this->fonts.typingTestFont.size, nullptr, 0);
    this->fonts.titleFont.size = 40;
    this->fonts.titleFont.font = LoadFontEx("assets/fonts/LexendDeca-Regular.ttf",
            this->fonts.titleFont.size, nullptr, 0);
    this->fonts.tinyFont.size = 18;
    this->fonts.tinyFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
            this->fonts.tinyFont.size, nullptr, 0);
    this->fonts.bigFont.size = 90;
    this->fonts.bigFont.font = LoadFontEx("assets/fonts/JetBrainsMono-Regular.ttf",
            this->fonts.bigFont.size, nullptr, 0);

    WordList english200;
    english200.name = "English 200";
    getFileContent("assets/word_lists/english_200.txt", english200.words);
    this->wordsLists.push_back(english200);

    WordList english1k;
    english1k.name = "English 1k";
    getFileContent("assets/word_lists/english_1k.txt", english1k.words);
    this->wordsLists.push_back(english1k);

    WordList english5k;
    english5k.name = "English 5k";
    getFileContent("assets/word_lists/english_5k.txt", english5k.words);
    this->wordsLists.push_back(english5k);

    WordList english10k;
    english10k.name = "English 10k";
    getFileContent("assets/word_lists/english_10k.txt", english10k.words);
    this->wordsLists.push_back(english10k);

    WordList english25k;
    english25k.name = "English 25k";
    getFileContent("assets/word_lists/english_25k.txt", english25k.words);
    this->wordsLists.push_back(english25k);
}

void Context::unload() {
    UnloadFont(this->fonts.typingTestFont.font);
    UnloadFont(this->fonts.tinyFont.font);
    UnloadFont(this->fonts.bigFont.font);
}

