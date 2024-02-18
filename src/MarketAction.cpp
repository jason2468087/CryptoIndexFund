#include "MarketAction.h"
#include <iostream>
#include <algorithm>
#include <string>
#include <vector>

BinanceHTTP binanceHTTP{};
CoinMarketCapHTTP coinMarketCapHTTP{};


cpr::Response MarketAction::checkAccount()
{
    cpr::Parameter timestamp{"timestamp", binanceHTTP.createTimeStamp()};
    cpr::Parameter signature{"signature",binanceHTTP.createSignature({timestamp})};

    return binanceHTTP.get({timestamp,signature}, "/api/v3/account");
}

double MarketAction::getUserAsset(std::string a)
{
    cpr::Parameter asset{"asset", a};
    cpr::Parameter timestamp{"timestamp", binanceHTTP.createTimeStamp()};
    cpr::Parameter signature{"signature",binanceHTTP.createSignature({asset,timestamp})};

    cpr::Response response{binanceHTTP.post({asset,timestamp,signature}, "/sapi/v3/asset/getUserAsset")};
    Json::Value json {Utility::parseJson(response.text)};

    return stod(json[0]["free"].asString());
}

std::vector<Coin> MarketAction::getTopCoinList(int size)
{
    cpr::Parameter limit{"limit", std::to_string(size)};

    cpr::Response response{coinMarketCapHTTP.get({limit}, "/v1/cryptocurrency/listings/latest")};
    Json::Value json {Utility::parseJson(response.text)};

    std::vector<Coin> coinList{};
    for (int i = 0 ; i < size ; i++)
    {
        coinList.push_back(Coin{
            json["data"][i]["symbol"].asString(),
            json["data"][i]["quote"]["USD"]["price"].asDouble(),
            json["data"][i]["quote"]["USD"]["market_cap_dominance"].asDouble()
        });
    }

    return coinList;
}

std::vector<Holding> MarketAction::getUserHolding()
{
    cpr::Parameter timestamp{"timestamp", binanceHTTP.createTimeStamp()};
    cpr::Parameter signature{"signature",binanceHTTP.createSignature({timestamp})};

    cpr::Response response{binanceHTTP.get({timestamp,signature}, "/api/v3/account")};
    Json::Value json {Utility::parseJson(response.text)};

    std::vector<Holding> holdingList{};
    int i{};
    while(true)
    {
        std::string qntStr{json["balances"][i]["free"].asString()};
        if (qntStr == "")
        {
            break;
        }
        else if (std::stod(qntStr) > 0)
        {
            std::string sym{json["balances"][i]["asset"].asString()};
            double qnt{std::stod(json["balances"][i]["free"].asString())};
            double price{(sym=="USDT") ? 1:getPrice(sym+"USDT")};
            double worth{qnt*price};

            Holding holding{sym,price,qnt,worth};
            holdingList.push_back(holding);
        }
         
        i++;
    }

    return holdingList;
}

double MarketAction::getPrice(std::string sym)
{
    cpr::Parameter symbol{"symbol", sym};

    cpr::Response response{binanceHTTP.get({symbol}, "/api/v3/ticker/price")};
    Json::Value json {Utility::parseJson(response.text)};

    return stod(json["price"].asString());
}


double MarketAction::getStepSize(std::string sym)
{
    cpr::Parameter symbol{"symbol", sym};

    cpr::Response response{binanceHTTP.get({symbol}, "/api/v3/exchangeInfo")};
    Json::Value json {Utility::parseJson(response.text)};

    return stod(json["symbols"][0]["filters"][1]["stepSize"].asString());
}

void MarketAction::trade(std::string sym, std::string trade_side, double qnt)
{
    cpr::Parameter symbol{"symbol", sym};
    cpr::Parameter side{"side", trade_side};
    cpr::Parameter quantity{"quantity", std::to_string(qnt)};
    cpr::Parameter type{"type", "MARKET"};
    cpr::Parameter timestamp{"timestamp", binanceHTTP.createTimeStamp()};
    cpr::Parameter signature{"signature",binanceHTTP.createSignature({symbol,side,quantity,type,timestamp})};

    cpr::Response response{binanceHTTP.post({symbol,side,quantity,type,timestamp,signature}, "/api/v3/order")};
    Json::Value json {Utility::parseJson(response.text)};
}

void MarketAction::startPriceWSoc(std::vector<Holding>& holdingList, void (*evtFuncPtr)(std::string,double))
{
    std::string streamParamUrl{};
    for (Holding holding:holdingList)
    {
        if (holding.symbol == "USDT")
            continue;
        std::string sym{holding.symbol};
        std::transform(sym.begin(), sym.end(), sym.begin(),
            [](unsigned char c){ return std::tolower(c); });
        streamParamUrl += sym+"usdt@trade/";
    }
    streamParamUrl.pop_back();

    WebSocketSSL::startWS("wss://stream.binance.com:9443/stream?streams="+streamParamUrl, evtFuncPtr); 
}