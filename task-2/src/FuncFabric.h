#ifndef __FuncFabr_h__
#define __FuncFabr_h__

#include "Functions.h"
#include <memory>
#include <vector>

class TFactory {
private:
    class TImpl;
    std::shared_ptr<const TImpl> Impl;
public:
    TFactory();
    ~TFactory();
    std::unique_ptr<TFunction> CreateObject( const std::string& name, 
                                             const std::initializer_list<double>& lst ) const;
    std::vector<std::string> GetAvailableObjects() const;
};


#endif