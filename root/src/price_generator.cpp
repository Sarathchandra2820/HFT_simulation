#include <iostream>       // For input/output
#include <string>         // For string manipulation
#include <vector>         // For storing a list of orders
#include <random>
#include <cstdio> // For popen and pclose

class Generate_price {
    
}

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