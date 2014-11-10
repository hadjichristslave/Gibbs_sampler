#ifndef Sampler_H
#define Sampler_H
#include <stdio.h>
#include <stdlib.h>

using namespace std;
class Sampler
{
    public:
        std::vector<int> resampleIndixes(Templates<double> &weights, Templates<int> &clusters);
        int resample();
    protected:
    private:
};

std::vector<int> Sampler::resampleIndixes(Templates<double> &weights, Templates<int> &clusters){
    std::vector<int> assigned;
    return assigned;

}


#endif // Sampler_H
