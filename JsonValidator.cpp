#include "JsonValidator.h"
#include "InvalidJsonSyntax.h"
#include "fileFunctions.h"

void JsonValidator::assertIndex(unsigned index) const {
    if(index > (_tokenCount - 1)) {
        throw std::out_of_range("Error, index is out of range");
    }
}

void JsonValidator::setTokens(std::ifstream& in) {
    bool stringHasNotBeenClosed = false;
    unsigned buildingOfKeywordTrue = 0;
    unsigned buildingOfKeywordFalse = 0;
    unsigned buildingOfKeywordNull = 0;

    while (true) {
        char currentSymbol = (char) in.get();
        std::cout << currentSymbol;

        if (in.eof()) {
            break;
        }

        if(stringHasNotBeenClosed && currentSymbol != '\"') {
            continue;
        }

        switch (currentSymbol) {
            case '"':
                _tokens += currentSymbol;
                stringHasNotBeenClosed = !stringHasNotBeenClosed;
                break;

            case 't':
                if(buildingOfKeywordTrue != 0) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordTrue = 1;
                _tokens += currentSymbol;
                break;

            case 'r':
                if(buildingOfKeywordTrue != 1) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordTrue = 2;
                _tokens += currentSymbol;
                break;

            case 'u':
                if(!(buildingOfKeywordTrue == 2 || buildingOfKeywordNull == 1)) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordTrue == 2 ? buildingOfKeywordTrue = 3 : buildingOfKeywordNull = 2;
                _tokens += currentSymbol;
                break;

            case 'e':
                if(!(buildingOfKeywordTrue == 3 || buildingOfKeywordFalse == 4)) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordTrue == 3 ? buildingOfKeywordTrue = 0 : buildingOfKeywordFalse = 0;
                _tokens += currentSymbol;
                break;

            case 'f':
                if(buildingOfKeywordFalse != 0) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordFalse = 1;
                _tokens += currentSymbol;
                break;

            case 'a':
                if(buildingOfKeywordFalse != 1) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordFalse = 2;
                _tokens += currentSymbol;
                break;

            case 'l':
                if(!(buildingOfKeywordFalse == 2 || buildingOfKeywordNull == 2 || buildingOfKeywordNull == 3)) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordFalse == 2 ? buildingOfKeywordFalse = 3 :
                        (buildingOfKeywordNull == 2 ? buildingOfKeywordNull = 3 : buildingOfKeywordNull = 0);
                _tokens += currentSymbol;
                break;

            case 's':
                if(buildingOfKeywordFalse != 3) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordFalse = 4;
                _tokens += currentSymbol;
                break;

            case 'n':
                if(buildingOfKeywordNull != 0) {
                    String message = "Out of place character at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                buildingOfKeywordNull = 1;
                _tokens += currentSymbol;
                break;

            case '{':
            case '}':

            case '[':
            case ']':

            case ':':
            case ',':

            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '-':
            case '.':

            case '\n':
                if(buildingOfKeywordTrue != 0 || buildingOfKeywordFalse != 0 || buildingOfKeywordNull != 0) {
                    String message = "Unfinished bool keyword at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                _tokens += currentSymbol;
                break;

            case ' ':
            case '\t':
                if(buildingOfKeywordTrue != 0 || buildingOfKeywordFalse != 0 || buildingOfKeywordNull != 0) {
                    String message = "Unfinished bool keyword at row ";
                    message += getLinesCount(in, in.tellg());

                    throw InvalidJsonSyntax(message);
                }
                break;

            default:
                String message = "Non-token character is out of quotation marks at row ";
                message += getLinesCount(in, in.tellg());

                throw InvalidJsonSyntax(message);
        }
    }
    in.clear();

    _tokenCount = _tokens.getLength();
}

unsigned JsonValidator::getTokenCount(char token, unsigned untilIndex) const {
    assertIndex(untilIndex);
    unsigned count = 0;

    for(unsigned i = 0; i <= untilIndex; ++i) {
        if(_tokens[i] == token) {
            count++;
        }
    }

    return count;
}

long long JsonValidator::getPositionOfToken(char token, unsigned timesMet) const {

    for (unsigned i = 0; i < _tokenCount; ++i) {
        if (_tokens[i] == token) {
            timesMet--;
        }

        if (timesMet == 0) {
            return i;
        }
    }

    return -1;
}

long long JsonValidator::getLastPositionOfToken(char token, unsigned fromIndex) const {
    assertIndex(fromIndex);

    for(unsigned i = fromIndex; i <= fromIndex; --i) {
        if(_tokens[i] == token) {
            return i;
        }
    }

    return -1;
}

char JsonValidator::getPrecedingNonNewLineTokens(unsigned index, unsigned tokensBack) const {
    assertIndex(index);

    if(tokensBack == 0) {
        return _tokens[index];
    }
    index--;

    while(index < _tokenCount) {
        if(_tokens[index] != '\n') {
            tokensBack--;

            if(tokensBack == 0) {
                return _tokens[index];
            }
        }
        index--;
    }

    return 0;
}

char JsonValidator::getNextNonNewLineToken(unsigned index) const {
    assertIndex(index + 1);

    index++;

    while(index < _tokenCount) {
        if(_tokens[index] != '\n') {
            return _tokens[index];
        }
        index++;
    }

    return 0;
}

unsigned JsonValidator::getRowPositionOfToken(unsigned index) const {
    assertIndex(index);
    unsigned rows = 1;

    for(int i = 0; i < index; ++i) {
        if(_tokens[i] == '\n') {
            rows++;
        }
    }

    return rows;
}

bool JsonValidator::tokenIsInJsonObjectScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                             long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket) {

    return (lastPositionOfClosedBracket > lastPositionOfOpenBracket || lastPositionOfOpenBrace > lastPositionOfOpenBracket)
            && lastPositionOfOpenBrace > lastPositionOfClosedBrace;
}

bool JsonValidator::tokenIsInJsonArrayScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                            long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket) {

    return (lastPositionOfClosedBrace > lastPositionOfOpenBrace || lastPositionOfOpenBracket > lastPositionOfOpenBrace)
            && lastPositionOfOpenBracket > lastPositionOfClosedBracket;
}

