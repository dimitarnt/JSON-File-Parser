#pragma once
#include "JsonNode.h"

class JsonKeyword : public JsonNode {
private:
    String _keyword;

    static void assertKeyword(const String& keyword);

public:
    JsonKeyword() = delete;
    explicit JsonKeyword(std::ifstream& in);
    explicit JsonKeyword(const String& keyword);
    explicit JsonKeyword(String&& keyword);

    void print(unsigned nestingLevel) const override;
    void save(std::ofstream& out, unsigned nestingLevel) const override;

    JsonNode* clone() const override;
};
