// Copyright (C) 2005, 2006 EPITA Research and Development Laboratory
//
// This file is part of the Olena Library.  This library is free
// software; you can redistribute it and/or modify it under the terms
// of the GNU General Public License version 2 as published by the
// Free Software Foundation.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this library; see the file COPYING.  If not, write to
// the Free Software Foundation, 59 Temple Place - Suite 330, Boston,
// MA 02111-1307, USA.
//
// As a special exception, you may use this file as part of a free
// software library without restriction.  Specifically, if other files
// instantiate templates or use macros or inline functions from this
// file, or you compile this file and link it with other files to
// produce an executable, this file does not by itself cause the
// resulting executable to be covered by the GNU General Public
// License.  This exception does not however invalidate any other
// reasons why the executable file might be covered by the GNU General
// Public License.

#ifndef METALIC_CASE_HH
# define METALIC_CASE_HH

# include <mlc/bexpr.hh>
# include <mlc/assert.hh>
# include <mlc/abort.hh>
# include <mlc/ret.hh>
# include <mlc/is_a.hh>
# include <mlc/implies.hh>
# include <mlc/comma.hh>
# include <mlc/cmp.hh>
# include <mlc/if.hh>


/// Macros mlc_case and mlc_case_.
/// \{

# define mlc_case(Namespace, Context, Data) \
  typename Namespace::case_<Context, Data>::ret

# define mlc_case_(Namespace, Context, Data) \
  Namespace::case_<Context, Data>::ret

/// \}



// FIXME: doc this file!


namespace mlc
{

  struct case_selected  {};
  struct case_not_selected {};


  namespace internal
  {
    struct a_get_case;
    struct a_simple_case;
    struct a_switch_case;

    const unsigned unknown_case_id = 10000U;
  }


  namespace ERROR
  {
    struct PARAMETER_OF_mlc_where_SHOULD_BE_A_BEXPR;

    struct A_case_STATEMENT_SHOULD_NOT_START_AT_INDEX_0_BUT_1;

    struct A_case_STATEMENT_SHOULD_DERIVE_FROM_mlc_where_;
    struct A_default_case_STATEMENT_SHOULD_NOT_DERIVE_FROM_mlc_where_;

    struct A_default_case_STATEMENT_IN_A_switch_SHOULD_HAVE_EITHER_A_ret_OR_A_protected;
    struct A_default_case_STATEMENT_IN_A_switch_SHOULD_NOT_HAVE_BOTH_A_ret_AND_A_protected;
    struct THE_protected_PART_OF_A_default_case_STATEMENT_IN_A_switch_SHOULD_HAVE_A_ret;

    struct A_case_STATEMENT_IN_A_switch_SHOULD_HAVE_EITHER_A_ret_OR_A_protected;
    struct A_case_STATEMENT_IN_A_switch_SHOULD_NOT_HAVE_BOTH_A_ret_AND_A_protected;
    struct THE_protected_PART_OF_A_case_STATEMENT_IN_A_switch_SHOULD_HAVE_A_ret;

    struct NO_case_STATEMENT_CAN_BE_SELECTED;

  } // end of namespace mlc::ERROR


  namespace WARNING
  {
    struct NO_default_case_STATEMENT_HAS_BEEN_DEFINED;

  } // end of namespace mlc::WARNING



  namespace internal
  {

    template <typename T, typename T_is_bexpr>
    struct where_;

    template <typename bexpr>
    struct where_ < bexpr, mlc::true_ >
      : public mlc_if_(bexpr, case_selected, case_not_selected)
    {
    };

    template <typename T>
    struct where_ < T, mlc::false_ >
    {
    };

  } // end of namespace mlc::internal


  template <typename bexpr>
  struct where_ :

    private assert_< mlc_is_a(bexpr, mlc::abstract::bexpr),
		     mlc::ERROR::PARAMETER_OF_mlc_where_SHOULD_BE_A_BEXPR >,

    // hack so that the error above cannot propagate to the mlc_if_
    public internal::where_< bexpr,
			     typename mlc_is_a(bexpr, mlc::abstract::bexpr)::eval >
  {
  };

  

  mlc_decl_typedef(protected_);


} // end of namespace mlc




/// \def mlc_case_equipment_for_namespace(NAMESPACE)

# define mlc_case_equipment_for_namespace(NAMESPACE)													\
																			\
