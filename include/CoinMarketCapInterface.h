#include "Utility.h"

class CoinMarketCapHTTP
{
    std::string baseURL{"https://pro-api.coinmarketcap.com"};
    std::string apiKey{"135240a5-ba1c-4bb4-888e-8ff37d4a9877"};

public:
    cpr::Response get(const std::initializer_list<cpr::Parameter>&, std::string);
};