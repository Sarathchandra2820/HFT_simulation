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

class GBM : public PricingModel {
private:
    double initialprice;
    double timestep;
    double mean;
    double volatility; 

    //define the random number generation engine
    std::random_device rd;
    std::mt19937 generator;
    std::normal_distribution<double> normal_dist;

public:
    double newprice;
    //define the GBM constructor
    GBM(double initialprice, double timestep, double mean, double volatility) : initialprice(initialprice), timestep(timestep), mean(mean), volatility(volatility), normal_dist(0.0, 1.0) {}

    double generatePrice() override {

        double Z = normal_dist(generator);

        newprice = initialprice + mean*initialprice*timestep + volatility*initialprice*sqrt(timestep)*Z;
        // Use the parameters to simulate the price

        initialprice = newprice;
        return newprice; // Simulate a random price
    }
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
    double mean;         // Mean
    double volatility;     // volatility
    double timestep;     // time step
    double jumpIntensity; // Jump intensity (lambda)
    double jumpMean;      // Mean jump size
    double jumpStdDev;    // Standard deviation of jump size

    double l_b = 0;
    double u_b = 1;

    //double normal_mu = 0;
    //double normal_sigma = 1; 

    //random number from uniform dist
    std::random_device rd;
    std::mt19937 generator;
    // Uniform distribution for floating-point numbers
    std::uniform_real_distribution<double> uniform_dist;
    //random number from uniform dist 0,1 
    std::normal_distribution<double> normal_dist;
    //random number from uniform dist jumpmean,jumpstddev 
    std::normal_distribution<double> jump_dist;

public:
    double new_price;
    // Constructor to initialize parameters
    JumpDiffusionModel(double initialPrice, double mean, double volatility, double timestep, double jumpIntensity, double jumpMean, double jumpStdDev)
        : initialPrice(initialPrice), mean(mean), volatility(volatility), 
        timestep(timestep), jumpIntensity(jumpIntensity),
         jumpMean(jumpMean), jumpStdDev(jumpStdDev), generator(rd()),                                 // Seed the engine
          uniform_dist(l_b, u_b),                          // Initialize uniform distribution with bounds
          jump_dist(jumpMean, jumpStdDev), normal_dist(0.0,1.0) {}

    double generatePrice() override {
        double U = uniform_dist(generator);
        double Z1 = normal_dist(generator);
        double j_p;
        if (U<jumpIntensity*timestep) {
            double Y = jump_dist(generator);
            double J = exp(Y);
            double j_p = (J-1)*initialPrice;
        }
        else {
            double j_p = 0;
        }
        new_price = initialPrice + mean*initialPrice*timestep + volatility*initialPrice*sqrt(timestep)*Z1 + j_p;
        initialPrice = new_price;
        // Placeholder for Jump Diffusion logic
        // Use the parameters to simulate the price
        return new_price; // Simulate a random price
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

    // Create an instance of JumpDiffusionModel with example parameters
    double initialprice = 100.0;
    int n_steps = 1000;
    JumpDiffusionModel model(initialprice, 0.05, 0.01, 0.001, 0.1, 0.0, 1.0);

    // Generate and print a simulated price
    for (int i = 0; i<n_steps; i++) {
        double price = model.generatePrice();
        std::cout << "Time step " << (i + 1) << ": Price = " << price << std::endl;
    }
    
    return 0;



}