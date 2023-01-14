#include "strParser.h"
#include <cstring>
#include "out.h"

const char *ParsedStr_t::at(uint16_t i) const
{
    return (_buffer + _indexes[i]);
}

const char *ParsedStr_t::operator[](uint16_t i) const
{
    return at(i);
}

int16_t ParsedStr_t::count() const
{
    return _indexes.size();
}

StrParser::StrParser(const std::vector<char> &sep) : _separators(sep)
{
}

ParseResult StrParser::parse(char *str, ParsedStr_t &out)
{
    uint16_t size = strlen(str);
    uint16_t i{0};
    bool is_last_sep{true};
    while (i < size)
    {
        bool isSep = false;
        for (auto j : _separators)
        {
            if (str[i] == j)
            {
                isSep = true;
                break;
            }
        }
        if (isSep)
        {
            str[i] = '\0';
            is_last_sep = true;
        }
        else if (is_last_sep)
        {
            out._indexes.push_back(i);
            is_last_sep = false;
        }
        i++;
    }
    str[i] = '\0';
    out._buffer = str;
    return ParseResult::PARSE_SUCESSFULL;
}
