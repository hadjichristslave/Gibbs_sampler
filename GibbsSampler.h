#ifndef GibbsSampler_H
#define GibbsSampler_H
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <stdlib.h>
#include <iterator>
#include <cmath>
#include <fstream>
#include <string>
#include <boost/algorithm/string.hpp>

//custom classes/structs/templates go after the generic includes
#include "structs.h"  //generic class definitions
#include "Templates.h" //depends to structs.h
#include "sufficientStatistics.h" // depends to structs.h, Templates.h






using namespace std;

class GibbsSampler
{
    public:
        GibbsSampler();
        virtual ~GibbsSampler();
        Part particle;
        vector<Part> particleVector;
        int collapsedCRP(vector<Part> &partVector);
        void initialize(vector<Part> &partVector, Templates<int> &mu , Templates<int> &c, sufficientStatistics &k, int dataDim);
        //void updateSufficientStatistics(Part &p, sufficientStatistics &k , int statisticsIndex);
        int sampleNewCluster(Templates<int> &mu , int alpha,Part &datapoint,  int partindex, sufficientStatistics &accumulatedStats , Templates<int> &c);
        double predictMarginal(Part &part, sufficientStatistics &Stats, int pointIndex );
        sufficientStatistics G0;

        static const bool debuger = true;

        /*
        *  Gibbs sampler specific functions
        *  Every hard coded variable that is needed in order to define the model is given bellow
        */
        static const int dataDim  = 2;
        static const double alpha = 5;
        static const int nIter    = 20;

        //Initialize the parameters of the base distributions


    protected:
    private:

};

#endif // GibbsSampler_H
