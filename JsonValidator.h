#pragma once
#include "String.h"
#include "helperFunctions.h"
#include <fstream>

class JsonValidator {
private:
    String _tokens;
    unsigned _tokenCount;

    void assertIndex(unsigned index) const;

    void setTokens(std::ifstream& in);

    unsigned getTokenCount(char token, unsigned untilIndex) const;
    long long getPositionOfToken(char token, unsigned timesMet) const;
    long long getLastPositionOfToken(char token, unsigned fromIndex) const;
    char getPrecedingNonNewLineTokens(unsigned index, unsigned tokensBack) const;
    unsigned getRowPositionOfToken(unsigned index) const;

    void validateBraceMatching() const;
    void validateBracePlacement() const;

    void validateBracketMatching() const;
    void validateBracketPlacement() const;

    void validateQuotationMarkPlacement() const;
    void validateColonPlacement() const;
    void validateCommaPlacement() const;

public:
    JsonValidator() = delete;
    explicit JsonValidator(std::ifstream& in);

    void validate() const;
};