bool JsonValidator::validTokenBeforeOpenBrace(char token) {

    return token == ',' || token == ':' || token == '[' || token == 0;
}

bool JsonValidator::validTokenBeforeClosedBrace(char previousToken, char tokenThreePositionsBack) {

    return (previousToken == '{' || previousToken == '}' || previousToken == '\"' || previousToken == ']'
            || previousToken == 'e' || previousToken == 'l' || (previousToken >= '0' && previousToken <= '9'))
            && !(previousToken == '\"' && tokenThreePositionsBack == ',');
}

bool JsonValidator::validTokenBeforeOpenBracket(char token) {

    return token == '[' || token == ':' || token == ',' || token == 0;
}

bool JsonValidator::validTokenBeforeClosedBracket(char token) {

    return token == '}' || token == '\"' || token == '[' || token == ']' || token == 'e' || token == 'l' || (token >= '0' && token <= '9');
}

bool JsonValidator::validZeroPlacementInJsonObject(char previousToken, char nextToken) {

    return !((previousToken == ':' || previousToken == '-') && !(nextToken == '.' || nextToken == ',')) && validTokenBeforeDigitInJsonObject(previousToken);
}

bool JsonValidator::validZeroPlacementInJsonArray(char previousToken, char nextToken) {

    return !((previousToken == ','|| previousToken == '-') && nextToken != '.') && validTokenBeforeDigitInJsonArray(previousToken);
}

bool JsonValidator::validTokenBeforeDigitInJsonObject(char token) {

    return token == ':' || token == '-' || token == '.' || (token >= '0' && token <= '9');
}

bool JsonValidator::validTokenBeforeDigitInJsonArray(char token) {

    return token == ',' || token == '-' || token == '.' || (token >= '0' && token <= '9');
}

bool JsonValidator::validTokenBeforeMinusInJsonObject(char previousToken, char nextToken) {

    return previousToken == ':' && nextToken >= '0' && nextToken <= '9';
}

