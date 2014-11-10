#ifndef Sampler_H
#define Sampler_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <boost/random/variate_generator.hpp>
#include <boost/random/gamma_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <boost/nondet_random.hpp>
#include <random>

using namespace std;
class Sampler
{
    public:
        void resampleIndixes(Templates<double> &weights, Templates<int> &clusters);
        int resample();
        double randomGamma(double shape, double scale);
        std::vector<double> generatePMF(std::vector<double> &alphas ,int precision);

    protected:
    private:
        static const int precision = 5;
};

void Sampler::resampleIndixes(Templates<double> &weights, Templates<int> &clusters){
    int topSize = clusters.uniqueSize();
    std::vector<double> intraCl(topSize,0);
    std::vector<std::vector<double> > interCl(topSize);
    double gammaRandom(double alpha, double theta);

    unique(clusters.begin() ,clusters.end());
    clusters.reassignIndexes();



    for(int i=0;i<clusters.size();i++){
        intraCl[clusters.get(i)] = intraCl[clusters.get(i)] + weights.get(i);
        interCl[clusters.get(i)].push_back(weights.get(i));
    }
    std::vector<double> pmf = generatePMF(intraCl, precision);

}

std::vector<double> Sampler::generatePMF(std::vector<double> &alphas, int precision){
    std::vector<double> d(alphas.size(),1);
    for(unsigned int i=0;i<precision;i++)
        for(unsigned int j=0;j<d.size();j++)
            d[j] = d[j] + randomGamma(alphas[j] , 1.0)*(1/(double)precision);
    return d;
}

//gamma distribuded random variables with
//more info: http://en.wikipedia.org/wiki/Gamma_distribution
double Sampler::randomGamma(double shape, double scale){
    //random device for seed
    std::random_device dev;
    //random number generator w/ seed
    boost::mt19937 rng(dev());
    //gamma dist which is needed
    boost::gamma_distribution<> pdf(shape);
    //merge distribution with random number generator
    boost::variate_generator<boost::mt19937&, boost::gamma_distribution<> >
    generator(rng, pdf);
    return scale*generator();
}

#endif // Sampler_H
