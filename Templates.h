#ifndef ABSTRACTS_H
#define ABSTRACTS_H
#include <vector>
#include <algorithm>
#include <stdlib.h>
using namespace std;


// A template for all the custom data structures
// Every get-set-delete type operation is handled from this template.
// A vector element is a private member of this class that can handle all types of different data structures
template<class T>
class Templates
{
    private:
        vector<T> elems;
        vector<T> uniques;
    public:
        void populate(T const& elem, int numOfEntries);
        void populate(bool isRand, int numOfEntries, bool ram, bool rander);
        void normalize();
        void populate(T const& elem, int numOfEntries, int numOfDimensions);
        void set(T const& elem, int index);
        void set(std::vector<T> &elem);
        void set(std::vector<T> &elem , int index);\
        int uniqueSize();
        T get(int index);
        T sum();
        void remove(int index);
        void print(int index);
        void print();
        std::vector<int> getActiveClusters();
        int size();
        int findFirst(T const& elem , int operation);
        void divide(T const& elem);
        std::vector<T> cumsum();
        typename std::vector<T>::iterator unique(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last);
        void reassignIndexes();
        typename std::vector<T>::iterator end();
        typename std::vector<T>::iterator begin();


    protected:
};

bool myfunction(int i , int j ){
    return (i==j);
}

template <class T>
void Templates<T>::populate(T const& elem, int numOfEntries){ // any time of vector object is allowed
    for(int i=0;i<numOfEntries;i++)
        elems.push_back(elem);
}

template <class T>
void Templates<T>::populate(bool isRand, int numOfEntries, bool ram, bool rander){ // any time of vector object is allowed
    for(int i=0;i<numOfEntries;i++){
        int random = rand();
        int variable = random%numOfEntries;
        elems.push_back(variable);
    }
}

template <class T>
void Templates<T>::normalize(){ // any time of vector object is allowed
    int size = elems.size();
    T sum =0;
    for(unsigned i=0;i<elems.size();i++)
        sum+=elems[i];
    for(unsigned int i=0;i<size;i++)
        elems[i] = elems[i]/sum;
}

template <class T>
void Templates<T>::set(T const& elem, int index){ // set the value of vector element
    elems.at(index) = elem;
}
template<class T>
void Templates<T>::set(std::vector<T> &elem){
    elems = elem;
}
template <class T>
T Templates<T>::get(int index){ // get the value of vector element
    return elems.at(index);
}

template <class T>
T Templates<T>::sum(){ // get the value of vector element
    T sum = 0;
    for(int i=0;i<elems.size();i++)
        sum+=elems[i];
    return sum;
}
//returns the index of the first element that is equal to elem -1 if no element is found
// operation indexes
// 1 search equal
// 2 search bigger
// 3 search smaller
template <class T>
int Templates<T>::findFirst(T const& elem, int operation){ // get the value of vector element
    switch(operation){
        case 1:
            for(int i=0;i<elems.size();i++)
                    if(elems[i]==elem) return i;
            break;
        case 2:
            for(int i=0;i<elems.size();i++)
                    if(elems[i]>=elem) return i;
            break;
        case 3:
            for(int i=0;i<elems.size();i++)
                    if(elems[i]<=elem) return i;
            break;
    }
    return -1;
}
//divide all elemets with value
template<class T>
void Templates<T>::divide(T const& elem){
    for(int i=0;i<elems.size();i++)
        elems[i] = elems[i]/elem;
}

template <class T>
void Templates<T>::remove(int index){ // delete element by index.
    return elems.erase(elems.begin()+index);
}
template <class T>
void Templates<T>::print(int index){ // print the indexed element
    std::cout << elems.at(index) << std::endl;
}
template <class T>
void Templates<T>::print(){ // print the indexed element
    for(int i=0;i<elems.size();i++)
        std::cout << elems.at(i) << "-";
    std::cout<< std::endl;
}
template <>
void Templates<Point>::print(int index){ // print the indexed element
    elems.at(index).print();
}
template <class T>
int Templates<T>::size(){ // self exp
    return elems.size();
}
template <class T>
std::vector<int> Templates<T>::getActiveClusters(){
    std::vector<int> emptyCells;
    for(int i=0;i<elems.size();i++)
        if(elems[i]>0)
            emptyCells.push_back(i);
    return emptyCells;

}
template <class T>
std::vector<T> Templates<T>::cumsum(){
    std::vector<T> cumSumVect;
    cumSumVect.push_back(elems[0]);
    if(elems.size()>1)
        for(int i=1;i<elems.size();i++){
            T sum = elems[i]+cumSumVect[i-1];
            cumSumVect.push_back(sum);
        }
    return cumSumVect;
}

template <class T>
int Templates<T>::uniqueSize(){
    std::vector<T> tempvec = elems; // make a copy of the vector
    typename vector<T>::iterator it; // uniqe unique unique repeat repeat repeat
    tempvec.erase( unique(tempvec.begin(), tempvec.end()) ,tempvec.end());
    for(int i=0;i<tempvec.size();i++)
        uniques.push_back(tempvec[i]);
    return tempvec.size();
}

// The awesome bublesort uniqe c++ function
template <class T>
typename std::vector<T>::iterator Templates<T>::unique(typename std::vector<T>::iterator first, typename std::vector<T>::iterator last){
    while(first!=last){
        typename std::vector<T>::iterator next(first);
        last  = std::remove(++next, last ,*first);
        first = next;
    }
    return last;
}

template <class T>
void Templates<T>::reassignIndexes(){
    typename std::vector<T>::iterator it;
    for(unsigned int i=0;i<elems.size();i++){
        it = find(uniques.begin(), uniques.end(), elems[i]);
        if( it!= uniques.end())
            elems[i] = it-uniques.begin();
    }
}
template <class T>
typename std::vector<T>::iterator Templates<T>::begin(){ // any time of vector object is allowed
    return elems.begin();
}
template <class T>
typename std::vector<T>::iterator Templates<T>::end(){ // any time of vector object is allowed
    return elems.end();
}



#endif // ABSTRACTS_H
