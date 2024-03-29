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
    bool numberIsBeingBuilt = false;
    unsigned buildingOfKeywordTrue = 0;
    unsigned buildingOfKeywordFalse = 0;
    unsigned buildingOfKeywordNull = 0;

    unsigned previousPosition = in.tellg();

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
                _tokens += currentSymbol;
                stringHasNotBeenClosed = !stringHasNotBeenClosed;
                break;

            case 't':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateT(in, buildingOfKeywordTrue);

                _tokens += currentSymbol;
                break;

            case 'r':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateR(in, buildingOfKeywordTrue);

                _tokens += currentSymbol;
                break;

            case 'u':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateU(in, buildingOfKeywordTrue, buildingOfKeywordNull);

                _tokens += currentSymbol;
                break;

            case 'e':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateE(in, buildingOfKeywordTrue, buildingOfKeywordFalse);

                _tokens += currentSymbol;
                break;

            case 'f':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateF(in, buildingOfKeywordFalse);

                _tokens += currentSymbol;
                break;

            case 'a':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateA(in, buildingOfKeywordFalse);

                _tokens += currentSymbol;
                break;

            case 'l':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateL(in, buildingOfKeywordFalse, buildingOfKeywordNull);

                _tokens += currentSymbol;
                break;

            case 's':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateS(in, buildingOfKeywordFalse);

                _tokens += currentSymbol;
                break;

            case 'n':
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                validateN(in, buildingOfKeywordNull);

                _tokens += currentSymbol;
                break;

            case '-':
                validateBoolBuildingInterception(in, buildingOfKeywordTrue, buildingOfKeywordFalse, buildingOfKeywordNull);
                validateNumberBuildingInterception(in, numberIsBeingBuilt);

                numberIsBeingBuilt = true;
                _tokens += currentSymbol;
                break;

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
            case '.':
                validateBoolBuildingInterception(in, buildingOfKeywordTrue, buildingOfKeywordFalse, buildingOfKeywordNull);

                numberIsBeingBuilt = true;
                _tokens += currentSymbol;
                break;

            case '{':
            case '}':

            case '[':
            case ']':

            case ':':
            case ',':
                validateBoolBuildingInterception(in, buildingOfKeywordTrue, buildingOfKeywordFalse, buildingOfKeywordNull);

                numberIsBeingBuilt = false;
                _tokens += currentSymbol;
                break;

            case '\n':
                validateBoolBuildingInterception(in, buildingOfKeywordTrue, buildingOfKeywordFalse, buildingOfKeywordNull);
                validateNumberBuildingInterception(in, numberIsBeingBuilt);

                _tokens += currentSymbol;
                break;

            case ' ':
            case '\t':
                validateBoolBuildingInterception(in, buildingOfKeywordTrue, buildingOfKeywordFalse, buildingOfKeywordNull);
                validateNumberBuildingInterception(in, numberIsBeingBuilt);
                break;

            default:
                validateBoolBuildingInterception(in, buildingOfKeywordTrue, buildingOfKeywordFalse, buildingOfKeywordNull);
                validateNumberBuildingInterception(in, numberIsBeingBuilt);

                throwNonTokenCharacterException(in);
        }
    }
    in.clear();
    in.seekg(previousPosition);

    _tokenCount = _tokens.getLength();
}

void JsonValidator::throwOutOfPlaceCharacterException(std::ifstream& in) {
    String message("Out of place character at row ");
    message += getLinesCount(in, in.tellg());

    throw InvalidJsonSyntax(message);
}

void JsonValidator::throwNonTokenCharacterException(std::ifstream& in) {
    String message("Non-token character is out of quotation marks at row ");
    message += getLinesCount(in, in.tellg());

    throw InvalidJsonSyntax(message);
}

