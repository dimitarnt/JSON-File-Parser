#pragma once
#include "JsonNode.h"

class JsonString : public JsonNode {
private:
    String _str;

    static void assertString(const String& str);

public:
    JsonString() = delete;
    explicit JsonString(std::ifstream& in);
    explicit JsonString(const String& str);
    explicit JsonString(String&& str);

    void print(unsigned nestingLevel) const override;
    void save(std::ofstream& out, unsigned nestingLevel) const override;

    JsonNode* clone() const override;
};
