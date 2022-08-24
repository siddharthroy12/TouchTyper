#include "helpers.hpp"
#include <cctype>

Vector2 getCenter(int width, int height) {
    Vector2 result;
    result.x = width/2.0;
    result.y = height/2.0;
    return result;
}

void drawMonospaceText(Font font, std::string text, Vector2 position, float fontSize, Color color) {
    Vector2 sizeOfCharacter = MeasureTextEx(font, "a", fontSize, 1);

    for (auto letter : text) {
        std::string c(1, letter);

        DrawTextEx(font, c.c_str(), position, fontSize, 1, color);
        position.x += sizeOfCharacter.x;
    }
}

double getTimeInMin() {
    return GetTime()/60;
}

template<class BidiIter >
BidiIter random_unique(BidiIter begin, BidiIter end, size_t num_random) {
    size_t left = std::distance(begin, end);
    while (num_random--) {
        BidiIter r = begin;
        std::advance(r, rand()%left);
        std::swap(*begin, *r);
        ++begin;
        --left;
    }
    return begin;
}

bool previousWasPeriod = false;

std::string generateSentence(Context &context, int words) {
    std::string output = "";

    // Shuffle the amount of words we need
    random_unique(context.words.begin(), context.words.end(), words);

    // Put the words in the senctence
    for(int i = 0; i < words; ++i) {
        std::string word = context.words[i];

        if (previousWasPeriod) {
            word[0] = toupper(word[0]);
        }

        output += word;

        previousWasPeriod = false;

        // Put . or , randomly
        if (GetRandomValue(0, 10) > 8 && context.testSettings.usePunctuation) {
            if (GetRandomValue(0, 10) >= 5) {
                output.push_back(',');
            } else {
                output.push_back('.');
                previousWasPeriod = true;
            }
        }

        output += ' ';
    }

    output.pop_back();

    return output;

}

void restartTest(Context &context) {
    int amount = context.testSettings.testModeAmounts[context.testSettings.selectedAmount];

    // If time mode is set put only 50 words after than it will be incremented as we type
    if (context.testSettings.testMode == TestMode::TIME) {
        amount = 50;
    }

    context.sentence = generateSentence(context, amount);

    if (context.testSettings.usePunctuation) {
        context.sentence[0] = toupper(context.sentence[0]);
        if (context.testSettings.testMode == TestMode::WORDS) {
            context.sentence += '.';
        }
    }

    context.input = "";
    context.currentScreen = Screen::TEST;
    context.wpm = 0;
    context.cpm = 0;
    context.accuracy = 0;
    context.raw = 0;
    context.testRunning = false;
}

void endTest(Context &context) {
    context.testRunning = false;
    context.currentScreen = Screen::RESULT;
    context.testEndTime = getTimeInMin();
}

bool getFileContent(std::string fileName, std::vector<std::string> & vecOfStrs) {
    // Open the File
    std::ifstream in(fileName.c_str());

    // Check if object is valid
    if(!in) {
        std::cerr << "Cannot open the File : " << fileName << std::endl;
        return false;
    }

    std::string str;
    // Read the next line from File untill it reaches the end.
    while (std::getline(in, str)) {
        // Line contains string of length > 0 then save it in vector
        if(str.size() > 0)
            vecOfStrs.push_back(str);
    }

    //Close The File
    in.close();
    return true;
}