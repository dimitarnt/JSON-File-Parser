#include "JsonValidator.h"
#include "InvalidJsonSyntax.h"
#include "helperFunctions.h"

void JsonValidator::assertIndex(unsigned index) const {
    if(index > (_tokenCount - 1)) {
        throw std::out_of_range("Error, index is out of range");
    }
}

void JsonValidator::setTokens(std::ifstream& in) {
    bool stringHasNotBeenClosed = false;

    while (true) {
        char currentSymbol = (char) in.get();

        if (in.eof()) {
            break;
        }

        if(stringHasNotBeenClosed && currentSymbol != '\"') {
            continue;
        }

        switch (currentSymbol) {
            case '"':
                _tokens += '\"';
                stringHasNotBeenClosed = !stringHasNotBeenClosed;
                break;

            case '{':
                _tokens += '{';
                break;

            case '}':
                _tokens += '}';
                break;

            case '[':
                _tokens += '[';
                break;

            case ']':
                _tokens += ']';
                break;

            case ':':
                _tokens += ':';
                break;

            case ',':
                _tokens += ',';
                break;

            case '\n':
                _tokens += '\n';
                break;

            case ' ':
            case '\t':
                break;

            default:
                String message = "Non-token character is out of quotation marks at row ";
                message += getLinesCount(in, in.tellg());

                std::cout << in.tellg();

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

        if(previousToken != ',' && previousToken != ':' && previousToken != '[' && previousToken != 0) {
            String message = "Out of place open brace at row ";
            message += getRowPositionOfToken(positionOfOpenBrace);

            throw InvalidJsonSyntax(message);
        }
    }

    for(unsigned i = 0; i < numberOfClosedBraces; ++i) {
        long long positionOfClosedBrace = getPositionOfToken('}', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBrace, 1);

        if(previousToken != '{' && previousToken != '}' && previousToken != '\"' && previousToken != ']') {
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

        if(previousToken != '[' && previousToken != ':' && previousToken != ',' && previousToken != 0) {
            String message = "Out of place open bracket at row ";
            message += getRowPositionOfToken(positionOfOpenBracket);

            throw InvalidJsonSyntax(message);
        }
    }

    for(unsigned i = 0; i < numberOfClosedBrackets; ++i) {
        long long positionOfClosedBracket = getPositionOfToken(']', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBracket, 1);

        if(previousToken != '}' && previousToken != '\"' && previousToken != '[' && previousToken != ']') {
            String message = "Out of place closed bracket at row ";
            message += getRowPositionOfToken(positionOfClosedBracket);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateQuotationMarkPlacement() const {
    unsigned numberOfQuotationMarks = getTokenCount('\"', _tokenCount - 1);

    for(unsigned i = 0; i < numberOfQuotationMarks; ++i) {
        long long positionOfQuotationMark = getPositionOfToken('\"', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfQuotationMark, 1);

        if(previousToken != '\"' && previousToken != ',' && previousToken != ':' && previousToken != '[' && previousToken != '{') {
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

        if((lastPositionOfClosedBrace > lastPositionOfOpenBrace || lastPositionOfOpenBracket > lastPositionOfOpenBrace)
            && lastPositionOfOpenBracket > lastPositionOfClosedBracket) {

            String message = "Out of place colon at row ";
            message += getRowPositionOfToken(positionOfColon);

            throw InvalidJsonSyntax(message);
        }

        if(previousToken != '\"' || (tokenThreePositionsBack != '{' && tokenThreePositionsBack != ',')) {
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

        unsigned openBracesBeforeComma = getTokenCount('{', positionOfComma);
        unsigned closedBracesBeforeComma = getTokenCount('{', positionOfComma);
        unsigned openBracketsBeforeComma = getTokenCount('[', positionOfComma);
        unsigned closedBracketsBeforeComma = getTokenCount(']', positionOfComma);

        char previousToken = getPrecedingNonNewLineTokens(positionOfComma, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfComma, 3);

        if((lastPositionOfClosedBracket > lastPositionOfOpenBracket || lastPositionOfOpenBrace > lastPositionOfOpenBracket)
           && lastPositionOfOpenBrace > lastPositionOfClosedBrace) {

            if(tokenThreePositionsBack == '{' || tokenThreePositionsBack == ',') {
                String message = "Comma is in place of a colon at row ";
                message += getRowPositionOfToken(positionOfComma);

                throw InvalidJsonSyntax(message);
            }
        }

        if(openBracesBeforeComma == closedBracesBeforeComma && openBracketsBeforeComma == closedBracketsBeforeComma) {
            String message = "Comma is out of any object or array at row ";
            message += getRowPositionOfToken(positionOfComma);

            throw InvalidJsonSyntax(message);
        }

        if(previousToken != '\"' && previousToken != '}' && previousToken != ']') {
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
    validateQuotationMarkPlacement();
    validateColonPlacement();
    validateCommaPlacement();
}