namespace NAMESPACE																	\
{																			\
																			\
  template <typename context,																\
	    typename data,																\
	    unsigned i = mlc::internal::unknown_case_id>												\
  struct case_ : public mlc::undefined															\
  {																			\
  };																			\
																			\
																			\
  template <typename context, typename data>														\
  struct case_ <context, data, 0> : public mlc::undefined												\
  {																			\
    typedef mlc::locked ret;																\
  };																			\
																			\
																			\
  template <typename context,																\
	    typename data>																\
  struct default_case_ : public mlc::undefined														\
  {																			\
  };																			\
																			\
																			\
  template <typename a_case>																\
  struct protected_in_																	\
  {																			\
    typedef typename mlc::typedef_::protected_::from_<a_case>::ret the;											\
    typedef typename mlc::is_found_<the>::bexpr is_found;												\
    typedef typename mlc::ret_found_in_<the>::bexpr is_found_with_ret;											\
    typedef mlc_ret(the) ret;																\
  };																			\
																			\
																			\
																			\
  namespace internal																	\
  {																			\
																			\
    template <typename use,																\
	      typename context, typename data, unsigned i,												\
	      bool the_ith_case_derives_from_true,													\
	      bool the_ith_case_derives_from_false>													\
    struct handle_case_;																\
																			\
																			\
    template <typename use,																\
	      typename context, typename data, unsigned i>												\
    struct handle_case_ <use, context, data, i, 1, 1>;													\
																			\
																			\
    template <typename use,																\
	      typename context, typename data>														\
    struct handle_default_case_;															\
																			\
																			\
    template <typename context, typename data>														\
    struct handle_default_case_ < mlc::internal::a_get_case, context, data >										\
																			\
      : private mlc::assert_< mlc::implies_< mlc::is_defined_< NAMESPACE::default_case_<context, data> >,						\
					     mlc_is_not_a(mlc_comma_1(NAMESPACE::default_case_<context, data>),						\
							  mlc::where_) >,										\
			      mlc::ERROR::A_default_case_STATEMENT_SHOULD_NOT_DERIVE_FROM_mlc_where_ >							\
    {																			\
      typedef NAMESPACE::default_case_<context, data> case_t;												\
      typedef typename protected_in_<case_t>::the protected_case_t;											\
																			\
      typedef typename mlc::if_< mlc::is_found_<protected_case_t>,											\
	                         protected_case_t,													\
	                         case_t >::ret ret;													\
    };																			\
																			\
																			\
    template <typename context, typename data>														\
    struct handle_default_case_ < mlc::internal::a_simple_case, context, data >										\
																			\
      : private mlc::assert_< mlc::implies_< mlc::is_defined_< NAMESPACE::default_case_<context, data> >,						\
					     mlc_is_not_a(mlc_comma_1(NAMESPACE::default_case_<context, data>),						\
							  mlc::where_) >,										\
			      mlc::ERROR::A_default_case_STATEMENT_SHOULD_NOT_DERIVE_FROM_mlc_where_ >							\
    {																			\
      typedef NAMESPACE::default_case_<context, data> case_t;												\
      typedef typename protected_in_<case_t>::the protected_case_t;											\
																			\
      typedef typename mlc::if_< mlc::is_found_<protected_case_t>,											\
	                         protected_case_t,													\
	                         case_t >::ret ret;													\
    };																			\
																			\
																			\
    template <typename context, typename data>														\
    struct handle_default_case_ < mlc::internal::a_switch_case, context, data >										\
																			\
      : private mlc::assert_< mlc::implies_< mlc::is_defined_< NAMESPACE::default_case_<context, data> >,						\
					     mlc_is_not_a(mlc_comma_1(NAMESPACE::default_case_<context, data>),						\
							  mlc::where_) >,										\
			      mlc::ERROR::A_default_case_STATEMENT_SHOULD_NOT_DERIVE_FROM_mlc_where_ >,							\
																			\
	private mlc::assert_< mlc::implies_< mlc::is_defined_< NAMESPACE::default_case_<context, data> >,						\
					     mlc::or_< mlc::ret_found_in_< NAMESPACE::default_case_<context, data> >,					\
						       typename protected_in_< NAMESPACE::default_case_<context, data> >::is_found > >,			\
			      mlc::ERROR::A_default_case_STATEMENT_IN_A_switch_SHOULD_HAVE_EITHER_A_ret_OR_A_protected >,				\
																			\
	private mlc::assert_< mlc::implies_< mlc::and_< mlc::is_defined_< NAMESPACE::default_case_<context, data> >,					\
							mlc::or_< mlc::ret_found_in_< NAMESPACE::default_case_<context, data> >,			\
								  typename protected_in_< NAMESPACE::default_case_<context, data> >::is_found > >,	\
					     mlc::xor_< mlc::ret_found_in_< NAMESPACE::default_case_<context, data> >,					\
							typename protected_in_< NAMESPACE::default_case_<context, data> >::is_found > >,		\
			      mlc::ERROR::A_default_case_STATEMENT_IN_A_switch_SHOULD_NOT_HAVE_BOTH_A_ret_AND_A_protected >,				\
																			\
        private mlc::assert_< mlc::implies_< typename protected_in_< NAMESPACE::default_case_<context, data> >::is_found,				\
					     typename protected_in_< NAMESPACE::default_case_<context, data> >::is_found_with_ret >,			\
			      mlc::ERROR::THE_protected_PART_OF_A_default_case_STATEMENT_IN_A_switch_SHOULD_HAVE_A_ret >				\
																			\
    {																			\
      typedef NAMESPACE::default_case_<context, data> case_t;												\
      typedef typename mlc::if_<mlc_is_a(case_t, mlc::undefined),											\
	                        mlc::none,														\
	                        case_t>::ret ret;													\
    };																			\
																			\
																			\
    template <typename use, typename context, typename data, unsigned i>										\
    struct handle_case_ <use, context, data, i, 0, 0>													\
																			\
      : private mlc::assert_< mlc::implies_< mlc::is_defined_< NAMESPACE::case_<context, data, i> >,							\
					     mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, i>),							\
						      mlc::where_) >,											\
			      mlc::ERROR::A_case_STATEMENT_SHOULD_DERIVE_FROM_mlc_where_ >,								\
																			\
	private mlc::assert_< mlc::implies_< mlc::neq_< use, mlc::internal::a_get_case >,								\
					     mlc::is_defined_< NAMESPACE::default_case_<context, data> > >,						\
			      mlc::WARNING::NO_default_case_STATEMENT_HAS_BEEN_DEFINED >								\
																			\
    {																			\
      typedef handle_default_case_<use, context, data> handle_t;											\
      typedef typename mlc::if_< mlc::is_defined_< NAMESPACE::default_case_<context, data> >,								\
	                         typename handle_t::ret,												\
	                         mlc::none >::ret ret;													\
    };																			\
																			\
																			\
																			\
    template <typename use,																\
	      typename context, typename data, unsigned i>												\
    struct handle_case_ <use, context, data, i, 1, 0>													\
																			\
      : private mlc::assert_< mlc::or_< mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, i>),							\
						 mlc::where_),												\
					mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, i>),							\
						 mlc::undefined) >,											\
			      mlc::ERROR::A_case_STATEMENT_SHOULD_DERIVE_FROM_mlc_where_ >,								\
																			\
	private mlc::assert_< mlc::implies_< mlc::and_< mlc::eq_<use, mlc::internal::a_switch_case>,							\
							mlc::is_defined_< NAMESPACE::case_<context, data, i> > >,					\
					     mlc::or_< mlc::ret_found_in_< NAMESPACE::case_<context, data, i> >,					\
						       typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found > >,			\
			      mlc::ERROR::A_case_STATEMENT_IN_A_switch_SHOULD_HAVE_EITHER_A_ret_OR_A_protected >,					\
																			\
	private mlc::assert_< mlc::implies_< mlc::and_list_< mlc::eq_<use, mlc::internal::a_switch_case>,						\
							     mlc::is_defined_< NAMESPACE::case_<context, data, i> >,					\
							     mlc::or_< mlc::ret_found_in_< NAMESPACE::case_<context, data, i> >,			\
								       typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found > >,	\
					     mlc::xor_< mlc::ret_found_in_< NAMESPACE::case_<context, data, i> >,					\
							typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found > >,			\
			      mlc::ERROR::A_case_STATEMENT_IN_A_switch_SHOULD_NOT_HAVE_BOTH_A_ret_AND_A_protected >,					\
																			\
	private mlc::assert_< mlc::implies_< mlc::and_list_< mlc::eq_<use, mlc::internal::a_switch_case>,						\
							     mlc::is_defined_< NAMESPACE::case_<context, data, i> >,					\
							     typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found >,			\
      					     typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found_with_ret >,				\
			      mlc::ERROR::THE_protected_PART_OF_A_case_STATEMENT_IN_A_switch_SHOULD_HAVE_A_ret >					\
    {																			\
      typedef NAMESPACE::case_<context, data, i> case_t;												\
      typedef typename protected_in_<case_t>::the protected_case_t;											\
																			\
      typedef typename mlc::if_< mlc::is_found_<protected_case_t>,											\
	                         protected_case_t,													\
	                         case_t >::ret ret;													\
    };																			\
																			\
																			\
																			\
    template <typename use,																\
	      typename context, typename data, unsigned i>												\
    struct handle_case_ <use, context, data, i, 0, 1>													\
																			\
      : private mlc::assert_< mlc::or_< mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, i>),							\
						 mlc::where_),												\
					mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, i>),							\
						 mlc::undefined) >,											\
			      mlc::ERROR::A_case_STATEMENT_SHOULD_DERIVE_FROM_mlc_where_ >,								\
																			\
	private mlc::assert_< mlc::implies_< mlc::and_< mlc::eq_<use, mlc::internal::a_switch_case>,							\
							mlc::is_defined_< NAMESPACE::case_<context, data, i> > >,					\
					     mlc::or_< mlc::ret_found_in_< NAMESPACE::case_<context, data, i> >,					\
						       typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found > >,			\
			      mlc::ERROR::A_case_STATEMENT_IN_A_switch_SHOULD_HAVE_EITHER_A_ret_OR_A_protected >,					\
																			\
	private mlc::assert_< mlc::implies_< mlc::and_list_< mlc::eq_<use, mlc::internal::a_switch_case>,						\
							     mlc::is_defined_< NAMESPACE::case_<context, data, i> >,					\
							     mlc::or_< mlc::ret_found_in_< NAMESPACE::case_<context, data, i> >,			\
								       typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found > >,	\
					     mlc::xor_< mlc::ret_found_in_< NAMESPACE::case_<context, data, i> >,					\
							typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found > >,			\
			      mlc::ERROR::A_case_STATEMENT_IN_A_switch_SHOULD_NOT_HAVE_BOTH_A_ret_AND_A_protected >,					\
																			\
	private mlc::assert_< mlc::implies_< mlc::and_list_< mlc::eq_<use, mlc::internal::a_switch_case>,						\
							     mlc::is_defined_< NAMESPACE::case_<context, data, i> >,					\
							     typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found >,			\
      					     typename protected_in_< NAMESPACE::case_<context, data, i> >::is_found_with_ret >,				\
			      mlc::ERROR::THE_protected_PART_OF_A_case_STATEMENT_IN_A_switch_SHOULD_HAVE_A_ret >					\
    {																			\
      typedef mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, i+1>),										\
		       mlc::case_selected) next_case_is_selected;											\
      typedef mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, i+1>),										\
		       mlc::case_not_selected) next_case_is_not_selected;										\
																			\
      typedef handle_case_ < use,															\
	                     context, data, i+1,													\
	                     mlc_bool(next_case_is_selected), mlc_bool(next_case_is_not_selected)							\
                            > handle_next_case_t;													\
      typedef typename handle_next_case_t::ret ret;													\
    };																			\
																			\
																			\
																			\
    template <typename use, typename context, typename data>												\
    struct select_case_																	\
																			\
      : private mlc::assert_< mlc::eq_< mlc_ret(mlc_comma_2(NAMESPACE::case_<context, data, 0>)),							\
					mlc::locked >,													\
			      mlc::ERROR::A_case_STATEMENT_SHOULD_NOT_START_AT_INDEX_0_BUT_1 >								\
    {																			\
      typedef mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, 1>),											\
		       mlc::case_selected) first_case_is_selected;											\
      typedef mlc_is_a(mlc_comma_2(NAMESPACE::case_<context, data, 1>),											\
		       mlc::case_not_selected) first_case_is_not_selected;										\
      typedef handle_case_ < use,															\
	                     context, data, 1,														\
	                     mlc_bool(first_case_is_selected), mlc_bool(first_case_is_not_selected)							\
	                   > handle_first_case_t;													\
      typedef typename handle_first_case_t::ret ret;													\
    };																			\
																			\
																			\
  }																			\
																			\
																			\
  template <typename context, typename data>														\
  struct get_case_																	\
  {																			\
    typedef typename NAMESPACE::internal::select_case_<mlc::internal::a_get_case, context, data>::ret ret;						\
  };																			\
																			\
																			\
  template <typename context, typename data>														\
  struct case_ <context, data, mlc::internal::unknown_case_id>												\
																			\
    : private mlc::assert_< mlc::neq_< mlc_comma_2(typename NAMESPACE::internal::select_case_<mlc::internal::a_simple_case,				\
						                                              context, data>::ret),					\
				       mlc::none >,													\
			    mlc::ERROR::NO_case_STATEMENT_CAN_BE_SELECTED >										\
																			\
  {																			\
    typedef typename NAMESPACE::internal::select_case_<mlc::internal::a_simple_case, context, data>::ret ret;						\
  };																			\
																			\
																			\
  template <typename context, typename data>														\
  struct switch_																	\
																			\
    : private mlc::assert_< mlc::neq_< mlc_comma_2(typename NAMESPACE::internal::select_case_<mlc::internal::a_switch_case,				\
						                                              context, data>::ret),					\
				       mlc::none >,													\
			    mlc::ERROR::NO_case_STATEMENT_CAN_BE_SELECTED >										\
																			\
  {																			\
    typedef typename NAMESPACE::internal::select_case_<mlc::internal::a_switch_case, context, data>::ret case_t;					\
    typedef typename case_t::ret ret;															\
  };																			\
																			\
																			\
}																			\
																			\
struct e_n_d__w_i_t_h__s_e_m_i_c_o_l_o_n



#endif // ! METALIC_CASE_HH
