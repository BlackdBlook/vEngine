#include "Name.h"

#include <algorithm>
#include <ratio>

using Name = Container::Name;
using CaseInsensitiveHash = Container::CaseInsensitiveHash;
using CaseInsensitiveEqual = Container::CaseInsensitiveEqual;


class NameStringCache
{
    static std::unordered_map<uint64, WPtr<const string>> StringCache;

    static bool CheckHash();

public:
    static SPtr<const string> GetCachedString(const string& str);
    static SPtr<const string> GetCachedString(string&& str);
};


std::unordered_map<uint64, WPtr<const string>> NameStringCache::StringCache;

bool NameStringCache::CheckHash()
{
    for(auto it = StringCache.begin(); it != StringCache.end();)
    {
        if(it->second.expired())
        {
            it = StringCache.erase(it);
        }else
        {
            ++it;
        }
    }
    return true;
}

inline uint64_t getblock64 ( const uint64_t * p, int i )
{
    return p[i];
}

inline uint64_t rotl64 ( uint64_t x, int8_t r )
{
    return (x << r) | (x >> (64 - r));
}

#define ROTL64(x,y)	rotl64(x,y)

#define BIG_CONSTANT(x) (x##LLU)

SPtr<const string> NameStringCache::GetCachedString(const string& str)
{
    CheckHash();
    const uint64 hash = CaseInsensitiveHash()(str);
    auto s = StringCache.find(hash);
    if(s == StringCache.end() || s->second.expired())
    {
        auto pStr = NewSPtr<const string>(str);
        StringCache.insert({hash, pStr});
        return pStr;
    }else
    {
        return s->second.lock();
    }
}

SPtr<const string> NameStringCache::GetCachedString(string&& str)
{
    CheckHash();
    const uint64 hash = CaseInsensitiveHash()(str);
    auto s = StringCache.find(hash);
    if(s == StringCache.end() || s->second.expired())
    {
        auto pStr = NewSPtr<const string>(std::move(str));
        StringCache.insert({hash, pStr});
        return pStr;
    }else
    {
        return s->second.lock();
    }
}

inline uint64_t fmix64 ( uint64_t k )
{
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xff51afd7ed558ccd);
    k ^= k >> 33;
    k *= BIG_CONSTANT(0xc4ceb9fe1a85ec53);
    k ^= k >> 33;

    return k;
}

void MurmurHash3_x64_128 ( const void * key, const size_t len,
                           const uint64_t seed, void * out )
{
    const uint8_t * data = (const uint8_t*)key;
    const size_t nblocks = len / 16;

    uint64_t h1 = seed;
    uint64_t h2 = seed;

    const uint64_t c1 = BIG_CONSTANT(0x87c37b91114253d5);
    const uint64_t c2 = BIG_CONSTANT(0x4cf5ad432745937f);

    //----------
    // body

    const uint64_t * blocks = (const uint64_t *)(data);

    for(int i = 0; i < nblocks; i++)
    {
        uint64_t k1 = getblock64(blocks,i*2+0);
        uint64_t k2 = getblock64(blocks,i*2+1);

        k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;

        h1 = ROTL64(h1,27); h1 += h2; h1 = h1*5+0x52dce729;

        k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

        h2 = ROTL64(h2,31); h2 += h1; h2 = h2*5+0x38495ab5;
    }

    //----------
    // tail

    const uint8_t * tail = (const uint8_t*)(data + nblocks*16);

    uint64_t k1 = 0;
    uint64_t k2 = 0;

    switch(len & 15)
    {
    case 15: k2 ^= ((uint64_t)tail[14]) << 48;
    case 14: k2 ^= ((uint64_t)tail[13]) << 40;
    case 13: k2 ^= ((uint64_t)tail[12]) << 32;
    case 12: k2 ^= ((uint64_t)tail[11]) << 24;
    case 11: k2 ^= ((uint64_t)tail[10]) << 16;
    case 10: k2 ^= ((uint64_t)tail[ 9]) << 8;
    case  9: k2 ^= ((uint64_t)tail[ 8]) << 0;
        k2 *= c2; k2  = ROTL64(k2,33); k2 *= c1; h2 ^= k2;

    case  8: k1 ^= ((uint64_t)tail[ 7]) << 56;
    case  7: k1 ^= ((uint64_t)tail[ 6]) << 48;
    case  6: k1 ^= ((uint64_t)tail[ 5]) << 40;
    case  5: k1 ^= ((uint64_t)tail[ 4]) << 32;
    case  4: k1 ^= ((uint64_t)tail[ 3]) << 24;
    case  3: k1 ^= ((uint64_t)tail[ 2]) << 16;
    case  2: k1 ^= ((uint64_t)tail[ 1]) << 8;
    case  1: k1 ^= ((uint64_t)tail[ 0]) << 0;
        k1 *= c1; k1  = ROTL64(k1,31); k1 *= c2; h1 ^= k1;
    };

    //----------
    // finalization

    h1 ^= len; h2 ^= len;

    h1 += h2;
    h2 += h1;

    h1 = fmix64(h1);
    h2 = fmix64(h2);

    h1 += h2;
    h2 += h1;

    ((uint64_t*)out)[0] = h1;
    // ((uint64_t*)out)[1] = h2;
}

uint32_t MurmurHash3_32(const char* key, size_t len) {
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

#define HASH_SEED 0x518c927498537b3dULL

size_t CaseInsensitiveHash::operator()(const char* key) const
{
    uint64 hash;
    string s = key;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    MurmurHash3_x64_128(s.c_str(), s.size(), HASH_SEED, &hash);
    return hash;
}

size_t CaseInsensitiveHash::operator()(const std::string& key) const
{
    uint64 hash;
    string s = key;
    std::transform(s.begin(), s.end(), s.begin(), ::tolower);
    MurmurHash3_x64_128(s.c_str(), s.size(), HASH_SEED, &hash);
    return hash;
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
    hash  = CaseInsensitiveHash()(str);
    pStr = NameStringCache::GetCachedString(str);
}

Name::Name(const std::string& str)
{
    hash  = CaseInsensitiveHash()(str);
    pStr = NameStringCache::GetCachedString(str);
}

Name::Name(std::string&& str)
{
    hash  = CaseInsensitiveHash()(str);
    pStr = NameStringCache::GetCachedString(str);
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