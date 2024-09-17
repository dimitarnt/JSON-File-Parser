#pragma once
#include "JsonNode.h"

class JsonNumber : public JsonNode {
private:
    String _number;

    static void assertNumber(const String& number);

public:
    JsonNumber() = delete;
    explicit JsonNumber(std::ifstream& in);
    explicit JsonNumber(const String& number);
    explicit JsonNumber(String&& number);

    void print(unsigned nestingLevel) const override;
    void save(std::ofstream& out, unsigned nestingLevel) const override;

    JsonNode* clone() const override;
};