void JsonValidator::validateBoolBuildingInterception(std::ifstream& in, unsigned buildingOfKeywordTrue,
                                                     unsigned buildingOfKeywordFalse, unsigned buildingOfKeywordNull) {

    if(buildingOfKeywordTrue != 0 || buildingOfKeywordFalse != 0 || buildingOfKeywordNull != 0) {
        String message("Unfinished bool keyword at row ");
        message += getLinesCount(in, in.tellg());

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateNumberBuildingInterception(std::ifstream& in, bool numberIsBeingBuilt) {
    char nextToken = (char)in.peek();

    if(numberIsBeingBuilt && ((nextToken >= '0' && nextToken <= '9') || nextToken == '.')) {
        String message("Disallowed character in number at row ");
        message += getLinesCount(in, in.tellg());

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateT(std::ifstream& in, unsigned& buildingOfKeywordTrue) {
    if(buildingOfKeywordTrue != 0) {
        throwOutOfPlaceCharacterException(in);
    }
    buildingOfKeywordTrue = 1;
}

void JsonValidator::validateR(std::ifstream& in, unsigned& buildingOfKeywordTrue) {
    if(buildingOfKeywordTrue != 1) {
        throwOutOfPlaceCharacterException(in);
    }
    buildingOfKeywordTrue = 2;
}

void JsonValidator::validateU(std::ifstream& in, unsigned& buildingOfKeywordTrue, unsigned& buildingOfKeywordNull) {
    if(!(buildingOfKeywordTrue == 2 || buildingOfKeywordNull == 1)) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordTrue == 2 ? buildingOfKeywordTrue = 3 : buildingOfKeywordNull = 2;
}

void JsonValidator::validateE(std::ifstream& in, unsigned& buildingOfKeywordTrue, unsigned& buildingOfKeywordFalse) {
    if(!(buildingOfKeywordTrue == 3 || buildingOfKeywordFalse == 4)) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordTrue == 3 ? buildingOfKeywordTrue = 0 : buildingOfKeywordFalse = 0;
}

void JsonValidator::validateF(std::ifstream& in, unsigned& buildingOfKeywordFalse) {
    if(buildingOfKeywordFalse != 0) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse = 1;
}

void JsonValidator::validateA(std::ifstream& in, unsigned& buildingOfKeywordFalse) {
    if(buildingOfKeywordFalse != 1) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse = 2;
}

void JsonValidator::validateL(std::ifstream& in, unsigned& buildingOfKeywordFalse, unsigned& buildingOfKeywordNull) {
    if(!(buildingOfKeywordFalse == 2 || buildingOfKeywordNull == 2 || buildingOfKeywordNull == 3)) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse == 2 ? buildingOfKeywordFalse = 3 :
    (buildingOfKeywordNull == 2 ? buildingOfKeywordNull = 3 : buildingOfKeywordNull = 0);
}

void JsonValidator::validateS(std::ifstream& in, unsigned& buildingOfKeywordFalse) {
    if(buildingOfKeywordFalse != 3) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse = 4;
}

void JsonValidator::validateN(std::ifstream& in, unsigned& buildingOfKeywordNull) {
    if(buildingOfKeywordNull != 0) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordNull = 1;
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

//Function to find whether a token should follow json object syntax rules
//A recursive algorithm is implemented to pass through nested objects and nested arrays
bool JsonValidator::tokenIsInJsonObjectScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                             long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket,
                                             unsigned nestedJsonObjectCount, unsigned nestedJsonArrayCount) const {

    if(lastPositionOfOpenBrace > lastPositionOfClosedBrace && lastPositionOfOpenBrace > lastPositionOfOpenBracket
        && lastPositionOfOpenBrace > lastPositionOfClosedBracket) {

        if(nestedJsonObjectCount == 0 && nestedJsonArrayCount == 0) {
            return true;
        }

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfOpenBrace - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfOpenBrace - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfOpenBrace - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfOpenBrace - 1);


        return tokenIsInJsonObjectScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount - 1, nestedJsonArrayCount);
    }

    if(lastPositionOfOpenBracket > lastPositionOfClosedBracket && lastPositionOfOpenBracket > lastPositionOfOpenBrace
        && lastPositionOfOpenBracket > lastPositionOfClosedBrace) {

        if(nestedJsonObjectCount == 0 && nestedJsonArrayCount == 0) {
            return false;
        }

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfOpenBracket - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfOpenBracket - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfOpenBracket - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfOpenBracket - 1);


        return tokenIsInJsonObjectScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount, nestedJsonArrayCount - 1);
    }

    if(lastPositionOfClosedBrace > lastPositionOfOpenBrace && lastPositionOfClosedBrace > lastPositionOfOpenBracket
        && lastPositionOfClosedBrace > lastPositionOfClosedBracket) {

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfClosedBrace - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfClosedBrace - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfClosedBrace - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfClosedBrace - 1);


        return tokenIsInJsonObjectScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount + 1, nestedJsonArrayCount);
    }

    if(lastPositionOfClosedBracket > lastPositionOfOpenBracket && lastPositionOfClosedBracket > lastPositionOfOpenBrace
        && lastPositionOfClosedBracket > lastPositionOfClosedBrace) {

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfClosedBracket - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfClosedBracket - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfClosedBracket - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfClosedBracket - 1);


        return tokenIsInJsonObjectScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount, nestedJsonArrayCount + 1);
    }

    return false;
}

