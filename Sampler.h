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
        Templates<int> sampleClusters(Templates<double> &pmf, int numberOfSamples);

    protected:
    private:
        static const int precision = 10;
};

void Sampler::resampleIndixes(Templates<double> &weights, Templates<int> &clusters){
    //vector of the particles to keep
    std::vector<int> keep;
    //template of the top PMF. Template used due to its massive functionality compared to a simple std::vector struct
    Templates<double> PMF;
    //get the unique number of clusters. To be used for the first hierarchy
    int topSize = clusters.uniqueSize();

    std::vector<double> intraCl(topSize,0);
    // set the inter number of clusters for the second hierarchy
    std::vector<std::vector<double> > interCl(topSize);

    double gammaRandom(double alpha, double theta);

    //find unique clusters, elements are modified internaly
    unique(clusters.begin() ,clusters.end());
    //fix the indexes to link to the super clusters.
    clusters.reassignIndexes();

    for(unsigned int i=0;i<clusters.size();i++){
        intraCl[clusters.get(i)] = intraCl[clusters.get(i)] + weights.get(i);
        // add the weight of the particle of the sub-cluster
        interCl[clusters.get(i)].push_back(weights.get(i));
        // add the index of the weight so that reverse tracability is possible.
        interCl[clusters.get(i)].push_back((double)i);
    }

    std::vector<double> pmf = generatePMF(intraCl, precision);
    PMF.set(pmf);
    PMF.normalize();


    Templates<int> topClusters = sampleClusters(PMF , clusters.size());

    for(unsigned int i=0;i<clusters.size();i++){
        int currInd = topClusters.get(i);
        Templates<double> weights;
        weights.set(interCl[currInd]);
        weights = weights.returnSeries(0,2);
        weights.normalize();
        Templates<double> indexes;
        indexes.set(interCl[currInd]);
        indexes = indexes.returnSeries(1,2);

        Templates<int> inter = sampleClusters(weights , 1);
        keep.push_back(inter.get(0));
    }
    for(unsigned int i=0;i<keep.size();i++){
        std::cout << keep[i] << "-";
    }
    std::cout << "\n";
}

Templates<int> Sampler::sampleClusters(Templates<double> &pmf , int numberOfSamples){
    Templates<int> clusterAssignments;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0,1);

    std::vector<double> cumulativeSum = pmf.cumsum();
    pmf.set(cumulativeSum);

    for(unsigned int i=0;i<numberOfSamples;i++){
        double randomNum = dis(gen);
        int assignment  = pmf.findFirst(randomNum, 2);
        clusterAssignments.add(assignment);
    }
    return clusterAssignments;
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
    //create gamma distribuded random var
    generator(rng, pdf);
    //multiply by scale to fix the one parameter gamma rnd provided by boost
    return scale*generator();
}

#endif // Sampler_H
