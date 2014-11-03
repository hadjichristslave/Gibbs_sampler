#ifndef SUFFICIENTSTATISTICS_H
#define SUFFICIENTSTATISTICS_H
#include "Auxiliary.h" // auxiliary functions like matrix multiplications go here
/*
* Structure that defines the sufficient statistics of a normal inverse wishart distribution
* Our case is about 2D data, so mu is a 1,2 vector and
* Lambda that defines the covariance is a 2,2 covariance vector.
*/
struct sufficientStatistics{
    /*
        Basic variable definition
    */
    public:
        Templates<int> kappa;
        Templates<int> nu;
        std::vector< std::vector<double> > mu;
        std::vector< std::vector< std::vector<double> > > lambda;
        static const int baseKappa =1;
        static const int baseNu =4;

        /*
            Helper variables
        */
            Auxiliary<double> aux;
        /*
            Function definitions
        */
        // strConstr
        sufficientStatistics(int,int);
        sufficientStatistics();
        void set(sufficientStatistics &k , int index);
        //Populate complex types
        void populateMu(std::vector< std::vector<double> > &lambda, int dim , int dataSize);
        void populateCovariance(std::vector< std::vector< std::vector<double> > > &lambda, int dim, int dataSize);
        void updateSufficientStatistics(Part &p, sufficientStatistics &k , int statisticsIndex , bool elementAdded);
        void print(sufficientStatistics &k,  int ind);
};

sufficientStatistics::sufficientStatistics(){
        int baseKappa  = 1;
        int baseNu     = 4;
}
sufficientStatistics::sufficientStatistics(int dataSize, int dataDim){
    int baseKappa  = 1;
    int baseNu     = 4;
    //Update single valued statistics
    kappa.populate(baseKappa, dataSize);
    nu.populate(baseNu, dataSize);

    // Update multidimensional statistics
    populateMu(mu,dataDim ,dataSize);
    populateCovariance(lambda,dataDim, dataSize);
}

void sufficientStatistics::populateMu(std::vector< std::vector<double> > &lambda, int dim , int dataSize){
    for(int j=0;j<dataSize;j++){
        std::vector<double> tempMu;
        tempMu.assign(dim,0.0);
        mu.push_back(tempMu);
    }
}

void sufficientStatistics::populateCovariance(std::vector< std::vector< std::vector<double> > > &lambda, int dim , int dataSize){
    for(int j=0;j<dataSize;j++){
        std::vector< std::vector<double> > currCov;
        for(int i=0;i<dim;i++){
            std::vector<double> temp;
            temp.assign(dim,0);
            temp[i]=1.0;
            currCov.push_back(temp);
        }
        lambda.push_back(currCov);
    }
}
void sufficientStatistics::print(sufficientStatistics &k,  int ind){
    std::cout << "ss's are" <<std::endl;
    std::cout << "kappa: " << k.kappa.get(ind) <<std::endl;
    std::cout << "nu : "<< k.nu.get(ind) <<std::endl;
    std::cout << "mu : ";
    for(int i=0;i<k.mu[ind].size();i++)
        std::cout << k.mu[ind][i] << ",";
    std::cout<< std::endl <<  "lambda: ";
    std::vector<std::vector<double> > lambda = k.lambda[ind];
    for(int i=0;i<lambda.size();i++){
        for(int j=0;j<lambda[i].size();j++)
            std::cout << lambda[i][j] << ",";
        std::cout<< std::endl << "        ";
    }
    std::cout << "---------------" << std::endl;
}

void sufficientStatistics::updateSufficientStatistics(Part &p, sufficientStatistics &k , int statisticsIndex , bool elementAdded){
    int kappa                                = k.kappa.get(statisticsIndex);
    int nu                                   = k.nu.get(statisticsIndex);
    std::vector<double> mu                   = k.mu.at(statisticsIndex);
    std::vector< std::vector<double> >lambda = k.lambda.at(statisticsIndex);

    //std::cout << std::endl <<"---initial statistics---" <<std::endl;
    //k.print(k, statisticsIndex);

    //Update kappa's and nu's to their new values
    double kappa1, nu1;
    /*
        TODO updating values one by one. This is bad practice should modify this to kappa0+numOfElementsWithinCluster
    */
    if(elementAdded){
        kappa1 = kappa + 1;
        nu1 = nu+1;
    }else{
        kappa1 = kappa-1;
        nu1    = nu-1;
        if(kappa1<baseKappa)
            kappa1 = baseKappa ;
        if(nu1<baseNu)
               nu1 = baseNu ;
    }

    // Some matrix operations needed hence the use of the Auxiliary class
    // Formulas of the updated values can be found on Herman Kampers paper
    // "Gibbs Sampling for fitting finite and infinite Gaussian mixture models" at page 4
    double kappa0, tempFrac;

    if(elementAdded){
        kappa0   = (double)kappa/((double)kappa+1);
        tempFrac = (double)1.0/((double)kappa+1);
    }else{
        kappa0   = (double)kappa/((double)kappa1);
        tempFrac = (double)1.0/((double)kappa1);
    }
    std::vector<double> sub = p.p.toVector();

    //calculate new mu
    std::vector<double> p1 = aux.productScalar(mu, kappa0);
    std::vector<double> p2 = aux.productScalar(sub, tempFrac);

    std::vector<double> mu1;
    if(elementAdded)
        mu1 = aux.matrixAddition(p1,p2);
    else
        mu1 = aux.matrixSubtraction(p1,p2);
    //calculate new lambda
    std::vector< std::vector<double> >lambda1;

    sub = aux.matrixSubtraction(mu, sub);
    std::vector< std::vector<double> > subTranspose =  aux.transpose(sub);
    std::vector< std::vector<double> > tempVec = aux.product(sub, subTranspose);

    tempVec = aux.productScalar(tempVec ,kappa0);

    if(elementAdded)
        lambda1 = aux.matrixAddition(lambda, tempVec);
    else
        lambda1 = aux.matrixSubtraction(lambda, tempVec);

    //Update the cluster hyperparameters
    k.kappa.set(kappa1,statisticsIndex);
    k.nu.set(nu1,statisticsIndex);
    k.mu.at(statisticsIndex) = mu1;
    k.lambda.at(statisticsIndex) = lambda1;

    //std::cout << std::endl <<"---updated statistics---" <<std::endl;
    //k.print(k, statisticsIndex);
}
void sufficientStatistics::set(sufficientStatistics &k, int index){
    kappa.set(k.kappa.get(0),index);
    nu.set(k.nu.get(0),index);
    mu[index]       = k.mu[0];
    lambda[index]   = k.lambda[0];
}
#endif // SUFFICIENTSTATISTICS_H
