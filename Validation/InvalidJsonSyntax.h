#pragma once
#include "String.h"
#include <exception>

class InvalidJsonSyntax : public std::exception {
private:
    String _message;

public:
    explicit InvalidJsonSyntax(const char* message);
    explicit InvalidJsonSyntax(const String& message);

    const char* what() const noexcept override;
};
