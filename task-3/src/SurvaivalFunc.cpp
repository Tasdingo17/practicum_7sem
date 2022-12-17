#ifndef __SurvFunc__
#define __SurvFunc__

#include <memory>
#include <bitset>
#include "GoLEngine.hpp"

#define iterN 100
#define PENALTY 2500

template <size_t N>
class SurvaivalFunc{
public:
    virtual ~SurvaivalFunc() {};
    virtual std::unique_ptr<SurvaivalFunc> clone() const = 0;
    virtual int get_criterion_val(const std::bitset<N>& individ) = 0;
};


template <size_t N>
class GoLSurvaivalFunc : public SurvaivalFunc<N>{
public:
    GoLSurvaivalFunc(): engine(){}

    virtual std::unique_ptr<SurvaivalFunc<N>> clone() const override{
        return std::make_unique<GoLSurvaivalFunc<N>>(*this);
    }

    virtual int get_criterion_val(const std::bitset<N>& individ) override{
        int res;
        auto target_desc = engine.get_great_descedant(individ, iterN);
        res = target_desc.count();
        if (target_desc == engine.get_descedant(target_desc)){
            res += PENALTY;
        }
        return res;
    }
private:
    GoLEngine engine;
};

#endif  // __SurvFunc__