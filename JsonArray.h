#pragma once
#include "Vector.h"
#include "JsonNode.h"

class JsonArray : public JsonNode {
private:
    Vector<JsonNode*> _jsonNodeCollection;

    void copyFrom(const JsonArray& other);
    void moveFrom(JsonArray&& other);
    void free();

public:
    JsonArray() = delete;
    explicit JsonArray(std::ifstream& in);

    JsonArray(const JsonArray& other);
    JsonArray(JsonArray&& other) noexcept;
    JsonArray& operator=(const JsonArray& other);
    JsonArray& operator=(JsonArray&& other) noexcept;
    ~JsonArray() override;

    void setData(std::ifstream& in);

    JsonNode* clone() const override;
    
};
