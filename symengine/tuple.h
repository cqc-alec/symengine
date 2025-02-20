#ifndef SYMENGINE_TUPLE_H
#define SYMENGINE_TUPLE_H

#include <symengine/basic.h>

namespace SymEngine
{

class Tuple : public Basic
{
private:
    vec_basic container_;

public:
    IMPLEMENT_TYPEID(SYMENGINE_TUPLE)
    Tuple(const vec_basic &container);

    hash_t __hash__() const;
    bool __eq__(const Basic &o) const;
    int compare(const Basic &o) const;
    static bool is_canonical(const vec_basic &container);

    virtual vec_basic get_args() const
    {
        return vec_basic(container_.begin(), container_.end());
    }
};

RCP<const Basic> tuple(const vec_basic &arg);

} // namespace SymEngine
#endif
