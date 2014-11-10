#ifndef Sampler_H
#define Sampler_H
#include <stdio.h>
#include <stdlib.h>

using namespace std;
class Sampler
{
    public:
        void resampleIndixes(Templates<double> &weights, Templates<int> &clusters);
        int resample();
    protected:
    private:
};

void Sampler::resampleIndixes(Templates<double> &weights, Templates<int> &clusters){
    int topSize = clusters.uniqueSize();
    std::vector<double> interCl(topSize,1);
    std::vector<std::vector<double> > intraCl(topSize);

    for(unsigned int i=0;i<clusters.size();i++){
    }


}


#endif // Sampler_H
