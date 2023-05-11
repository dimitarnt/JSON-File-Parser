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
    char getNextNonNewLineToken(unsigned index) const;
    unsigned getRowPositionOfToken(unsigned index) const;

    static bool tokenIsInJsonObjectScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                         long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket);
    static bool tokenIsInJsonArrayScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                        long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket);

    static bool validTokenBeforeOpenBrace(char token);
    static bool validTokenBeforeClosedBrace(char previousToken, char tokenThreePositionsBack);
    static bool validTokenBeforeOpenBracket(char token);
    static bool validTokenBeforeClosedBracket(char token);

    static bool validTokenBeforeDigitInJsonObject(char token);
    static bool validTokenBeforeDigitInJsonArray(char token);

    static bool validZeroPlacementInJsonObject(char previousToken, char nextToken);
    static bool validZeroPlacementInJsonArray(char previousToken, char nextToken);

    static bool validTokenBeforeMinusInJsonObject(char previousToken, char nextToken);
    static bool validTokenBeforeMinusInJsonArray(char previousToken, char nextToken);
    static bool validTokenBeforeDot(char previousToken, char nextToken);

    static bool validTokenBeforeValueKeywordInJsonObject(char token);
    static bool validTokenBeforeValueKeywordInJsonArray(char token);

    static bool validTokenBeforeQuotationMark(char previousToken, char tokenTwoPositionsBack);
    static bool validTokensPrecedingColonInJsonObject(char previousToken, char tokenThreePositionsBack);
    static bool validTokensPrecedingCommaInJsonObject(char previousToken, char tokenThreePositionsBack);
    static bool validTokensPrecedingCommaInJsonArray(char token);

    void validateBraceMatching() const;
    void validateBracePlacement() const;

    void validateBracketMatching() const;
    void validateBracketPlacement() const;

    void validateDigitPlacement() const;
    void validateDotPlacement() const;
    void validateMinusPlacement() const;
    void validateValueKeywordPlacement() const;

    void validateQuotationMarkPlacement() const;
    void validateColonPlacement() const;
    void validateCommaPlacement() const;

public:
    JsonValidator() = delete;
    explicit JsonValidator(std::ifstream& in);

    void validate() const;
};
