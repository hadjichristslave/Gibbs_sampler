#ifndef Sampler_H
#define Sampler_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <boost/random/variate_generator.hpp>
#include <boost/random/gamma_distribution.hpp>
#include <boost/random/mersenne_twister.hpp>

using namespace std;
class Sampler
{
    public:
        void resampleIndixes(Templates<double> &weights, Templates<int> &clusters);
        int resample();
        double randomGamma(double alpha);
        std::vector<double> generatePMF(std::vector<double> alphas ,int precision);

    protected:
    private:
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


}

std::vector<double> Sampler::generatePMF(std::vector<double> alphas, int precision){
    std::vector<double> d;
    return d;
}

double Sampler::randomGamma(double alpha){
    boost::mt19937 rng(time(0));
    boost::gamma_distribution<> pdf(alpha);
    boost::variate_generator<boost::mt19937&, boost::gamma_distribution<> >
    generator(rng, pdf);
    return generator();
}

#endif // Sampler_H
