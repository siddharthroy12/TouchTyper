#include "helpers.hpp"
#include <cctype>
#include <unordered_map>

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

template<class BidiIter>
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

bool useCaplitalNext = false;
bool previousWasDash = false;

char quotes[3][2] = {
    {'\'', '\''},
    {'\"', '\"'},
    {'(', ')'}
};

std::unordered_map<std::string, std::string> punctuations = {
  {"are", "aren't"},
  {"can", "can't"},
  {"could", "couldn't"},
  {"did", "didn't"},
  {"does", "doesn't"},
  {"do", "don't"},
  {"had", "hadn't"},
  {"has", "hasn't"},
  {"have", "haven't"},
  {"is", "isn't"},
  {"must", "mustn't"},
  {"should", "shouldn't"},
  {"was", "wasn't"},
  {"were", "weren't"},
  {"will", "won't"},
  {"would", "wouldn't"}
};

std::string generateSentence(Context &context, int numberOfWords) {
    auto words = context.wordsLists[context.selectedWordList].words;
    std::string output = "";

    // Shuffle the amount of words we need
    random_unique(words.begin(), words.end(), numberOfWords);

    // Put the words in the senctence
    for(int i = 0; i < numberOfWords; ++i) {
        std::string word = words[i];

        if (context.testSettings.usePunctuation) {
            if (punctuations.find(word) != punctuations.end()) {
                word = punctuations[word];
            }
        }

        bool inQuotes = context.testSettings.usePunctuation && (GetRandomValue(0, 10) == 10);
        bool itsDashTime = context.testSettings.usePunctuation && (GetRandomValue(0, 10) == 10) && !previousWasDash && !useCaplitalNext;
        bool itsNumber = context.testSettings.useNumbers && (GetRandomValue(0, 10) == 10);

        if (useCaplitalNext) {
            word[0] = toupper(word[0]);
        }

        if (inQuotes) {
            int quote = GetRandomValue(0, 2);
            output.push_back(quotes[quote][0]);
            word += quotes[quote][1];
        } else if(itsDashTime) {
            word = "-";
        }

        if (itsNumber) {
            word = std::to_string(GetRandomValue(0, 1000));
        }

        output += word;

        useCaplitalNext = false;

        // Put . or , randomly
        if (GetRandomValue(0, 10) > 8 &&
                context.testSettings.usePunctuation &&
                !previousWasDash &&
                !itsDashTime &&
                !inQuotes) {
            switch (GetRandomValue(0, 3)) {
                case 0:
                    output.push_back(',');
                    break;
                case 1:
                    output.push_back('.');
                    useCaplitalNext = true;
                    break;
                case 2:
                    output.push_back('!');
                    useCaplitalNext = true;
                    break;
                case 3:
                    output.push_back('?');
                    useCaplitalNext = true;
                    break;
            }
        }

        previousWasDash = itsDashTime;

        output += ' ';
    }

    output.pop_back();

    return output;
}

void restartTest(Context &context, bool repeat) {
    if (!repeat) {
        int amount = context.testSettings.testModeAmounts[context.testSettings.selectedAmount];

        // If time mode is set put only 50 words after than it will be incremented as we type
        if (context.testSettings.testMode == TestMode::TIME) {
            amount = 120;
        }

        context.sentence = generateSentence(context, amount);

        if (context.testSettings.usePunctuation) {
            context.sentence[0] = toupper(context.sentence[0]);
            if (context.testSettings.testMode == TestMode::WORDS && !useCaplitalNext) {
                context.sentence += '.';
            }
        }
    }

    context.input = "";
    context.currentScreen = Screen::TEST;
    context.wpm = 0;
    context.cpm = 0;
    context.accuracy = 0;
    context.raw = 0;
    context.testRunning = false;
    context.correctLetters = 0;
    context.incorrecLetters = 0;
    context.furthestVisitedIndex = -1;
}

void endTest(Context &context) {
    context.testRunning = false;
    context.currentScreen = Screen::RESULT;
    context.testEndTime = GetTime();
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

// Save integer value to storage file (to defined position)
// NOTE: Storage positions is directly related to file memory layout (4 bytes each integer)
bool saveStorageValue(unsigned int position, int value) {
    bool success = false;
    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    const char *filePath = TextFormat("%s%s", GetApplicationDirectory(), STORAGE_DATA_FILE);
    unsigned char *fileData = LoadFileData(filePath, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL) {
        if (dataSize <= (position*sizeof(int))) {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL) {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            } else {
                // RL_REALLOC failed
                TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", filePath, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        } else {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(filePath, newFileData, newDataSize);
        RL_FREE(newFileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", filePath, value);
    } else {
        TraceLog(LOG_INFO, "FILEIO: [%s] File created successfully", filePath);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(filePath, fileData, dataSize);
        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", filePath, value);
    }

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int loadStorageValue(unsigned int position, int defaultValue) {
    int value = defaultValue;
    unsigned int dataSize = 0;
    const char *filePath = TextFormat("%s%s", GetApplicationDirectory(), STORAGE_DATA_FILE);
    unsigned char *fileData = LoadFileData(filePath, &dataSize);

    if (fileData != NULL) {
        if (dataSize < (position*4)) {
            TraceLog(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", filePath, position);
        } else {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TraceLog(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", filePath, value);
    }

    return value;
}
