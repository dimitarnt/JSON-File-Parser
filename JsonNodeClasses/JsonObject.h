#pragma once
#include "Vector.hpp"
#include "JsonPairContainer.h"
#include "JsonNodeContainer.h"
#include "UndoActionContainer.h"

class JsonObject : public JsonNode {
private:
    JsonPairContainer _jsonPairs;

    void assertUniqueKey(const String& newKey) const; //New key shouldn't be a duplicate of another key in the same object
    String createDefaultNewKey() const;

    unsigned getKeyIndex(const String& key, unsigned nestingLevel) const;
    unsigned getKeyIndex(String&& key, unsigned nestingLevel) const;

public:
    JsonObject();
    explicit JsonObject(std::ifstream& in);
    JsonNode* clone() const override;

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
