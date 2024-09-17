#pragma once
#include "Vector.hpp"
#include "JsonNodeContainer.h"
#include "JsonPair.h"
#include "UndoActionContainer.h"

class JsonArray : public JsonNode {
private:
    JsonNodeContainer _jsonNodeCollection;

    static void assertNaturalNumberFromStr(const String& index, unsigned nestingLevel);
    void assertIndex(size_t index, unsigned nestingLevel) const;
    void assertExtendedIndex(size_t index, unsigned nestingLevel) const; //The current size is a valid extended-index

    static unsigned getIndex(const String& key, unsigned nestingLevel);
    static unsigned getIndex(String&& key, unsigned nestingLevel);

public:
    JsonArray();
    explicit JsonArray(std::ifstream& in);
    JsonNode* clone() const override;

    //These functions are used for the search function
    void add(const SharedPtr<JsonNode>& newJsonNode);
    void add(SharedPtr<JsonNode>&& newJsonNode);
    bool isEmpty() const;

    void print(unsigned nestingLevel) const override;
    void save(std::ofstream& out, unsigned nestingLevel) const override;

    void search(JsonArray& searchResults, const char* key) const;

    void rename(const char* newKey, const char* path, unsigned nestingLevel, UndoActionContainer& undoData);

    void set(const SharedPtr<JsonNode>& newNode, const char* path, unsigned nestingLevel, UndoActionContainer& undoData);
    void set(SharedPtr<JsonNode>&& newNode, const char* path, unsigned nestingLevel, UndoActionContainer& undoData);
    void setEmptyObject(const char* path, unsigned nestingLevel, UndoActionContainer& undoData);
    void setEmptyArray(const char* path, unsigned nestingLevel, UndoActionContainer& undoData);
    void setString(const char* newStr, const char* path, unsigned nestingLevel, UndoActionContainer& undoData);
    void setKeyword(const char* newKeyword, const char* path, unsigned nestingLevel, UndoActionContainer& undoData);
    void setNumber(const char* newNumber, const char* path, unsigned nestingLevel, UndoActionContainer& undoData);

    JsonPair remove(const char* path, unsigned nestingLevel, UndoActionContainer& undoData);

    void create(const JsonPair& newPair, const char* path, bool createInArray, unsigned nestingLevel);
    void create(JsonPair&& newPair, const char* path, bool createInArray, unsigned nestingLevel);
    void createEmptyObject(const char* newKey, const char* path, bool createInArray, unsigned nestingLevel);
    void createEmptyArray(const char* newKey, const char* path, bool createInArray, unsigned nestingLevel);
    void createString(const char* newKey, const char* newStr, const char* path, bool createInArray, unsigned nestingLevel);
    void createKeyword(const char* newKey, const char* newKeyword, const char* path, bool createInArray, unsigned nestingLevel);
    void createNumber(const char* newKey, const char* newNumber, const char* path, bool createInArray, unsigned nestingLevel);

    JsonPair moveRemove(const char* path, unsigned nestingLevel);
    void move(JsonPair&& jsonPairForMoving, const char* path, bool moveInArray, unsigned nestingLevel);

    //Functions to initiate printing or saving after a certain path is traversed
    void printPath(const char* path, unsigned nestingLevel);
    void savePath(std::ofstream& out, const char* path, unsigned nestingLevel);
};