bool JsonValidator::validTokenBeforeMinusInJsonArray(char previousToken, char nextToken) {

    return (previousToken == ',' || previousToken == '[') && nextToken >= '0' && nextToken <= '9';
}

bool JsonValidator::validTokenBeforeDot(char previousToken, char nextToken) {

    return previousToken >= '0' && previousToken <= '9' && nextToken >= '0' && nextToken <= '9';
}

bool JsonValidator::validTokenBeforeValueKeywordInJsonObject(char token) {

    return token == ':';
}

bool JsonValidator::validTokenBeforeValueKeywordInJsonArray(char token) {

    return token == '[' || token == ',';
}

bool JsonValidator::validTokenBeforeQuotationMark(char previousToken, char tokenTwoPositionsBack) {

    return (previousToken == '\"' || previousToken == ':' || previousToken == ',' || previousToken == '{' || previousToken == '[')
            && !(previousToken == '\"' && tokenTwoPositionsBack == '\"');
}

bool JsonValidator::validTokensPrecedingColonInJsonObject(char previousToken, char tokenThreePositionsBack) {

    return previousToken == '\"' && (tokenThreePositionsBack == ',' || tokenThreePositionsBack == '{');
}

bool JsonValidator::validTokensPrecedingCommaInJsonObject(char previousToken, char tokenThreePositionsBack) {

    return validTokensPrecedingCommaInJsonArray(previousToken) && tokenThreePositionsBack != '{' && tokenThreePositionsBack != ',';
}

bool JsonValidator::validTokensPrecedingCommaInJsonArray(char token) {

    return token == '\"' || token == '}' || token == ']' || token == 'e' || token == 'l' || (token >= '0' && token <= '9');
}