//Function to find whether a token should follow json array syntax rules
//A recursive algorithm is implemented to pass through nested objects and nested arrays
bool JsonValidator::tokenIsInJsonArrayScope(long long lastPositionOfOpenBrace, long long lastPositionOfClosedBrace,
                                            long long lastPositionOfOpenBracket, long long lastPositionOfClosedBracket,
                                            unsigned nestedJsonObjectCount, unsigned nestedJsonArrayCount) const {

    if(lastPositionOfOpenBrace > lastPositionOfClosedBrace && lastPositionOfOpenBrace > lastPositionOfOpenBracket
       && lastPositionOfOpenBrace > lastPositionOfClosedBracket) {

        if(nestedJsonObjectCount == 0 && nestedJsonArrayCount == 0) {
            return false;
        }

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfOpenBrace - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfOpenBrace - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfOpenBrace - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfOpenBrace - 1);


        return tokenIsInJsonArrayScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount - 1, nestedJsonArrayCount);
    }

    if(lastPositionOfOpenBracket > lastPositionOfClosedBracket && lastPositionOfOpenBracket > lastPositionOfOpenBrace
       && lastPositionOfOpenBracket > lastPositionOfClosedBrace) {

        if(nestedJsonObjectCount == 0 && nestedJsonArrayCount == 0) {
            return true;
        }

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfOpenBracket - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfOpenBracket - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfOpenBracket - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfOpenBracket - 1);


        return tokenIsInJsonArrayScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount, nestedJsonArrayCount - 1);
    }

    if(lastPositionOfClosedBrace > lastPositionOfOpenBrace && lastPositionOfClosedBrace > lastPositionOfOpenBracket
       && lastPositionOfClosedBrace > lastPositionOfClosedBracket) {

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfClosedBrace - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfClosedBrace - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfClosedBrace - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfClosedBrace - 1);


        return tokenIsInJsonArrayScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount + 1, nestedJsonArrayCount);
    }

    if(lastPositionOfClosedBracket > lastPositionOfOpenBracket && lastPositionOfClosedBracket > lastPositionOfOpenBrace
       && lastPositionOfClosedBracket > lastPositionOfClosedBrace) {

        long long newLastPositionOfOpenBrace = getLastPositionOfToken('{', lastPositionOfClosedBracket - 1);
        long long newLastPositionOfClosedBrace = getLastPositionOfToken('}', lastPositionOfClosedBracket - 1);
        long long newLastPositionOfOpenBracket = getLastPositionOfToken('[', lastPositionOfClosedBracket - 1);
        long long newLastPositionOfClosedBracket = getLastPositionOfToken(']', lastPositionOfClosedBracket - 1);


        return tokenIsInJsonArrayScope(newLastPositionOfOpenBrace, newLastPositionOfClosedBrace, newLastPositionOfOpenBracket,
                                        newLastPositionOfClosedBracket, nestedJsonObjectCount, nestedJsonArrayCount + 1);
    }

    return false;
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

    return token == '[' || token == ',' || token == '-' || token == '.' || (token >= '0' && token <= '9');
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
    unsigned numberOfOpenBraces = getCharCount(_tokens.getData(), _tokenCount, '{');
    unsigned numberOfClosedBraces = getCharCount(_tokens.getData(), _tokenCount, '}');

    if(numberOfOpenBraces < numberOfClosedBraces) {
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', _tokenCount - 1);

        String message("Extraneous closed brace at row ");
        message += getRowPositionOfToken(lastPositionOfClosedBrace);

        throw InvalidJsonSyntax(message);
    }

    if(numberOfOpenBraces > numberOfClosedBraces) {
        long long firstPositionOfOpenBrace = getPositionOfChar(_tokens.getData(), _tokenCount, '{', 1);

        String message("No matching closed brace of open brace at row ");
        message += getRowPositionOfToken(firstPositionOfOpenBrace);

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateBracePlacement() const {
    unsigned numberOfOpenBraces = getCharCount(_tokens.getData(), _tokenCount, '{');
    unsigned numberOfClosedBraces = getCharCount(_tokens.getData(), _tokenCount, '}');

    for(unsigned i = 0; i < numberOfOpenBraces; ++i) {
        long long positionOfOpenBrace = getPositionOfChar(_tokens.getData(), _tokenCount, '{', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfOpenBrace, 1);

        if(!validTokenBeforeOpenBrace(previousToken)) {
            String message("Out of place open brace at row ");
            message += getRowPositionOfToken(positionOfOpenBrace);

            throw InvalidJsonSyntax(message);
        }
    }

    for(unsigned i = 0; i < numberOfClosedBraces; ++i) {
        long long positionOfClosedBrace = getPositionOfChar(_tokens.getData(), _tokenCount, '}', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBrace, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfClosedBrace, 3);

        if(!validTokenBeforeClosedBrace(previousToken, tokenThreePositionsBack)) {
            String message("Out of place closed brace at row ");
            message += getRowPositionOfToken(positionOfClosedBrace);

            throw InvalidJsonSyntax(message);
        }

        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfClosedBrace - 1);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfClosedBrace - 1);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfClosedBrace - 1);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfClosedBrace - 1);

        //To avoid {[}] cases for example
        if(lastPositionOfOpenBracket > lastPositionOfOpenBrace && lastPositionOfOpenBracket > lastPositionOfClosedBrace
            && lastPositionOfOpenBracket > lastPositionOfClosedBracket) {

            String message("Out of place closed brace at row ");
            message += getRowPositionOfToken(positionOfClosedBrace);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateBracketMatching() const {
    unsigned numberOfOpenBrackets = getCharCount(_tokens.getData(), _tokenCount, '[');
    unsigned numberOfClosedBrackets = getCharCount(_tokens.getData(), _tokenCount, ']');

    if(numberOfOpenBrackets < numberOfClosedBrackets) {
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', _tokenCount - 1);

        String message("Extraneous closed bracket at row ");
        message += getRowPositionOfToken(lastPositionOfClosedBracket);

        throw InvalidJsonSyntax(message);
    }

    if(numberOfOpenBrackets > numberOfClosedBrackets) {
        long long firstPositionOfOpenBracket = getPositionOfChar(_tokens.getData(), _tokenCount, '[', 1);

        String message("No matching closed bracket at row ");
        message += getRowPositionOfToken(firstPositionOfOpenBracket);

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateBracketPlacement() const {
    unsigned numberOfOpenBrackets = getCharCount(_tokens.getData(), _tokenCount, '[');
    unsigned numberOfClosedBrackets = getCharCount(_tokens.getData(), _tokenCount, ']');

    for(unsigned i = 0; i < numberOfOpenBrackets; ++i) {
        long long positionOfOpenBracket = getPositionOfChar(_tokens.getData(), _tokenCount, '[', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfOpenBracket, 1);

        if(!validTokenBeforeOpenBracket(previousToken)) {
            String message("Out of place open bracket at row ");
            message += getRowPositionOfToken(positionOfOpenBracket);

            throw InvalidJsonSyntax(message);
        }
    }

    for(unsigned i = 0; i < numberOfClosedBrackets; ++i) {
        long long positionOfClosedBracket = getPositionOfChar(_tokens.getData(), _tokenCount, ']', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBracket, 1);

        if(!validTokenBeforeClosedBracket(previousToken)) {
            String message("Out of place closed bracket at row ");
            message += getRowPositionOfToken(positionOfClosedBracket);

            throw InvalidJsonSyntax(message);
        }

        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfClosedBracket - 1);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfClosedBracket - 1);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfClosedBracket - 1);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfClosedBracket - 1);

        //To avoid [{]} cases for example
        if(lastPositionOfOpenBrace > lastPositionOfOpenBracket && lastPositionOfOpenBrace > lastPositionOfClosedBrace
           && lastPositionOfOpenBrace > lastPositionOfClosedBracket) {

            String message("Out of place closed brace at row ");
            message += getRowPositionOfToken(positionOfClosedBracket);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateDigitPlacement() const {

    //Validating zeros then ones then twos etc.
    for(unsigned i = 0; i < NUMBER_OF_DIGITS; ++i) {
        unsigned currentDigitCount = getCharCount(_tokens.getData(), _tokenCount, '0' + i);

        for(unsigned j = 0; j < currentDigitCount; ++j) {
            long long positionOfCurrentDigit = getPositionOfChar(_tokens.getData(), _tokenCount, '0' + i, j + 1);
            long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfCurrentDigit);
            long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfCurrentDigit);
            long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfCurrentDigit);
            long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfCurrentDigit);

            char previousToken = getPrecedingNonNewLineTokens(positionOfCurrentDigit, 1);
            char nextToken = getNextNonNewLineToken(positionOfCurrentDigit);

            if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {

                if(('0' + i) == '0' && (!validZeroPlacementInJsonObject(previousToken, nextToken) || !validTokenBeforeDigitInJsonObject(previousToken))) {
                    String message("Out of place 0 at row ");
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }

                if(!validTokenBeforeDigitInJsonObject(previousToken)) {
                    String message("Out of place digit at row ");
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }
            }

            if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {

                if(('0' + i) == '0' && (!validZeroPlacementInJsonArray(previousToken, nextToken) || !validTokenBeforeDigitInJsonArray(previousToken))) {
                    String message("Out of place 0 at row ");
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }

                if(!validTokenBeforeDigitInJsonArray(previousToken)) {
                    String message("Out of place digit at row ");
                    message += getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }
            }
        }
    }
}

