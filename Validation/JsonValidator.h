#pragma once
#include "String.h"
#include <fstream>

class JsonValidator {
private:
    String _tokens;

    void assertIndex(long long index) const;

    void setTokens(std::ifstream& in);

    static void throwOutOfPlaceCharacterException(std::ifstream& in);
    static void throwNonTokenCharacterException(std::ifstream& in);

    static void validateBoolBuildingInterception(std::ifstream& in, long long buildingOfTrueKeyword,
                                                 long long buildingOfFalseKeyword, long long buildingOfNullKeyword);
    static void validateNumberBuildingInterception(std::ifstream& in, bool numberIsBeingBuilt);

    static void validateT(std::ifstream& in, long long& buildingOfKeywordTrue);
    static void validateR(std::ifstream& in, long long& buildingOfKeywordTrue);
    static void validateU(std::ifstream& in, long long& buildingOfKeywordTrue, long long& buildingOfKeywordNull);
    static void validateE(std::ifstream& in, long long& buildingOfKeywordTrue, long long& buildingOfKeywordFalse);
    static void validateF(std::ifstream& in, long long& buildingOfKeywordFalse);
    static void validateA(std::ifstream& in, long long& buildingOfKeywordFalse);
    static void validateL(std::ifstream& in, long long& buildingOfKeywordFalse, long long& buildingOfKeywordNull);
    static void validateS(std::ifstream& in, long long& buildingOfKeywordFalse);
    static void validateN(std::ifstream& in, long long& buildingOfKeywordNull);

    long long getLastPositionOfToken(char token, long long fromIndex) const;
    char getPrecedingNonNewLineTokens(long long index, long long tokensBack) const;
    char getNextNonNewLineToken(long long index) const;
    long long getRowPositionOfToken(long long index) const;

    bool tokenIsInJsonObjectScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                  long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket,
                                  long long nestedJsonObjectCount, long long nestedJsonArrayCount) const;
    bool tokenIsInJsonArrayScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                 long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket,
                                 long long nestedJsonObjectCount, long long nestedJsonArrayCount) const;

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
    static bool validTokenBeforeDecimalPoint(char previousToken, char nextToken);

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
    void validateDecimalPointPlacement() const;
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
