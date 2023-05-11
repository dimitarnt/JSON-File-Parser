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

    static bool tokenIsInJsonObjectScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                         long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket);
    static bool tokenIsInJsonArrayScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                        long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket);

    static bool validTokenBeforeOpenBrace(char token) ;
    static bool validTokenBeforeClosedBrace(char token);
    static bool validTokenBeforeOpenBracket(char token);
    static bool validTokenBeforeClosedBracket(char token);

    static bool validTokenBeforeQuotationMark(char token);
    static bool validTokensPrecedingColonInJsonObject(char token, char tokenThreePositionsBack);
    static bool validTokensPrecedingCommaInJsonObject(char token, char tokenThreePositionsBack);
    static bool validTokensPrecedingCommaInJsonArray(char token);

    static bool validTokenBeforeDigit(char token);
    static bool validTokenBeforeDot(char token);

    static bool validTokenBeforeBoolCharacter(char token);
    static bool validTokenBeforeNullCharacter(char token);

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
