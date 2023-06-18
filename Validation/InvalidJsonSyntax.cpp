#include "InvalidJsonSyntax.h"

InvalidJsonSyntax::InvalidJsonSyntax(const char* message) {
    _message = String(message);
}

InvalidJsonSyntax::InvalidJsonSyntax(const String& message) {
    _message = message;
}

const char* InvalidJsonSyntax::what() const noexcept {

    return _message.getData();
}
