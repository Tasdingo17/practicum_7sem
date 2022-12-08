#ifndef __Mutation__
#define __Mutation__

#include <random>
#include <memory>
#include <bitset>

template<size_t N>
class Mutation {
public:
    virtual std::bitset<N> mutate(const std::bitset<N>& individ) = 0;
    virtual std::unique_ptr<Mutation<N>> clone() const = 0;
    virtual ~Mutation() {};
};

#include <iostream>
template<size_t N>
class Mutator: public Mutation<N>{
public:
    Mutator(double _prob): prob(_prob){};

    std::unique_ptr<Mutation<N>> clone() const override{
        return std::make_unique<Mutator>(*this);
    }

    std::bitset<N> mutate(const std::bitset<N>& individ) override{
        std::bitset<N> res(individ);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> mutation_prob(0, 1);

        for (unsigned i = 0; i <= res.size(); i++){
            if (mutation_prob(gen) < prob){
                res[i] = ~res[i]; 
            }
        }
        return res;
    }
private:
    double prob;    // mutation probability
};

#endif // __Mutation__