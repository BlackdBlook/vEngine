#pragma once
#include <unordered_map>

#include "Header.h"

namespace Container
{
    struct CaseInsensitiveHash {
        size_t operator()(const std::string& key) const;
        size_t operator()(const class Name& key) const;
    };

    struct CaseInsensitiveEqual {
        bool operator()(const Name& key1, const Name& key2) const;
        bool operator()(const Name& key1, const string& key2) const;
    };
    class Name
    {
        SPtr<const std::string> str;
        
        uint64 hash = INVALID_HASH;
    public:
        Name();
        
        Name(const std::string& str);
        
        Name(std::string&& str);

        Name(const Name& other);
        
        Name(Name&& other) noexcept;
        
        const std::string& ToString() const
        {
            return *str;
        }

        Name& operator=(const Name& other);
        
        Name& operator=(const string& other);
        
        bool operator==(const Name& other) const
        {
            assert(GetHash() != INVALID_HASH);
            const uint64 otherHash = CaseInsensitiveHash()(other);
            return hash == otherHash;
        }
        
        bool operator==(const string& other) const
        {
            assert(GetHash() != INVALID_HASH);
            const uint64 otherHash = CaseInsensitiveHash()(other);
            return hash == otherHash;
        }

        bool operator!=(const Name& other) const
        {
            return !(*this == other);
        }

        
        bool operator!=(const string& other) const
        {
            return !(*this == other);
        }


        inline uint64 GetHash() const
        {
            return hash;
        }

        inline uint64 GetSize() const
        {
            return str->size();
        }
        
        static constexpr uint64 INVALID_HASH = 0xffffffffffffffff;
    };


    
}

namespace std {
    template <>
    struct hash<Container::Name> {
        std::size_t operator()(const Container::Name& k) const noexcept
        {
            return k.GetHash(); 
        }
    };
}