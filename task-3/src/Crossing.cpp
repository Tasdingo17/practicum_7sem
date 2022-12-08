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
    virtual pair_descendant<N> cross(const std::bitset<N>& lhs, const std::bitset<N>& rhs) = 0;
    virtual std::unique_ptr<Crossing<N>> clone() const = 0;
    virtual ~Crossing() {};
};

template<size_t N>
class Crosser : public Crossing<N>{
public:
    Crosser(): point1(N / 3), point2(N * 2 / 3){};
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
};


#endif // __Crossing__