void JsonValidator::validateBraceMatching() const {
    unsigned numberOfOpenBraces = getTokenCount('{', _tokenCount - 1);
    unsigned numberOfClosedBraces = getTokenCount('}', _tokenCount - 1);

    if(numberOfOpenBraces < numberOfClosedBraces) {
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', _tokenCount - 1);

        String message = "Extraneous closed brace at row ";
        message += getRowPositionOfToken(lastPositionOfClosedBrace);

        throw InvalidJsonSyntax(message);
    }

    if(numberOfOpenBraces > numberOfClosedBraces) {
        long long firstPositionOfOpenBrace = getPositionOfToken('{', 1);

        String message = "No matching closed brace of open brace at row ";
        message += getRowPositionOfToken(firstPositionOfOpenBrace);

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateBracePlacement() const {
    unsigned numberOfOpenBraces = getTokenCount('{', _tokenCount - 1);
    unsigned numberOfClosedBraces = getTokenCount('}', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfOpenBraces; ++i) {
        long long positionOfOpenBrace = getPositionOfToken('{', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfOpenBrace, 1);

        if(!validTokenBeforeOpenBrace(previousToken)) {
            String message = "Out of place open brace at row ";
            message += getRowPositionOfToken(positionOfOpenBrace);

            throw InvalidJsonSyntax(message);
        }
    }

    for(unsigned i = 0; i < numberOfClosedBraces; ++i) {
        long long positionOfClosedBrace = getPositionOfToken('}', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBrace, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfClosedBrace, 3);

        if(!validTokenBeforeClosedBrace(previousToken, tokenThreePositionsBack)) {
            String message = "Out of place closed brace at row ";
            message += getRowPositionOfToken(positionOfClosedBrace);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateBracketMatching() const {
    unsigned numberOfOpenBrackets = getTokenCount('[', _tokenCount - 1);
    unsigned numberOfClosedBrackets = getTokenCount(']', _tokenCount - 1);

    if(numberOfOpenBrackets < numberOfClosedBrackets) {
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', _tokenCount - 1);

        String message = "Extraneous closed bracket at row ";
        message += getRowPositionOfToken(lastPositionOfClosedBracket);

        throw InvalidJsonSyntax(message);
    }

    if(numberOfOpenBrackets > numberOfClosedBrackets) {
        long long firstPositionOfOpenBracket = getPositionOfToken('[', 1);

        String message = "No matching closed bracket at row ";
        message += getRowPositionOfToken(firstPositionOfOpenBracket);

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateBracketPlacement() const {
    unsigned numberOfOpenBrackets = getTokenCount('[', _tokenCount - 1);
    unsigned numberOfClosedBrackets = getTokenCount(']', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfOpenBrackets; ++i) {
        long long positionOfOpenBracket = getPositionOfToken('[', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfOpenBracket, 1);

        if(!validTokenBeforeOpenBracket(previousToken)) {
            String message = "Out of place open bracket at row ";
            message += getRowPositionOfToken(positionOfOpenBracket);

            throw InvalidJsonSyntax(message);
        }
    }

    for(unsigned i = 0; i < numberOfClosedBrackets; ++i) {
        long long positionOfClosedBracket = getPositionOfToken(']', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBracket, 1);

        if(!validTokenBeforeClosedBracket(previousToken)) {
            String message = "Out of place closed bracket at row ";
            message += getRowPositionOfToken(positionOfClosedBracket);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateDigitPlacement() const {

    for(unsigned i = 0; i < NUMBER_OF_DIGITS; ++i) {
        unsigned currentDigitCount = getTokenCount('0' + i, _tokenCount - 1);

        for(unsigned j = 0; j < currentDigitCount; ++j) {
            long long positionOfCurrentDigit = getPositionOfToken('0' + i, j + 1);
            long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfCurrentDigit);
            long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfCurrentDigit);
            long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfCurrentDigit);
            long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfCurrentDigit);

            char previousToken = getPrecedingNonNewLineTokens(positionOfCurrentDigit, 1);
            char nextToken = getNextNonNewLineToken(positionOfCurrentDigit);

            if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)) {

                if(('0' + i) == '0' && (!validZeroPlacementInJsonObject(previousToken, nextToken) || !validTokenBeforeDigitInJsonObject(previousToken))) {
                    String message = "Out of place 0 at row ";
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }

                if(!validTokenBeforeDigitInJsonObject(previousToken)) {
                    String message = "Out of place digit at row ";
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }
            }

            if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)) {

                if(('0' + i) == '0' && (!validZeroPlacementInJsonArray(previousToken, nextToken) || !validTokenBeforeDigitInJsonArray(previousToken))) {
                    String message = "Out of place 0 at row ";
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }

                if(!validTokenBeforeDigitInJsonArray(previousToken)) {
                    String message = "Out of place digit at row ";
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }
            }
        }
    }
}

void JsonValidator::validateDotPlacement() const {
    unsigned numberOfDots = getTokenCount('.', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfDots; ++i) {
        long long positionOfDot = getPositionOfToken('.', i + 1);

        char previousToken = getPrecedingNonNewLineTokens(positionOfDot, 1);
        char nextToken = getNextNonNewLineToken(positionOfDot);

        if(!validTokenBeforeDot(previousToken, nextToken)) {
            String message = "Out of place dot at row ";
            message += getRowPositionOfToken(positionOfDot);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateMinusPlacement() const {
    unsigned numberOfMinuses = getTokenCount('-', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfMinuses; ++i) {
        long long positionOfMinuses = getPositionOfToken('-', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfMinuses);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfMinuses);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfMinuses);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfMinuses);

        char previousToken = getPrecedingNonNewLineTokens(positionOfMinuses, 1);
        char nextToken = getNextNonNewLineToken(positionOfMinuses);

        if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)
            && !validTokenBeforeMinusInJsonObject(previousToken, nextToken)) {

            String message = "Out of place minus at row ";
            message += getRowPositionOfToken(positionOfMinuses);

            throw InvalidJsonSyntax(message);
        }

        if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)
            && !validTokenBeforeMinusInJsonArray(previousToken, nextToken)) {

            String message = "Out of place minus at row ";
            message += getRowPositionOfToken(positionOfMinuses);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateValueKeywordPlacement() const {

    for(unsigned i = 0; i < NUMBER_OF_JSON_KEYWORDS; ++i) {
        char firstCharacterOfKeyword = 0;
        unsigned currentKeywordCount = 0;

        switch(i) {
            case 0:
                firstCharacterOfKeyword = 't';
                currentKeywordCount = getTokenCount('t', _tokenCount - 1);
                break;
            case 1:
                firstCharacterOfKeyword = 'f';
                currentKeywordCount = getTokenCount('f', _tokenCount - 1);
                break;
            case 2:
                firstCharacterOfKeyword = 'n';
                currentKeywordCount = getTokenCount('n', _tokenCount - 1);
                break;
        }

        for(unsigned j = 0; j < currentKeywordCount; ++j) {
            long long positionOfKeyword = getPositionOfToken(firstCharacterOfKeyword, j + 1);
            long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfKeyword);
            long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfKeyword);
            long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfKeyword);
            long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfKeyword);

            char previousToken = getPrecedingNonNewLineTokens(positionOfKeyword, 1);

            if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)
               && !validTokenBeforeValueKeywordInJsonObject(previousToken)) {

                String message = "Out of place keyword at row ";
                message += getRowPositionOfToken(positionOfKeyword);

                throw InvalidJsonSyntax(message);
            }

            if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)
               && !validTokenBeforeValueKeywordInJsonArray(previousToken)) {

                String message = "Out of place keyword at row ";
                message += getRowPositionOfToken(positionOfKeyword);

                throw InvalidJsonSyntax(message);
            }
        }
    }

}

