#include "Name.h"

#include <algorithm>

using Name = Container::Name;
using CaseInsensitiveHash = Container::CaseInsensitiveHash;
using CaseInsensitiveEqual = Container::CaseInsensitiveEqual;

uint32_t MurmurHash3(const char* key, size_t len) {
    uint32_t h = 0;
    uint32_t c1 = 0xcc9e2d51;
    uint32_t c2 = 0x1b873593;
    uint32_t r1 = 15;
    uint32_t r2 = 13;
    uint32_t m = 5;
    uint32_t n = 0xe6546b64;

    for (size_t i = 0; i < len; i++) {
        uint32_t k = static_cast<uint32_t>(std::tolower(key[i]));
        k *= c1;
        k = (k << r1) | (k >> (32 - r1));  // Rotate left
        k *= c2;

        h ^= k;
        h = (h << r2) | (h >> (32 - r2));  // Rotate left
        h = h * m + n;
    }

    h ^= len;
    h ^= (h >> 16);
    h *= 0x85ebca6b;
    h ^= (h >> 13);
    h *= 0xc2b2ae35;
    h ^= (h >> 16);

    return h;
}

size_t CaseInsensitiveHash::operator()(const char* key) const
{
    const size_t len = strlen(key);
    return MurmurHash3(key, len);
}

size_t CaseInsensitiveHash::operator()(const std::string& key) const
{
    return MurmurHash3(key.c_str(), key.size());
}

size_t CaseInsensitiveHash::operator()(const Name& key) const
{
    return key.GetHash();
}

bool CaseInsensitiveEqual::operator()(const Name& key1, const Name& key2) const
{
    return key1.GetHash() == key2.GetHash();
}

bool CaseInsensitiveEqual::operator()(const Name& key1, const string& key2) const
{
    const uint64 key2Hash = CaseInsensitiveHash()(key2);
    return key1.GetHash() == key2Hash;
}

Name::Name()
{
    pStr = NewSPtr<const std::string>("");
    
    hash = INVALID_HASH;
}

Name::Name(const char* str)
{
    pStr = NewSPtr<const std::string>(str);

    hash  = CaseInsensitiveHash()(str);
}

Name::Name(const std::string& str)
{
    this->pStr = NewSPtr<const std::string>(str);
    
    hash = CaseInsensitiveHash()(*this->pStr);
}

Name::Name(std::string&& str)
{
    this->pStr = NewSPtr<const std::string>(std::move(str));

    hash = CaseInsensitiveHash()(*this->pStr);
}

Name::Name(const Name& other)
{
    pStr = other.pStr;
    hash = other.hash;
}

Name::Name(Name&& other) noexcept
{
    pStr = std::move(other.pStr);
    hash = other.hash;
}

Name& Name::operator=(const char* other)
{
    pStr = NewSPtr<string>(other);
    hash = CaseInsensitiveHash()(other);
    return *this;
}

Name& Name::operator=(const Name& other)
{
    if(other != *this)
    {
        pStr = other.pStr;
        hash = CaseInsensitiveHash()(other);
    }
    return *this;
}

Name& Name::operator=(const string& other)
{
    pStr = NewSPtr<const std::string>(other);
    hash = CaseInsensitiveHash()(other);
    return *this;
}