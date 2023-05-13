#pragma once
#include "Vector.h"
#include "JsonNode.h"

class JsonArray : public JsonNode {
private:
    Vector<JsonNode*> _jsonNodeCollection;
    unsigned _nestingLevel;

    void copyFrom(const JsonArray& other);
    void moveFrom(JsonArray&& other);
    void free();

public:
    JsonArray() = delete;
    explicit JsonArray(std::ifstream& in, unsigned nestingLevel);

    JsonArray(const JsonArray& other);
    JsonArray(JsonArray&& other) noexcept;
    JsonArray& operator=(const JsonArray& other);
    JsonArray& operator=(JsonArray&& other) noexcept;
    ~JsonArray() override;

    void print() const override;

    JsonNode* clone() const override;
    
};