void JsonValidator::validateQuotationMarkPlacement() const {
    unsigned numberOfQuotationMarks = getTokenCount('\"', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfQuotationMarks; ++i) {
        long long positionOfQuotationMark = getPositionOfToken('\"', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfQuotationMark, 1);
        char tokenTwoPositionsBack = getPrecedingNonNewLineTokens(positionOfQuotationMark, 2);

        if(!validTokenBeforeQuotationMark(previousToken, tokenTwoPositionsBack)) {
            String message = "Out of place quotation mark at row ";
            message += getRowPositionOfToken(positionOfQuotationMark);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateColonPlacement() const {
    unsigned numberOfColons = getTokenCount(':', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfColons; ++i) {
        long long positionOfColon = getPositionOfToken(':', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfColon);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfColon);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfColon);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfColon);

        char previousToken = getPrecedingNonNewLineTokens(positionOfColon, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfColon, 3);

        if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)) {
            String message = "Colon is in place of a comma at row ";
            message += getRowPositionOfToken(positionOfColon);

            throw InvalidJsonSyntax(message);
        }

        if(!validTokensPrecedingColonInJsonObject(previousToken, tokenThreePositionsBack)) {
            String message = "Out of place colon at row ";
            message += getRowPositionOfToken(positionOfColon);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateCommaPlacement() const {
    unsigned numberOfCommas = getTokenCount(',', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfCommas; ++i) {
        long long positionOfComma = getPositionOfToken(',', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfComma);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfComma);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfComma);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfComma);

        char previousToken = getPrecedingNonNewLineTokens(positionOfComma, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfComma, 3);

        if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket)) {

            if(!validTokensPrecedingCommaInJsonObject(previousToken, tokenThreePositionsBack)) {
                String message = "Comma is in place of a colon at row ";
                message += getRowPositionOfToken(positionOfComma);

                throw InvalidJsonSyntax(message);
            }
        }

        if(!validTokensPrecedingCommaInJsonArray(previousToken)) {
            String message = "Out of place comma at row ";
            message += getRowPositionOfToken(positionOfComma);

            throw InvalidJsonSyntax(message);
        }
    }
}

JsonValidator::JsonValidator(std::ifstream& in) {
    unsigned previousPosition = in.tellg();
    setTokens(in);

    in.seekg(previousPosition);
}

void JsonValidator::validate() const {
    validateBraceMatching();
    validateBracePlacement();

    validateBracketMatching();
    validateBracketPlacement();

    validateDigitPlacement();
    validateDotPlacement();
    validateMinusPlacement();
    validateValueKeywordPlacement();

    validateQuotationMarkPlacement();
    validateColonPlacement();
    validateCommaPlacement();
}