void JsonValidator::validateDotPlacement() const {
    unsigned numberOfDots = getCharCount(_tokens.getData(), _tokenCount, '.');

    for(unsigned i = 0; i < numberOfDots; ++i) {
        long long positionOfDot = getPositionOfChar(_tokens.getData(), _tokenCount, '.', i + 1);

        char previousToken = getPrecedingNonNewLineTokens(positionOfDot, 1);
        char nextToken = getNextNonNewLineToken(positionOfDot);

        if(!validTokenBeforeDot(previousToken, nextToken)) {
            String message("Out of place dot at row ");
            message += getRowPositionOfToken(positionOfDot);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateMinusPlacement() const {
    unsigned numberOfMinuses = getCharCount(_tokens.getData(), _tokenCount, '-');

    for(unsigned i = 0; i < numberOfMinuses; ++i) {
        long long positionOfMinuses = getPositionOfChar(_tokens.getData(), _tokenCount, '-', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfMinuses);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfMinuses);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfMinuses);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfMinuses);

        char previousToken = getPrecedingNonNewLineTokens(positionOfMinuses, 1);
        char nextToken = getNextNonNewLineToken(positionOfMinuses);

        if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
           && !validTokenBeforeMinusInJsonObject(previousToken, nextToken)) {

            String message("Out of place minus at row ");
            message += getRowPositionOfToken(positionOfMinuses);

            throw InvalidJsonSyntax(message);
        }

        if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
           && !validTokenBeforeMinusInJsonArray(previousToken, nextToken)) {

            String message("Out of place minus at row ");
            message += getRowPositionOfToken(positionOfMinuses);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateValueKeywordPlacement() const {

    //Validating true then false then null keywords
    for(unsigned i = 0; i < NUMBER_OF_JSON_KEYWORDS; ++i) {
        char firstCharacterOfKeyword = 0;
        unsigned currentKeywordCount = 0;

        switch(i) {
            case 0:
                firstCharacterOfKeyword = 't';
                currentKeywordCount = getCharCount(_tokens.getData(), _tokenCount, 't');
                break;
            case 1:
                firstCharacterOfKeyword = 'f';
                currentKeywordCount = getCharCount(_tokens.getData(), _tokenCount, 'f');
                break;
            case 2:
                firstCharacterOfKeyword = 'n';
                currentKeywordCount = getCharCount(_tokens.getData(), _tokenCount, 'n');
                break;
            default:
                throw std::runtime_error("No implementation for validation of new keyword");
        }

        for(unsigned j = 0; j < currentKeywordCount; ++j) {
            long long positionOfKeyword = getPositionOfChar(_tokens.getData(), _tokenCount, firstCharacterOfKeyword, j + 1);
            long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfKeyword);
            long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfKeyword);
            long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfKeyword);
            long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfKeyword);

            char previousToken = getPrecedingNonNewLineTokens(positionOfKeyword, 1);

            if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
               && !validTokenBeforeValueKeywordInJsonObject(previousToken)) {

                String message("Out of place keyword at row ");
                message += getRowPositionOfToken(positionOfKeyword);

                throw InvalidJsonSyntax(message);
            }

            if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
               && !validTokenBeforeValueKeywordInJsonArray(previousToken)) {

                String message("Out of place keyword at row ");
                message += getRowPositionOfToken(positionOfKeyword);

                throw InvalidJsonSyntax(message);
            }
        }
    }

}

