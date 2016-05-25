
//  (C) Copyright Dave Abrahams, Steve Cleary, Beman Dawes, Howard
//  Hinnant & John Maddock 2000.  
//  Use, modification and distribution are subject to the Boost Software License,
//  Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt).
//
//  See http://www.boost.org/libs/type_traits for most recent version including documentation.


#ifndef NDNBOOST_TT_IS_ENUM_HPP_INCLUDED
#define NDNBOOST_TT_IS_ENUM_HPP_INCLUDED

#include <ndnboost/type_traits/intrinsics.hpp>
#ifndef NDNBOOST_IS_ENUM
#include <ndnboost/type_traits/add_reference.hpp>
#include <ndnboost/type_traits/is_arithmetic.hpp>
#include <ndnboost/type_traits/is_reference.hpp>
#include <ndnboost/type_traits/is_convertible.hpp>
#include <ndnboost/type_traits/is_array.hpp>
#ifdef __GNUC__
#include <ndnboost/type_traits/is_function.hpp>
#endif
#include <ndnboost/type_traits/config.hpp>
#if defined(NDNBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION) 
#  include <ndnboost/type_traits/is_class.hpp>
#  include <ndnboost/type_traits/is_union.hpp>
#endif
#endif

// should be the last #include
#include <ndnboost/type_traits/detail/bool_trait_def.hpp>

namespace ndnboost {

#ifndef NDNBOOST_IS_ENUM
#if !(defined(__BORLANDC__) && (__BORLANDC__ <= 0x551))

namespace detail {

#if defined(NDNBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION) 

template <typename T>
struct is_class_or_union
{
   NDNBOOST_STATIC_CONSTANT(bool, value =
      (::ndnboost::type_traits::ice_or<
           ::ndnboost::is_class<T>::value
         , ::ndnboost::is_union<T>::value
      >::value));
};

#else

template <typename T>
struct is_class_or_union
{
# if NDNBOOST_WORKAROUND(__BORLANDC__, NDNBOOST_TESTED_AT(0x581))// we simply can't detect it this way.
    NDNBOOST_STATIC_CONSTANT(bool, value = false);
# else
    template <class U> static ::ndnboost::type_traits::yes_type is_class_or_union_tester(void(U::*)(void));

#  if NDNBOOST_WORKAROUND(__MWERKS__, <= 0x3000) // no SFINAE
    static ::ndnboost::type_traits::no_type is_class_or_union_tester(...);
    NDNBOOST_STATIC_CONSTANT(
        bool, value = sizeof(is_class_or_union_tester(0)) == sizeof(::ndnboost::type_traits::yes_type));
#  else
    template <class U>
    static ::ndnboost::type_traits::no_type is_class_or_union_tester(...);
    NDNBOOST_STATIC_CONSTANT(
        bool, value = sizeof(is_class_or_union_tester<T>(0)) == sizeof(::ndnboost::type_traits::yes_type));
#  endif
# endif
};
#endif

struct int_convertible
{
    int_convertible(int);
};

// Don't evaluate convertibility to int_convertible unless the type
// is non-arithmetic. This suppresses warnings with GCC.
template <bool is_typename_arithmetic_or_reference = true>
struct is_enum_helper
{
    template <typename T> struct type
    {
        NDNBOOST_STATIC_CONSTANT(bool, value = false);
    };
};

template <>
struct is_enum_helper<false>
{
    template <typename T> struct type
       : public ::ndnboost::is_convertible<typename ndnboost::add_reference<T>::type,::ndnboost::detail::int_convertible>
    {
    };
};

template <typename T> struct is_enum_impl
{
   //typedef ::ndnboost::add_reference<T> ar_t;
   //typedef typename ar_t::type r_type;

#if defined(__GNUC__)

#ifdef NDNBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION
    
   // We MUST check for is_class_or_union on conforming compilers in
   // order to correctly deduce that noncopyable types are not enums
   // (dwa 2002/04/15)...
   NDNBOOST_STATIC_CONSTANT(bool, selector =
      (::ndnboost::type_traits::ice_or<
           ::ndnboost::is_arithmetic<T>::value
         , ::ndnboost::is_reference<T>::value
         , ::ndnboost::is_function<T>::value
         , is_class_or_union<T>::value
         , is_array<T>::value
      >::value));
#else
   // ...however, not checking is_class_or_union on non-conforming
   // compilers prevents a dependency recursion.
   NDNBOOST_STATIC_CONSTANT(bool, selector =
      (::ndnboost::type_traits::ice_or<
           ::ndnboost::is_arithmetic<T>::value
         , ::ndnboost::is_reference<T>::value
         , ::ndnboost::is_function<T>::value
         , is_array<T>::value
      >::value));
#endif // NDNBOOST_TT_HAS_CONFORMING_IS_CLASS_IMPLEMENTATION

#else // !defined(__GNUC__):
    
   NDNBOOST_STATIC_CONSTANT(bool, selector =
      (::ndnboost::type_traits::ice_or<
           ::ndnboost::is_arithmetic<T>::value
         , ::ndnboost::is_reference<T>::value
         , is_class_or_union<T>::value
         , is_array<T>::value
      >::value));
    
#endif

#if NDNBOOST_WORKAROUND(__BORLANDC__, < 0x600)
    typedef ::ndnboost::detail::is_enum_helper<
          ::ndnboost::detail::is_enum_impl<T>::selector
        > se_t;
#else
    typedef ::ndnboost::detail::is_enum_helper<selector> se_t;
#endif

    typedef typename se_t::template type<T> helper;
    NDNBOOST_STATIC_CONSTANT(bool, value = helper::value);
};

// these help on compilers with no partial specialization support:
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void,false)
#ifndef NDNBOOST_NO_CV_VOID_SPECIALIZATIONS
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void const,false)
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void volatile,false)
NDNBOOST_TT_AUX_BOOL_TRAIT_IMPL_SPEC1(is_enum,void const volatile,false)
#endif

} // namespace detail

NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_enum,T,::ndnboost::detail::is_enum_impl<T>::value)

#else // __BORLANDC__
//
// buggy is_convertible prevents working
// implementation of is_enum:
NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_enum,T,false)

#endif

#else // NDNBOOST_IS_ENUM

NDNBOOST_TT_AUX_BOOL_TRAIT_DEF1(is_enum,T,NDNBOOST_IS_ENUM(T))

#endif

} // namespace ndnboost

#include <ndnboost/type_traits/detail/bool_trait_undef.hpp>

#endif // NDNBOOST_TT_IS_ENUM_HPP_INCLUDED
