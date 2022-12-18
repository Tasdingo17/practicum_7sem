#ifndef __Crossing__
#define __Crossing__

#include <bitset>
#include <random>
#include <utility>
#include <memory>

template<size_t N>
using pair_descendant = std::pair<std::bitset<N>, std::bitset<N>>;

template<size_t N>
class Crossing{
public:
    Crossing(double _cross_prob): cross_prob(_cross_prob) {};
    virtual pair_descendant<N> cross(const std::bitset<N>& lhs, const std::bitset<N>& rhs) = 0;
    virtual std::unique_ptr<Crossing<N>> clone() const = 0;
    double get_crossing_prob(){
        return cross_prob;
    }
    virtual ~Crossing() {};
private:
    double cross_prob;
};

template<size_t N>
class Crosser : public Crossing<N>{
public:
    Crosser(double _cross_prob = 0.8): point1(N / 3), point2(N * 2 / 3), cross_prob(_cross_prob){};
    Crosser(const Crosser& other) = default;
    ~Crosser() = default;

    std::unique_ptr<Crossing<N>> clone() const override{
        return std::make_unique<Crosser<N>>(*this);
    }

    // 2-point crossover
    pair_descendant<N> cross(const std::bitset<N>& lhs, const std::bitset<N>& rhs) override{
        std::bitset<N> descendant1 = lhs;
        std::bitset<N> descendant2 = rhs;
        for (unsigned i = point2; i >= point1; i--){
            descendant1[i] = rhs[i];
            descendant2[i] = lhs[i];
        }
        return {descendant1, descendant2};
    }
private:
    unsigned int point1;
    unsigned int point2;
    double cross_prob;
};


#endif // __Crossing__