void JsonValidator::validateQuotationMarkPlacement() const {
    unsigned numberOfQuotationMarks = getCharCount(_tokens.getData(), _tokenCount, '\"');

    for(unsigned i = 0; i < numberOfQuotationMarks; ++i) {
        long long positionOfQuotationMark = getPositionOfChar(_tokens.getData(), _tokenCount, '\"', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfQuotationMark, 1);
        char tokenTwoPositionsBack = getPrecedingNonNewLineTokens(positionOfQuotationMark, 2);

        if(!validTokenBeforeQuotationMark(previousToken, tokenTwoPositionsBack)) {
            String message("Out of place quotation mark at row ");
            message += getRowPositionOfToken(positionOfQuotationMark);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateColonPlacement() const {
    unsigned numberOfColons = getCharCount(_tokens.getData(), _tokenCount, ':');

    for(unsigned i = 0; i < numberOfColons; ++i) {
        long long positionOfColon = getPositionOfChar(_tokens.getData(), _tokenCount, ':', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfColon);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfColon);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfColon);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfColon);

        char previousToken = getPrecedingNonNewLineTokens(positionOfColon, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfColon, 3);

        if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {
            String message("Out of place colon at row ");
            message += getRowPositionOfToken(positionOfColon);

            throw InvalidJsonSyntax(message);
        }

        if(!validTokensPrecedingColonInJsonObject(previousToken, tokenThreePositionsBack)) {
            String message("Out of place colon at row ");
            message += getRowPositionOfToken(positionOfColon);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateCommaPlacement() const {
    unsigned numberOfCommas = getCharCount(_tokens.getData(), _tokenCount, ',');

    for(unsigned i = 0; i < numberOfCommas; ++i) {
        long long positionOfComma = getPositionOfChar(_tokens.getData(), _tokenCount, ',', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfComma);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfComma);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfComma);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfComma);

        char previousToken = getPrecedingNonNewLineTokens(positionOfComma, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfComma, 3);

        if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {

            if(!validTokensPrecedingCommaInJsonObject(previousToken, tokenThreePositionsBack)) {
                String message("Out of place comma at row ");
                message += getRowPositionOfToken(positionOfComma);

                throw InvalidJsonSyntax(message);
            }
        }

        if(!validTokensPrecedingCommaInJsonArray(previousToken)) {
            String message("Out of place comma at row ");
            message += getRowPositionOfToken(positionOfComma);

            throw InvalidJsonSyntax(message);
        }
    }
}

JsonValidator::JsonValidator(std::ifstream& in) {
    setTokens(in);
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
