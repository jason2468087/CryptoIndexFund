#include "BinanceInterface.h"
#include "CoinMarketCapInterface.h"
#include "WebSocketSSL.h"

struct Coin
{
    std::string symbol{};
    double price{};
    double weight{};
};

struct Holding
{
    std::string symbol{};
    double price{};
    double quantity{};
    double worth{};
};

namespace MarketAction
{
    cpr::Response checkAccount();
    double getUserAsset(std::string);
    std::vector<Holding> getUserHolding();
    double getPrice(std::string);
    void startPriceWSoc(std::vector<Holding>&, void (*evtFuncPtr)(std::string,double));
    std::vector<Coin> getTopCoinList(int);
    double getStepSize(std::string);
    void trade(std::string, std::string, double);
}