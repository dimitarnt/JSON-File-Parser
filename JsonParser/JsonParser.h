#pragma once
#include "JsonValidator.h"
#include "JsonNodeContainer.h"
#include "JsonPair.h"
#include "UndoActionContainer.h"

//Singleton parser class
class JsonParser {
private:
    JsonNodeContainer _startingNode;
    String _fileName;
    String _traversedPath;
    String _moveFromPath;
    UndoActionContainer _undoData;
    JsonNodeType _startingNodeType = JsonNodeType::JSON_OBJECT;
    bool _fileIsOpened = false;
    bool _changesHaveBeenMade = false;
    bool _actionIsBeingUndone = false;

    static JsonParser* instance;

    static void assertJsonFileName(const char* fileName);
    static void assertKey(const char* key);
    void assertOpenFile() const;
    void assertNonInitiatedMove() const;

    String getConcatenatedTraversalPath(const char* path) const;
    static String getPreviousPath(const char* path);
    static String getBaseKey(const char* path);
    static String addToPath(const char* path, const String& newKey);

    void set(const char* newValue, const char* path, JsonNodeType type);
    void createInArray(const char* newValue, const char* path, JsonNodeType type);
    void createInObject(const char* newKey, const char* newValue, const char* path, JsonNodeType type);
    void save(const char* fileName, const char* path);

    JsonParser() = default;

public:
    JsonParser(const JsonParser& other) = delete;
    JsonParser& operator=(const JsonParser& other) = delete;

    static JsonParser* getInstance();
    static void freeInstance();

    bool fileIsOpened() const;
    bool changesHaveBeenMade() const;
    bool actionIsBeingUndone() const;

    static void validate(const char* fileName);

    void openFile(const char* fileName);
    void closeFile();

    void print() const;
    void search(const char* key) const;

    void traverse(const char* path);
    void previous();
    void goBack();

    void undo();
    void undoAll();

    void rename(const char* newKey, const char* path);

    void set(const SharedPtr<JsonNode>& newNode, const char* path);
    void set(SharedPtr<JsonNode>&& newNode, const char* path);
    void setEmptyObject(const char* path);
    void setEmptyArray(const char* path);
    void setString(const char* newStr, const char* path);
    void setKeyword(const char* newKeyword, const char* path);
    void setNumber(const char* newNumber, const char* path);

    void remove(const char* path);

    void createInArray(const SharedPtr<JsonNode>& newNode, const char* path);
    void createInArray(SharedPtr<JsonNode>&& newNode, const char* path);
    void createInObject(const JsonPair& newPair, const char* path);
    void createInObject(JsonPair&& newPair, const char* path);

    void createEmptyObjectInArray(const char* path);
    void createEmptyObjectInObject(const char* newKey, const char* path);

    void createEmptyArrayInArray(const char* path);
    void createEmptyArrayInObject(const char* newKey, const char* path);

    void createStringInArray(const char* newStr, const char* path);
    void createStringInObject(const char* newKey, const char* newStr, const char* path);

    void createKeywordInArray(const char* newKeyword, const char* path);
    void createKeywordInObject(const char* newKey, const char* newKeyword, const char* path);

    void createNumberInArray(const char* newNumber, const char* path);
    void createNumberInObject(const char* newKey, const char* newNumber, const char* path);

    void moveFrom(const char* pathFrom);
    void moveToArray(const char* pathTo, const char* pathFrom);
    void moveToObject(const char* pathTo, const char* pathFrom, const char* oldKey);

    void save(const char* path);
    void saveAs(const char* fileName, const char* path);
};
