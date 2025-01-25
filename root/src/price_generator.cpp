#include <iostream>       // For input/output
#include <string>         // For string manipulation
#include <vector>         // For storing a list of orders
#include <random>
#include <cstdio> // For popen and pclose

class PricingModel {
public:
    virtual double generatePrice() = 0; // Pure virtual function
    virtual ~PricingModel() = default;  // Virtual destructor
};

class OUprocess : public PricingModel {
private:
    double initialPrice;      // Initial price
    double timestep;          // Time step  
    double mean; // Initial volatility
    double meanReversion;     // Mean reversion rate
    double longTermVol;       // Long-term volatility

    //For the random number generation
    double normal_mu = 0;
    double normal_sigma = 1; 

    //random number from uniform dist
    std::random_device rd;
    std::mt19937 generator;
    std::normal_distribution<double> normal_dist;

public:
    double new_price;


    // Constructor to initialize parameters
    OUprocess(double initialPrice, double timestep, double mean, double meanReversion, double longTermVol)
        : initialPrice(initialPrice), timestep(timestep), mean(mean), meanReversion(meanReversion),
          longTermVol(longTermVol) {}

    double generatePrice() override {

        double randomSample = normal_dist(generator);

        new_price = initialPrice + meanReversion*(mean-initialPrice)*timestep + longTermVol*(timestep)*randomSample;
        // Use the parameters to simulate the price

        initialPrice = new_price;
        return new_price; // Simulate a random price
    }
};

class JumpDiffusionModel : public PricingModel {
private:
    double initialPrice; // Initial price
    double jumpIntensity; // Jump intensity (lambda)
    double jumpMean;      // Mean jump size
    double jumpStdDev;    // Standard deviation of jump size

public:
    // Constructor to initialize parameters
    JumpDiffusionModel(double initialPrice, double jumpIntensity, double jumpMean, double jumpStdDev)
        : initialPrice(initialPrice), jumpIntensity(jumpIntensity), jumpMean(jumpMean), jumpStdDev(jumpStdDev) {}

    double generatePrice() override {
        // Placeholder for Jump Diffusion logic
        // Use the parameters to simulate the price
        return initialPrice + (rand() % 15); // Simulate a random price
    }
};

/*
class MarketDrivenModel : public PricingModel {
private:
    double initialPrice; // Initial price
    double bidAskSpread; // Bid-ask spread

public:
    // Constructor to initialize parameters
    MarketDrivenModel(double initialPrice, double bidAskSpread)
        : initialPrice(initialPrice), bidAskSpread(bidAskSpread) {}

    double generatePrice() override {
        // Placeholder for market-driven logic
        // Use the parameters to simulate the price
        return initialPrice + (rand() % 5); // Simulate a random price
    }
};
*/



int main () {
    double p0 = 100;
    double T = 1;
    double N = 1000;
    double mu = 0.5;
    double vol = 0.05; 

    double normal_mu = 0;
    double normal_sigma = 1; 

    std::vector<double> p_arr(N, 0);

    p_arr[0] = p0;
    double dt = T/N;

    //random number from uniform dist
    std::random_device rd;
    std::mt19937 generator(rd());

    std::normal_distribution<double> normal_dist(normal_mu, normal_sigma); 

    for (int i = 0; i < N; ++i) {

        double sample = normal_dist(generator);

        p_arr[i+1] = p_arr[i] + mu*p_arr[i]*dt + vol*p_arr[i]*sqrt(dt)*sample;
        std::cout << p_arr[i] << std::endl;
    }

    return 0;



}