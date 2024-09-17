#include "JsonValidator.h"
#include "InvalidJsonSyntax.h"
#include "fileFunctions.h"
#include "helperFunctions.h"

void JsonValidator::assertIndex(long long index) const {
    if(index < 0 || index > (_tokens.getLength() - 1)) {
        throw std::out_of_range("Error, index is out of range");
    }
}

void JsonValidator::setTokens(std::ifstream& in) {
    bool stringHasNotBeenClosed = false;
    bool numberIsBeingBuilt = false;
    long long buildingOfKeywordTrue = 0;
    long long buildingOfKeywordFalse = 0;
    long long buildingOfKeywordNull = 0;

    long long previousPosition = in.tellg();

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

void JsonValidator::validateBoolBuildingInterception(std::ifstream& in, long long buildingOfKeywordTrue,
                                                     long long buildingOfKeywordFalse, long long buildingOfKeywordNull) {

    if(buildingOfKeywordTrue != 0 || buildingOfKeywordFalse != 0 || buildingOfKeywordNull != 0) {
        String message("Unfinished bool keyword at row ");
        message += getLinesCount(in, in.tellg());

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateNumberBuildingInterception(std::ifstream& in, bool numberIsBeingBuilt) {
    char nextToken = (char)in.peek();

    if(numberIsBeingBuilt && (isDigit(nextToken) || nextToken == '.')) {
        String message("Disallowed character in number at row ");
        message += getLinesCount(in, in.tellg());

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateT(std::ifstream& in, long long& buildingOfKeywordTrue) {
    if(buildingOfKeywordTrue != 0) {
        throwOutOfPlaceCharacterException(in);
    }
    buildingOfKeywordTrue = 1;
}

void JsonValidator::validateR(std::ifstream& in, long long& buildingOfKeywordTrue) {
    if(buildingOfKeywordTrue != 1) {
        throwOutOfPlaceCharacterException(in);
    }
    buildingOfKeywordTrue = 2;
}

void JsonValidator::validateU(std::ifstream& in, long long& buildingOfKeywordTrue, long long& buildingOfKeywordNull) {
    if(!(buildingOfKeywordTrue == 2 || buildingOfKeywordNull == 1)) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordTrue == 2 ? buildingOfKeywordTrue = 3 : buildingOfKeywordNull = 2;
}

void JsonValidator::validateE(std::ifstream& in, long long& buildingOfKeywordTrue, long long& buildingOfKeywordFalse) {
    if(!(buildingOfKeywordTrue == 3 || buildingOfKeywordFalse == 4)) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordTrue == 3 ? buildingOfKeywordTrue = 0 : buildingOfKeywordFalse = 0;
}

void JsonValidator::validateF(std::ifstream& in, long long& buildingOfKeywordFalse) {
    if(buildingOfKeywordFalse != 0) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse = 1;
}

void JsonValidator::validateA(std::ifstream& in, long long& buildingOfKeywordFalse) {
    if(buildingOfKeywordFalse != 1) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse = 2;
}

void JsonValidator::validateL(std::ifstream& in, long long& buildingOfKeywordFalse, long long& buildingOfKeywordNull) {
    if(!(buildingOfKeywordFalse == 2 || buildingOfKeywordNull == 2 || buildingOfKeywordNull == 3)) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse == 2 ? buildingOfKeywordFalse = 3 :
    (buildingOfKeywordNull == 2 ? buildingOfKeywordNull = 3 : buildingOfKeywordNull = 0);
}

void JsonValidator::validateS(std::ifstream& in, long long& buildingOfKeywordFalse) {
    if(buildingOfKeywordFalse != 3) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordFalse = 4;
}

void JsonValidator::validateN(std::ifstream& in, long long& buildingOfKeywordNull) {
    if(buildingOfKeywordNull != 0) {
        throwOutOfPlaceCharacterException(in);
    }

    buildingOfKeywordNull = 1;
}

long long JsonValidator::getLastPositionOfToken(char token, long long fromIndex) const {
    assertIndex(fromIndex);

    for(long long i = 0; i <= fromIndex; ++i) {
        if(_tokens[fromIndex - i] == token) {
            return fromIndex - i;
        }
    }

    return -1;
}

char JsonValidator::getPrecedingNonNewLineTokens(long long index, long long tokensBack) const {
    assertIndex(index);
    size_t tokenCount = _tokens.getLength();

    if(tokensBack == 0) {
        return _tokens[index];
    }

    if(index == 0) {
        return 0;
    }

    index--;

    while(index < tokenCount) {
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

char JsonValidator::getNextNonNewLineToken(long long index) const {
    assertIndex(index + 1);
    size_t tokenCount = _tokens.getLength();

    index++;

    while(index < tokenCount) {
        if(_tokens[index] != '\n') {
            return _tokens[index];
        }
        index++;
    }

    return 0;
}

long long JsonValidator::getRowPositionOfToken(long long index) const {
    assertIndex(index);
    long long rows = 1;

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
                                             long long nestedJsonObjectCount, long long nestedJsonArrayCount) const {

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
                                            long long nestedJsonObjectCount, long long nestedJsonArrayCount) const {

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

    return token == '}' || token == '\"' || token == '[' || token == ']' || token == 'e' || token == 'l' || isDigit(token);
}

bool JsonValidator::validZeroPlacementInJsonObject(char previousToken, char nextToken) {

    return !((previousToken == ':' || previousToken == '-') && !(nextToken == '.' || nextToken == ',')) && validTokenBeforeDigitInJsonObject(previousToken);
}

bool JsonValidator::validZeroPlacementInJsonArray(char previousToken, char nextToken) {

    return !((previousToken == ','|| previousToken == '-') && nextToken != '.') && validTokenBeforeDigitInJsonArray(previousToken);
}

bool JsonValidator::validTokenBeforeDigitInJsonObject(char token) {

    return token == ':' || token == '-' || token == '.' || isDigit(token);
}

bool JsonValidator::validTokenBeforeDigitInJsonArray(char token) {

    return token == '[' || token == ',' || token == '-' || token == '.' || isDigit(token);
}

bool JsonValidator::validTokenBeforeMinusInJsonObject(char previousToken, char nextToken) {

    return previousToken == ':' && isDigit(nextToken);
}

bool JsonValidator::validTokenBeforeMinusInJsonArray(char previousToken, char nextToken) {

    return (previousToken == ',' || previousToken == '[') && isDigit(nextToken);
}

bool JsonValidator::validTokenBeforeDecimalPoint(char previousToken, char nextToken) {

    return isDigit(previousToken) && isDigit(nextToken);
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

    return token == '\"' || token == '}' || token == ']' || token == 'e' || token == 'l' || isDigit(token);
}

void JsonValidator::validateBraceMatching() const {
    long long numberOfOpenBraces = _tokens.getCharCount('{');
    long long numberOfClosedBraces = _tokens.getCharCount('}');

    if(numberOfOpenBraces < numberOfClosedBraces) {
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', _tokens.getLength() - 1);

        String message("Extraneous closed brace at row ");
        message += (size_t)getRowPositionOfToken(lastPositionOfClosedBrace);

        throw InvalidJsonSyntax(message);
    }

    if(numberOfOpenBraces > numberOfClosedBraces) {
        long long firstPositionOfOpenBrace = _tokens.getPositionOfChar('{', 1);

        String message("No matching closed brace of open brace at row ");
        message += (size_t)getRowPositionOfToken(firstPositionOfOpenBrace);

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateBracePlacement() const {
    long long numberOfOpenBraces = _tokens.getCharCount('{');
    long long numberOfClosedBraces = _tokens.getCharCount('}');

    for(long long i = 0; i < numberOfOpenBraces; ++i) {
        long long positionOfOpenBrace = _tokens.getPositionOfChar('{', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfOpenBrace, 1);

        if(!validTokenBeforeOpenBrace(previousToken)) {
            String message("Out of place open brace at row ");
            message += (size_t)getRowPositionOfToken(positionOfOpenBrace);

            throw InvalidJsonSyntax(message);
        }
    }

    for(long long i = 0; i < numberOfClosedBraces; ++i) {
        long long positionOfClosedBrace = _tokens.getPositionOfChar('}', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBrace, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfClosedBrace, 3);

        if(!validTokenBeforeClosedBrace(previousToken, tokenThreePositionsBack)) {
            String message("Out of place closed brace at row ");
            message += (size_t)getRowPositionOfToken(positionOfClosedBrace);

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
            message += (size_t)getRowPositionOfToken(positionOfClosedBrace);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateBracketMatching() const {
    long long numberOfOpenBrackets = _tokens.getCharCount('[');
    long long numberOfClosedBrackets = _tokens.getCharCount(']');

    if(numberOfOpenBrackets < numberOfClosedBrackets) {
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', _tokens.getLength() - 1);

        String message("Extraneous closed bracket at row ");
        message += (size_t)getRowPositionOfToken(lastPositionOfClosedBracket);

        throw InvalidJsonSyntax(message);
    }

    if(numberOfOpenBrackets > numberOfClosedBrackets) {
        long long firstPositionOfOpenBracket = _tokens.getPositionOfChar('[', 1);

        String message("No matching closed bracket at row ");
        message += (size_t)getRowPositionOfToken(firstPositionOfOpenBracket);

        throw InvalidJsonSyntax(message);
    }
}

void JsonValidator::validateBracketPlacement() const {
    long long numberOfOpenBrackets = _tokens.getCharCount('[');
    long long numberOfClosedBrackets = _tokens.getCharCount(']');

    for(long long i = 0; i < numberOfOpenBrackets; ++i) {
        long long positionOfOpenBracket = _tokens.getPositionOfChar('[', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfOpenBracket, 1);

        if(!validTokenBeforeOpenBracket(previousToken)) {
            String message("Out of place open bracket at row ");
            message += (size_t)getRowPositionOfToken(positionOfOpenBracket);

            throw InvalidJsonSyntax(message);
        }
    }

    for(long long i = 0; i < numberOfClosedBrackets; ++i) {
        long long positionOfClosedBracket = _tokens.getPositionOfChar(']', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfClosedBracket, 1);

        if(!validTokenBeforeClosedBracket(previousToken)) {
            String message("Out of place closed bracket at row ");
            message += (size_t)getRowPositionOfToken(positionOfClosedBracket);

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
            message += (size_t)getRowPositionOfToken(positionOfClosedBracket);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateDigitPlacement() const {

    //Validating zeros then ones then twos etc.
    for(long long i = 0; i < NUMBER_OF_DIGITS; ++i) {
        long long currentDigitCount = _tokens.getCharCount('0' + i);

        for(long long j = 0; j < currentDigitCount; ++j) {
            long long positionOfCurrentDigit = _tokens.getPositionOfChar('0' + i, j + 1);
            long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfCurrentDigit);
            long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfCurrentDigit);
            long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfCurrentDigit);
            long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfCurrentDigit);

            char previousToken = getPrecedingNonNewLineTokens(positionOfCurrentDigit, 1);
            char nextToken = getNextNonNewLineToken(positionOfCurrentDigit);

            if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {

                if(('0' + i) == '0' && (!validZeroPlacementInJsonObject(previousToken, nextToken) || !validTokenBeforeDigitInJsonObject(previousToken))) {
                    String message("Out of place 0 at row ");
                    message += (size_t)getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }

                if(!validTokenBeforeDigitInJsonObject(previousToken)) {
                    String message("Out of place digit at row ");
                    message += (size_t)getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }
            }

            if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {

                if(('0' + i) == '0' && (!validZeroPlacementInJsonArray(previousToken, nextToken) || !validTokenBeforeDigitInJsonArray(previousToken))) {
                    String message("Out of place 0 at row ");
                    message += (size_t)getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }

                if(!validTokenBeforeDigitInJsonArray(previousToken)) {
                    String message("Out of place digit at row ");
                    message += (size_t)getRowPositionOfToken(positionOfCurrentDigit);

                    throw InvalidJsonSyntax(message);
                }
            }
        }
    }
}

void JsonValidator::validateDecimalPointPlacement() const {
    long long numberOfDecimalPoints = _tokens.getCharCount('.');

    for(long long i = 0; i < numberOfDecimalPoints; ++i) {
        long long positionOfDecimalPoint = _tokens.getPositionOfChar('.', i + 1);

        char previousToken = getPrecedingNonNewLineTokens(positionOfDecimalPoint, 1);
        char nextToken = getNextNonNewLineToken(positionOfDecimalPoint);

        if(!validTokenBeforeDecimalPoint(previousToken, nextToken)) {
            String message("Out of place decimal point at row ");
            message += (size_t)getRowPositionOfToken(positionOfDecimalPoint);

            throw InvalidJsonSyntax(message);
        }

        for(long long j = 2; true; ++j) {
            previousToken = getPrecedingNonNewLineTokens(positionOfDecimalPoint, j);

            if(previousToken == '.') {
                String message("Multiple decimal points in number at row ");
                message += (size_t)getRowPositionOfToken(positionOfDecimalPoint);

                throw InvalidJsonSyntax(message);
            }

            if(!isDigit(previousToken)) {
                break;
            }
        }

    }
}

void JsonValidator::validateMinusPlacement() const {
    long long numberOfMinuses = _tokens.getCharCount('-');

    for(long long i = 0; i < numberOfMinuses; ++i) {
        long long positionOfMinuses = _tokens.getPositionOfChar('-', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfMinuses);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfMinuses);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfMinuses);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfMinuses);

        char previousToken = getPrecedingNonNewLineTokens(positionOfMinuses, 1);
        char nextToken = getNextNonNewLineToken(positionOfMinuses);

        if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
           && !validTokenBeforeMinusInJsonObject(previousToken, nextToken)) {

            String message("Out of place minus at row ");
            message += (size_t)getRowPositionOfToken(positionOfMinuses);

            throw InvalidJsonSyntax(message);
        }

        if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
           && !validTokenBeforeMinusInJsonArray(previousToken, nextToken)) {

            String message("Out of place minus at row ");
            message += (size_t)getRowPositionOfToken(positionOfMinuses);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateValueKeywordPlacement() const {

    //Validating true then false then null keywords
    for(long long i = 0; i < NUMBER_OF_JSON_KEYWORDS; ++i) {
        char firstCharacterOfKeyword = 0;
        long long currentKeywordCount = 0;

        switch(i) {
            case 0:
                firstCharacterOfKeyword = 't';
                currentKeywordCount = _tokens.getCharCount('t');
                break;
            case 1:
                firstCharacterOfKeyword = 'f';
                currentKeywordCount = _tokens.getCharCount('f');
                break;
            case 2:
                firstCharacterOfKeyword = 'n';
                currentKeywordCount = _tokens.getCharCount('n');
                break;
            default:
                throw std::runtime_error("No implementation for validation of new keyword");
        }

        for(long long j = 0; j < currentKeywordCount; ++j) {
            long long positionOfKeyword = _tokens.getPositionOfChar(firstCharacterOfKeyword, j + 1);
            long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfKeyword);
            long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfKeyword);
            long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfKeyword);
            long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfKeyword);

            char previousToken = getPrecedingNonNewLineTokens(positionOfKeyword, 1);

            if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
               && !validTokenBeforeValueKeywordInJsonObject(previousToken)) {

                String message("Out of place keyword at row ");
                message += (size_t)getRowPositionOfToken(positionOfKeyword);

                throw InvalidJsonSyntax(message);
            }

            if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)
               && !validTokenBeforeValueKeywordInJsonArray(previousToken)) {

                String message("Out of place keyword at row ");
                message += (size_t)getRowPositionOfToken(positionOfKeyword);

                throw InvalidJsonSyntax(message);
            }
        }
    }

}

void JsonValidator::validateQuotationMarkPlacement() const {
    long long numberOfQuotationMarks = _tokens.getCharCount('\"');

    for(long long i = 0; i < numberOfQuotationMarks; ++i) {
        long long positionOfQuotationMark = _tokens.getPositionOfChar('\"', i + 1);
        char previousToken = getPrecedingNonNewLineTokens(positionOfQuotationMark, 1);
        char tokenTwoPositionsBack = getPrecedingNonNewLineTokens(positionOfQuotationMark, 2);

        if(!validTokenBeforeQuotationMark(previousToken, tokenTwoPositionsBack)) {
            String message("Out of place quotation mark at row ");
            message += (size_t)getRowPositionOfToken(positionOfQuotationMark);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateColonPlacement() const {
    long long numberOfColons = _tokens.getCharCount(':');

    for(long long i = 0; i < numberOfColons; ++i) {
        long long positionOfColon = _tokens.getPositionOfChar(':', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfColon);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfColon);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfColon);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfColon);

        char previousToken = getPrecedingNonNewLineTokens(positionOfColon, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfColon, 3);

        if(tokenIsInJsonArrayScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {
            String message("Out of place colon at row ");
            message += (size_t)getRowPositionOfToken(positionOfColon);

            throw InvalidJsonSyntax(message);
        }

        if(!validTokensPrecedingColonInJsonObject(previousToken, tokenThreePositionsBack)) {
            String message("Out of place colon at row ");
            message += (size_t)getRowPositionOfToken(positionOfColon);

            throw InvalidJsonSyntax(message);
        }
    }
}

void JsonValidator::validateCommaPlacement() const {
    long long numberOfCommas = _tokens.getCharCount(',');

    for(long long i = 0; i < numberOfCommas; ++i) {
        long long positionOfComma = _tokens.getPositionOfChar(',', i + 1);
        long long lastPositionOfOpenBrace = getLastPositionOfToken('{', positionOfComma);
        long long lastPositionOfClosedBrace = getLastPositionOfToken('}', positionOfComma);
        long long lastPositionOfOpenBracket = getLastPositionOfToken('[', positionOfComma);
        long long lastPositionOfClosedBracket = getLastPositionOfToken(']', positionOfComma);

        char previousToken = getPrecedingNonNewLineTokens(positionOfComma, 1);
        char tokenThreePositionsBack = getPrecedingNonNewLineTokens(positionOfComma, 3);

        if(tokenIsInJsonObjectScope(lastPositionOfOpenBrace, lastPositionOfClosedBrace, lastPositionOfOpenBracket, lastPositionOfClosedBracket, 0, 0)) {

            if(!validTokensPrecedingCommaInJsonObject(previousToken, tokenThreePositionsBack)) {
                String message("Out of place comma at row ");
                message += (size_t)getRowPositionOfToken(positionOfComma);

                throw InvalidJsonSyntax(message);
            }
        }

        if(!validTokensPrecedingCommaInJsonArray(previousToken)) {
            String message("Out of place comma at row ");
            message += (size_t)getRowPositionOfToken(positionOfComma);

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
    validateDecimalPointPlacement();
    validateMinusPlacement();
    validateValueKeywordPlacement();

    validateQuotationMarkPlacement();
    validateColonPlacement();
    validateCommaPlacement();
}
