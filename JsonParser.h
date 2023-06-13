#pragma once
#include "JsonValidator.h"
#include "JsonNodeContainer.h"

class JsonParser {
private:
    JsonNodeContainer _startingNode;
    String _fileName;
    JsonNodeType _startingNodeType = JsonNodeType::JSON_OBJECT;
    bool _fileIsOpened = false;

    static JsonParser* instance;

    static void assertJsonFileName(const char* fileName);
    static void assertString(const char* str);
    void assertOpenFile() const;

    void create(const char* path, bool isAddressingStartingNode, bool createInArray, const char* newKey, const char* newStr);
    void move(const char* pathFrom, const char* pathTo, bool isAddressingStartingNode, bool moveInArray);

    JsonParser() = default;

public:
    JsonParser(const JsonParser& other) = delete;
    JsonParser& operator=(const JsonParser& other) = delete;

    static JsonParser* getInstance();
    static void freeInstance();

    void openFile(const char* fileName);
    void closeFile();

    void print() const;
    void search(const char* key) const;

    void set(const char* path, const char* newStr);
    void remove(const char* path);

    void createInArray(const char* path, const char* newStr);
    void createInStartingObject(const char* newKey, const char* newStr);
    void createInObject(const char* path, const char* newKey, const char* newStr);

    void moveBetweenArrays(const char* pathFrom, const char* pathTo);
    void moveToStartingObject(const char* pathFrom);
    void moveBetweenObjects(const char* pathFrom, const char* pathTo);
};
