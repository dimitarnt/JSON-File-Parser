#pragma once
#include "Vector.hpp"
#include "Pair.hpp"
#include "JsonNodeContainer.h"

class JsonArray : public JsonNode {
private:
    JsonNodeContainer _jsonNodeCollection;

    static void assertNaturalNumberFromStr(const String& index, unsigned nestingLevel);
    void assertIndex(size_t index, unsigned nestingLevel) const;
    void assertExtendedIndex(size_t index, unsigned nestingLevel) const;

public:
    JsonArray();
    explicit JsonArray(std::ifstream& in);
    JsonNode* clone() const override;

    void add(const SharedPtr<JsonNode>& newJsonNode);
    void add(SharedPtr<JsonNode>&& newJsonNode);
    bool isEmpty() const;

    void print(unsigned nestingLevel, bool isInArray) const override;
    void save(std::ofstream& out, unsigned nestingLevel, bool isInArray) const override;
    void search(JsonArray& searchResults, const String& keyStr) const;

    void set(const char* path, const char* newStr, unsigned nestingLevel);
    Pair<String, SharedPtr<JsonNode>> remove(const char* path, unsigned nestingLevel);

    void create(const char* path, bool isAddressingStartingNode, bool createInArray,
                const char* newKey, const char* newStr, unsigned nestingLevel);
    void move(const char* path, bool isAddressingStartingNode, bool moveInArray,
              const char* movedKey, SharedPtr<JsonNode>&& jsonNodeForMoving, unsigned nestingLevel);

    void savePath(const char* path, std::ofstream& out, unsigned nestingLevel);
};
