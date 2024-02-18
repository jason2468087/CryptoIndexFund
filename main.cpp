#include <cpr/cpr.h>
#include <iostream>
#include "MarketAction.h"

using namespace std::chrono;


duration tradePeriod{30min};
int fundSize{4};
std::vector<Coin> topCoinList{};
std::vector<Holding> holdingList{};

void printIterTime()
{
    std::time_t t = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
    std::tm ltime;
    localtime_r(&t, &ltime);
    std::cout << "< Rebalance portfolio at: "<<std::put_time(&ltime, "%F %T"/*"%H:%M:%S"*/) << " >\n";
}

void wait()
{
    time_point lastItrTime {system_clock::now()};
    time_point now {lastItrTime};

    while((now - lastItrTime) < tradePeriod)
    {
        std::this_thread::sleep_for(1s);
        now = system_clock::now();

        std::cout<<"Next trade is coming in: "<< (tradePeriod - (now - lastItrTime))/1s<<"s\r"<<std::flush;
    }
}

void updatePrice_WSoc(std::string sym, double price)
{
    for(Holding holding:holdingList)
        if(holding.symbol == sym)
        {
            holding.price = price;
            holding.worth = holding.price * holding.quantity;
        }
}

void rebalance() 
{    
    printIterTime();

    topCoinList = MarketAction::getTopCoinList(fundSize);
    holdingList = MarketAction::getUserHolding();

    // Calculate sum of coin'weight and portfolio asset
    double totalWeight{};
    double totalAsset{};
    std::for_each(topCoinList.begin(), topCoinList.end(), [&totalWeight](Coin&c) mutable {totalWeight += c.weight;});
    std::for_each(holdingList.begin(), holdingList.end(), [&totalAsset](Holding&h) mutable {totalAsset += h.worth;});

    // Sell all excess coin
    for(Holding holding:holdingList)
    {
        if (holding.symbol == "USDT")
            continue;

        double holdingRatio{holding.worth/totalAsset};
        double targetWeight{};

        for (Coin coin:topCoinList)
            if (holding.symbol == coin.symbol)
                targetWeight = coin.weight/totalWeight;

        if (holdingRatio > targetWeight)
        {
            std::string tradeSym = holding.symbol+"USDT";
            double sellQnt {totalAsset * (holdingRatio - targetWeight) / holding.price};
            double step {MarketAction::getStepSize(tradeSym)};
            double filtered_qnt {Utility::lotPrecisionFilter(sellQnt,step)};
            //MarketAction::trade(tradeSym, "SELL", filtered_qnt);
            std::cout<<"SELL "<<filtered_qnt<<" "<<tradeSym<<"\n";
        }
    }

    // Buy all excess coin
    for(Coin coin:topCoinList)
    {
        if (coin.symbol == "USDT")
            continue;

        double holdingRatio{};
        double targetWeight{coin.weight/totalWeight};

        for(Holding holding:holdingList)
            if(coin.symbol == holding.symbol)
                holdingRatio = holding.worth/totalAsset;
        
        if (holdingRatio < targetWeight)
        {
            std::string tradeSym = coin.symbol+"USDT";
            double buyQnt {totalAsset * (targetWeight - holdingRatio) / coin.price};
            double step {MarketAction::getStepSize(tradeSym)};
            double filtered_qnt {Utility::lotPrecisionFilter(buyQnt,step)};
            //MarketAction::trade(tradeSym, "BUY", filtered_qnt);
            std::cout<<"BUY "<<filtered_qnt<<" "<<tradeSym<<"\n";
        }
    }
}

int main()
{
    std::cout<<"CryptoIndexFund 17:\n";

    while(true)
    {
        rebalance();
        wait();
    }
    //MarketAction::startPriceWSoc(holdingList,updatePrice_WSoc);
    return 0;
}