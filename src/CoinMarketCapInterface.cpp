#include "CoinMarketCapInterface.h"
#include <iostream>

cpr::Response CoinMarketCapHTTP::get(const std::initializer_list<cpr::Parameter>& parameters, std::string endPoint)
{
    cpr::Response r = cpr::Get(cpr::Url{baseURL+endPoint},
                      cpr::Header{{"Accepts","application/json"},{"X-CMC_PRO_API_KEY", apiKey}},
                      cpr::Parameters{parameters}
                      );

    //std::cout<<"Response Status:"<<r.status_code<<"\n"; 
    //std::cout<<"Response Content:"<<r.text<<"\n"; 

    return r;
}