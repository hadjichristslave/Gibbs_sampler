#include "GibbsSampler.h"

GibbsSampler::GibbsSampler(){
    //ctor
    // Set the seed for the random number generator. What better place than the constructor, right?
    srand(time(NULL));
    G0  = sufficientStatistics(1,dataDim);
}

GibbsSampler::~GibbsSampler(){
    //dtor
}
/* resampleParts takes as input a vector of Part and outputs a vector of indexes that are sampled from the
*  data set after being clustered as a mixture of Gaussians using a Gibbs sampler.
*  This function is an enhancement to the random roullete wheel procedure of the FastSLAM algorithm
*/
int GibbsSampler::collapsedCRP(vector<Part> &partVector){


    //Get the dataset size. Will be needed to initialize the sufficient statistics struct.
    int dataSize = partVector.size();

    /* Inverse normal wishart distribution has is a 4 paarameter distribution.
    *  Hence its sufficient statistics are κ,λ,μ,ν.
    *  Since we can maximaly have a number of gaussians equal to the number of particles, we create the maximum number of SS structs
    *  In very big data sets this can be bad practice, but in smaller ones it does not introduce a problem
    */
    sufficientStatistics k = sufficientStatistics(dataSize , dataDim);

    // m vector represents the number of data points that exist in cluster C
    // size will be 1xN where N is the number of datapoints
    Templates<int> m;
    m.populate(0.0,dataSize);

    //c represents the cluster every point is assigned to
    // size will be 1xN where N is the number of datapoints
    Templates<int> c;
    c.populate(0,dataSize);

    // The first iteration will randomly allocate particles to clusters. The sufficient statistics of every cluster will be updated normallys
    this->initialize(partVector, m, c,k , dataSize);
    if(debuger && !debuger ){
        m.print();
        c.print();
        k.print(k,0);
        k.print(k,1);
        k.print(k,2);
        G0.print(G0,0);
    }
    //start gibbs sampler iterations
    //As the number of iterations --> infinity, the sampler will mathematically converge to the target distrubiotion
    //Increased number of iterations == slower algo. you win some lose some.
    for(int i=0;i<nIter;i++){
    //for(int i=0;i<1;i++){
        //For all iterations for all particles
        for(int j=0;j<dataSize; j++){
            //Remove data item from cluster

            m.set(m.get(c.get(j))-1, c.get(j) );
            //Update the sufficient statistics
            G0.updateSufficientStatistics(partVector[j], k, c.get(j), false);
            int cluster = this->sampleNewCluster(m , alpha, partVector[j] , k);
            c.set(cluster, j);
            m.set(m.get(c.get(j))+1, c.get(j));
            if(m.get(c.get(j))>1)
                G0.updateSufficientStatistics(partVector[j], k, cluster, true);
            else{
                //G0.updateSufficientStatistics(partVector[j], G0, 0, true);
                k.set(G0,cluster);
                G0.updateSufficientStatistics(partVector[j], k, cluster, true);
            }
        }
        //std::vector<int> act = m.getActiveClusters();
        //std::cout << act.size()<< "--" << std::endl;
        //m.print();

        if(debuger){
            std::cout << "--------" <<std::endl;
            for ( int ij=0;ij<c.size();ij++){
                Point p = (Point)partVector[ij];
                std::cout << p.getX() << "," << p.getY() << "," << c.get(ij)<< std::endl;;
            }


            std::cout << "++++++++" << std::endl;
            std::vector<int> er = m.getActiveClusters();
            for(int ik=0;ik<er.size();ik++){
                if(k.kappa.get(er[ik])>1 &&k.nu.get(er[ik])>4)
                    k.printMu(k, er[ik] );
            }
        }
    }
    //c.print();
}
void GibbsSampler::initialize(vector<Part> &partVector, Templates<int> &mu , Templates<int> &c, sufficientStatistics &k , int dataSize){
    //Some variable declerations
    int counter = 0;
    int randChoice;
    //iterate through all the elemets and assign them to a random cluster
    for(std::vector<Part>::iterator it=partVector.begin(); it!=partVector.end(); it++ , ++counter){
        //dereference the iterator. Easier to function with dereferenced structures
        Part &d = *it;

        //In the first step of the algorihtm, randomly allocate the particles to clusters
        //randChoice = rand() % dataSize;
        randChoice = counter%10;
        // Set the data to the random cluster
        c.set(randChoice, counter);
        mu.set(mu.get(c.get(randChoice))+1, randChoice);
        //If a new cluster is introduced, use the base distribution's hyperparameters, else use the
        if(mu.get(c.get(randChoice)) >1)
            G0.updateSufficientStatistics(d, k, randChoice, true);
        else{
            k.set(G0, randChoice);
            G0.updateSufficientStatistics(d, k, randChoice, true);
        }
    }
}

