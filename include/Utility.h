#include <cpr/cpr.h>
#include <openssl/sha.h>
#include <openssl/hmac.h>
#include <string>
#include <string_view>
#include <array>
#include <ctime>
#include "json/json.h"

namespace Utility
{
    std::string b2a_hex(const std::uint8_t* p, std::size_t n);
    std::string hmac_sha256(const char* key, std::size_t klen, const char* data, std::size_t dlen);
    Json::Value parseJson(std::string json);
    double lotPrecisionFilter(double,double);
}