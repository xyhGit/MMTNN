//-----------------------------------------------------------------------------
// boost aligned_storage.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2002-2003
// Eric Friedman, Itay Maman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef NDNBOOST_ALIGNED_STORAGE_HPP
#define NDNBOOST_ALIGNED_STORAGE_HPP

#include <cstddef> // for std::size_t

#include "ndnboost/config.hpp"
#include "ndnboost/detail/workaround.hpp"
#include "ndnboost/type_traits/alignment_of.hpp"
#include "ndnboost/type_traits/type_with_alignment.hpp"
#include "ndnboost/type_traits/is_pod.hpp"

#include "ndnboost/mpl/eval_if.hpp"
#include "ndnboost/mpl/identity.hpp"

#include "ndnboost/type_traits/detail/bool_trait_def.hpp"

namespace ndnboost {

namespace detail { namespace aligned_storage {

NDNBOOST_STATIC_CONSTANT(
      std::size_t
    , alignment_of_max_align = ::ndnboost::alignment_of<max_align>::value
    );

//
// To be TR1 conforming this must be a POD type:
//
template <
      std::size_t size_
    , std::size_t alignment_
>
struct aligned_storage_imp
{
    union data_t
    {
        char buf[size_];

        typename ::ndnboost::mpl::eval_if_c<
              alignment_ == std::size_t(-1)
            , ::ndnboost::mpl::identity< ::ndnboost::detail::max_align >
            , ::ndnboost::type_with_alignment<alignment_>
            >::type align_;
    } data_;
    void* address() const { return const_cast<aligned_storage_imp*>(this); }
};

template< std::size_t alignment_ >
struct aligned_storage_imp<0u,alignment_>
{
    /* intentionally empty */
    void* address() const { return 0; }
};

}} // namespace detail::aligned_storage

template <
      std::size_t size_
    , std::size_t alignment_ = std::size_t(-1)
>
class aligned_storage : 
#ifndef __BORLANDC__
   private 
#else
   public
#endif
   ::ndnboost::detail::aligned_storage::aligned_storage_imp<size_, alignment_> 
{
 
public: // constants

    typedef ::ndnboost::detail::aligned_storage::aligned_storage_imp<size_, alignment_> type;

    NDNBOOST_STATIC_CONSTANT(
          std::size_t
        , size = size_
        );
    NDNBOOST_STATIC_CONSTANT(
          std::size_t
        , alignment = (
              alignment_ == std::size_t(-1)
            ? ::ndnboost::detail::aligned_storage::alignment_of_max_align
            : alignment_
            )
        );

private: // noncopyable

    aligned_storage(const aligned_storage&);
    aligned_storage& operator=(const aligned_storage&);

public: // structors

    aligned_storage()
    {
    }

    ~aligned_storage()
    {
    }

public: // accessors

    void* address()
    {
        return static_cast<type*>(this)->address();
    }

    const void* address() const
    {
        return static_cast<const type*>(this)->address();
    }
};

//
// Make sure that is_pod recognises aligned_storage<>::type
// as a POD (Note that aligned_storage<> itself is not a POD):
//
template <std::size_t size_, std::size_t alignment_>
struct is_pod< ::ndnboost::detail::aligned_storage::aligned_storage_imp<size_,alignment_> >
   NDNBOOST_TT_AUX_BOOL_C_BASE(true)
{ 
    NDNBOOST_TT_AUX_BOOL_TRAIT_VALUE_DECL(true)
}; 


} // namespace ndnboost

#include "ndnboost/type_traits/detail/bool_trait_undef.hpp"

#endif // NDNBOOST_ALIGNED_STORAGE_HPP