//sample a new cluster given the alphas the sufficient statistics and the value
int GibbsSampler::sampleNewCluster(Templates<int> &mu , double alpha, Part &datapoint,
                                sufficientStatistics &accumulatedStats){
    int cluster;
    // Get all clusters that are active
    std::vector<int> activeClusters = mu.getActiveClusters();

    // Get the number of active clusters
    double totalSize = mu.sum();

    // Cluster that will store the probability of a Part being in this cluster
    // Given all the other Parts in steps t-1,t
    Templates<double> clustProbability;
    clustProbability.populate(0,activeClusters.size());

    //Calculate the marginal of every element given all other elements within the cluster
    for(int i=0;i<activeClusters.size();i++){
        int clustInd = activeClusters[i];
        // changed c.get(clustind) to clustInd;
        double pred = this->predictMarginal(datapoint, accumulatedStats, clustInd )*mu.get(clustInd );
        clustProbability.set(pred, i);
    }

    // calculate the marginal of a partcle being assigned in a new cluster
    double nu0 =  this->predictMarginal(datapoint, G0, 0);

    //normalizing constant
    double normalizer = (double)(clustProbability.sum())/(alpha+totalSize) + alpha/(alpha+totalSize)*nu0;

    // probability of going to a new cluster
    double newClusterProb = alpha/(alpha+(double)mu.sum())*nu0/normalizer;

    //random number time to sampler
    double randomNumber=  (rand()%100+1.0)/100.0;
    if(randomNumber<newClusterProb ){
        //find the fist available cluster and assign the index there
        cluster = mu.findFirst(0 , 1);
    }else{
        //find the first value in the cumulative sum bigger than the random number
        // standard cumsum operations. Return the index of the cluster
        double u1       = randomNumber-newClusterProb;

        //normalizer cumputations
        double tempNorm = normalizer*((double)alpha+(double)mu.sum());

        //divide all elements with normalizer, make the elements sum to one
        clustProbability.divide(tempNorm);

        std::vector<double> cumsum = clustProbability.cumsum();
        //set clust probability vector to the template so that we get the nice functions of the Template
        clustProbability.set(cumsum);
        //find the first index in the cumsum bigger than u1 and return it
        //This is a classical MCMC approach that is nicely presented in a video lecture by Jarad Niemi here:
        //https://www.youtube.com/watch?v=MKnjsqYVG4Y&list=UUvJW6o0x1dzKZJ5b5exYuxw
        int clusterIndex = clustProbability.findFirst(u1,2);
        cluster = activeClusters[clusterIndex];
    }
    return cluster;
}
double GibbsSampler::predictMarginal(Part &part, sufficientStatistics &Stats, int pointIndex){
    /* Predict marginal for the data.
    *  Data are assumed to be a mixture of Gaussians
    *  Prior distribution is a Normal inverse Wishart
    *  Proof of the out Formula as well as implementation specific information regarding the Gibbs sampler
    *  can be found here: http://non-parametric.blogspot.nl/2014/10/model-implementation-technical-details.html#more
    */
    //retrieve the sufficient statistics
    int kappa                                 = Stats.kappa.get(pointIndex);
    int nu                                    = Stats.nu.get(pointIndex);
    std::vector<double> mu                    = Stats.mu[pointIndex];
    std::vector< std::vector<double> > lambda = Stats.lambda[pointIndex];

    int nuTemp = nu-dataDim+1;
    //calculate covariance
    double denominator = ((double)kappa+1)/((double)kappa*(double)nuTemp);
    std::vector< std::vector<double> > covariance = Stats.aux.productScalar(lambda, denominator);

    //calculate the non-normalized marginal
    std::vector<double> observation = part.p.toVector();
    double firstPart = (Stats.aux.getFirstPart(mu, observation, covariance)/(double)nuTemp)+1.0;
    double exponent  = -((double)(nu+1)/2);

    // abs is not correct but math.pow gives unreasonable NAN errors when multiplying negative integer to non integer
    // Not 100% sure if this is correct.
    firstPart = pow(abs(firstPart),exponent);
    exponent = -exponent;
    //calculate the determinant
    std::vector< std::vector<double> > detVect = Stats.aux.productScalar(covariance, nuTemp*dataDim);
    double retVal = Stats.aux.determinant(detVect);
    retVal = 1/sqrt(abs(retVal));
    retVal = firstPart*(tgamma(exponent)/tgamma((double)nuTemp/2))*retVal;

    //Capture -nan and nan values
    if(retVal!=retVal){
            std::cout<< "error marginal " << retVal << std::endl;
    }
    return retVal;
}

// To use the sampler, set debugger to false and then call
// perl gibbsParser.pl to run the gibbs sampler and see the cluster classifications
// dependencies: mystats.m
// All files should be in the same folder

int main(){
    vector<Part> particleVector;
    GibbsSampler gs;

    //read a file with x and y positions in every line, add the data to the point array
    // This can be changed to include any type of positional info
    string line;
    ifstream myfile ("positionalInfo.txt");

    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            //split comma sepparated file
            std::string sample = line;
            std::vector<string> strs;
            boost::split(strs, sample, boost::is_any_of(","));
            //convert string to char for char to double conversion
            string tmp = strs[0];
            string tmp1 = strs[1];
            char tab2[1024];
            char tab1[1024];
            strncpy(tab2, tmp.c_str(), sizeof(tab2));
            tab2[sizeof(tab2) - 1] = 0;
            strncpy(tab1, tmp1.c_str(), sizeof(tab1));
            tab2[sizeof(tab1) - 1] = 0;
            char* pEnd;

            //char to double
            double x = strtod (tab2, &pEnd);
            double y = strtod (tab1, &pEnd);

            //finally add data to vector
            Part p = Part();
            Point poi(x, y);
            p.setPoint(poi);
            particleVector.push_back(p);
        }
        myfile.close();
    }

    try{
        gs.collapsedCRP(particleVector);

    }catch(exception &e){
        std::cout << " Exception occured " << e.what() << std::endl;
    }


 }
