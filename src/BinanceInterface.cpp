#include "BinanceInterface.h"
#include <iostream>

#include "json/json.h"
#include <memory>

std::string BinanceHTTP::createSignature(const std::initializer_list<cpr::Parameter>& parameters)
{
    std::string paramStr{};
    for (cpr::Parameter param:parameters)
    {
        paramStr += (param.key+"="+param.value)+"&";
    }
    paramStr.pop_back(); //remove last &

    std::string key{apiSecret};
    
    return Utility::hmac_sha256(key.c_str(),key.length(),paramStr.c_str(),paramStr.length());
}


std::string BinanceHTTP::createTimeStamp()
{
    cpr::Response r{get({},"/api/v3/time")};
    
    Json::Value json {Utility::parseJson(r.text)};
    
    return json["serverTime"].asString();
}

cpr::Response BinanceHTTP::get(const std::initializer_list<cpr::Parameter>& parameters, std::string endPoint)
{
    cpr::Response r = cpr::Get(cpr::Url{baseURL+endPoint},
                      cpr::Header{{"X-MBX-APIKEY", apiKey}},
                      cpr::Parameters{parameters}
                      );

    //std::cout<<"Response Status:"<<r.status_code<<"\n"; 
    //std::cout<<"Response Content:"<<r.text<<"\n"; 

    return r;
}

cpr::Response BinanceHTTP::post(const std::initializer_list<cpr::Parameter>& parameters, std::string endPoint)
{
    cpr::Response r = cpr::Post(cpr::Url{baseURL+endPoint},
                      cpr::Header{{"X-MBX-APIKEY", apiKey}},
                      cpr::Parameters{parameters}
                      );

    //std::cout<<"Response Status:"<<r.status_code<<"\n"; 
    //std::cout<<"Response Content:"<<r.text<<"\n"; 
    
    return r;
}