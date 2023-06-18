#pragma once
#include "Vector.hpp"
#include "JsonPairContainer.h"
#include "JsonArray.h"
#include "JsonNodeContainer.h"

class JsonObject : public JsonNode {
private:
    JsonPairContainer _jsonPairs;

    static void assertKey(const char* key);
    void assertNewKey(const char* newKey) const; //New key shouldn't be a duplicate of another key in the same object

    unsigned getKeyIndex(const String& key, unsigned nestingLevel) const;
    unsigned getKeyIndex(String&& key, unsigned nestingLevel) const;

public:
    JsonObject() = delete;
    explicit JsonObject(std::ifstream& in);
    JsonNode* clone() const override;

    void print(unsigned nestingLevel, bool isInArray) const override;
    void save(std::ofstream& out, unsigned nestingLevel, bool isInArray) const override;
    void search(JsonArray& searchResults, const String& keyStr) const;

    void set(const char* path, const char* newStr, unsigned nestingLevel);
    Pair<String, SharedPtr<JsonNode>> remove(const char* path, unsigned nestingLevel);

    void create(const char* path, bool isAddressingStartingNode, bool createInArray,
                const char* newKey, const char* newStr, unsigned nestingLevel);
    void move(const char* path, bool isAddressingStartingNode, bool moveInArray,
              const char* movedKey, SharedPtr<JsonNode>&& jsonNodeForMoving, unsigned nestingLevel);

    //Function to initiate saving after a certain path is traversed
    void savePath(const char* path, std::ofstream& out, unsigned nestingLevel);
};
