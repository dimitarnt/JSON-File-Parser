#pragma once
#include "JsonValidator.h"
#include "JsonNodeContainer.h"

//Singleton parser class
class JsonParser {
private:
    JsonNodeContainer _startingNode;
    String _fileName;
    JsonNodeType _startingNodeType = JsonNodeType::JSON_OBJECT;
    bool _fileIsOpened = false;
    bool _changesHaveBeenMade = false;

    static JsonParser* instance;

    static void assertJsonFileName(const char* fileName);
    static void assertString(const char* str);
    void assertOpenFile() const;

    void create(const char* path, bool isAddressingStartingNode, bool createInArray, const char* newKey, const char* newStr);
    void move(const char* pathFrom, const char* pathTo, bool isAddressingStartingNode, bool moveInArray);
    void save(const char* path, bool isAddressingStartingNode, const char* fileName);

    JsonParser() = default;

public:
    JsonParser(const JsonParser& other) = delete;
    JsonParser& operator=(const JsonParser& other) = delete;

    static JsonParser* getInstance();
    static void freeInstance();

    JsonNodeType getStartingNodeType() const;
    bool fileIsOpened() const;
    bool changesHaveBeenMade() const;

    static void validate(const char* fileName);

    void openFile(const char* fileName);
    void closeFile();

    void print() const;
    void search(const char* key) const;

    void set(const char* path, const char* newStr);
    void remove(const char* path);

    void createInArray(const char* path, const char* newStr);
    void createInStartingObject(const char* newKey, const char* newStr);
    void createInObject(const char* path, const char* newKey, const char* newStr);

    void moveToArray(const char* pathFrom, const char* pathTo);
    void moveToStartingObject(const char* pathFrom);
    void moveToObject(const char* pathFrom, const char* pathTo);

    void save();
    void save(const char* path);
    void saveAs(const char* fileName);
    void saveAs(const char* fileName, const char* path);
};
