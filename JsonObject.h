#pragma once
#include "String.h"
#include "Vector.h"
#include "JsonNode.h"

class JsonObject : public JsonNode {
private:
    Vector<JsonNode*> _jsonNodeCollection;
    Vector<String> _correspondingKeys;

    void copyFrom(const JsonObject& other);
    void moveFrom(JsonObject&& other);
    void free();

public:
    JsonObject() = delete;
    explicit JsonObject(std::ifstream& in);

    JsonObject(const JsonObject& other);
    JsonObject(JsonObject&& other) noexcept;
    JsonObject& operator=(const JsonObject& other);
    JsonObject& operator=(JsonObject&& other) noexcept;
    ~JsonObject() override;

    void setData(std::ifstream& in);

    JsonNode* clone() const override;
};
