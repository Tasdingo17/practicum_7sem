#ifndef __SurvFunc__
#define __SurvFunc__

#include <memory>
#include <bitset>

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
    virtual std::unique_ptr<SurvaivalFunc<N>> clone() const override{
        return std::make_unique<GoLSurvaivalFunc<N>>(*this);
    }

    virtual int get_criterion_val(const std::bitset<N>& individ) override{
        return individ.count();
    }
};

#endif  // __SurvFunc__