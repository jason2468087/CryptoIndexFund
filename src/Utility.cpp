#include "Utility.h"
#include <cmath>

std::string Utility::b2a_hex(const std::uint8_t* p, std::size_t n) {
    static const char hex[] = "0123456789abcdef";
    std::string res;
    res.reserve(n * 2);

    for (auto end = p + n; p != end; ++p) {
        const std::uint8_t v = (*p);
        res += hex[(v >> 4) & 0x0F];
        res += hex[v & 0x0F];
    }

    return res;
}

std::string Utility::hmac_sha256(const char* key, std::size_t klen, const char* data, std::size_t dlen) {
    std::uint8_t digest[EVP_MAX_MD_SIZE];
    std::uint32_t dilen{};

    auto p = ::HMAC(
        ::EVP_sha256()
        , key
        , klen
        , (std::uint8_t*)data
        , dlen
        , digest
        , &dilen
    );
    assert(p);

    return b2a_hex(digest, dilen);
}


Json::Value Utility::parseJson(std::string json)
{
    JSONCPP_STRING err;
    Json::Value root;
    const auto jsonLength = static_cast<int>(json.length());

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(json.c_str(), json.c_str() + jsonLength, &root, &err))
    {
        return "parse Json error\n";
    }

    return root;
}

double Utility::lotPrecisionFilter(double qnt,double step)
{
    auto const r{ std::fmod(qnt, step) };
    return qnt - std::copysign(r < 0 ? r + step : r, 1);
}