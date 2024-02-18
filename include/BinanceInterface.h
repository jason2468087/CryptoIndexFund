#include "Utility.h"

class BinanceHTTP
{
    std::string baseURL{"https://api.binance.com"};
    std::string apiKey{"Zu2hjriAqRcwJd5HYMmJA6BekvyaKJXZkP6Rh0NBn9zokYsdzpqzHd6ytTD6Bnbp"};
    std::string apiSecret{"RWHwHU8nW5Mvy1ChSrSrpwBGeeRNjFFEV8fBlfRrKXLc6i4EyS3SJPDsUsDd6DMW"};

public:
    std::string createSignature(const std::initializer_list<cpr::Parameter>&);
    std::string createTimeStamp();
    cpr::Response get(const std::initializer_list<cpr::Parameter>&, std::string);
    cpr::Response post(const std::initializer_list<cpr::Parameter>&, std::string);
};