#pragma once

#include <vector>
#include <inttypes.h>

enum ParseResult
{
    PARSE_UNKNOWN = 0,
    PARSE_SUCESSFULL,
    PARSE_ERROR,
};

struct ParsedStr_t
{
    friend class StrParser;

private:
    const char *_buffer;
    std::vector<uint16_t> _indexes;

public:
    const char *at(uint16_t) const;
    int16_t count() const;
    const char *operator[](uint16_t) const;
};

class StrParser
{
private:
    std::vector<char> _separators;

public:
    explicit StrParser(const std::vector<char> &);

    ParseResult parse(char *, ParsedStr_t &);
};