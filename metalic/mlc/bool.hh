// Copyright (C) 2001, 2002, 2003, 2004, 2005, 2006 EPITA Research and Development Laboratory
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

#ifndef METALIC_BOOL_HH
# define METALIC_BOOL_HH

# include <mlc/value.hh>
# include <mlc/flags.hh>
# include <mlc/wrap.hh>


/*! \def mlc_bool(BExpr)
**
** Macro that retrieves a Boolean value from a Boolean expression type.
** Its result is either true or false.
*/
# define mlc_bool(BExpr)  mlc::internal::get_bool<BExpr>::value



/*! \def mlc_iff(Type, BExpr)
**
** FIXME: doc
*/
# define mlc_iff(Type, BExpr)  typename mlc::iff_<Type, BExpr>::ret
# define mlc_iff_(Type, BExpr) mlc::iff_<Type, BExpr>::ret





namespace mlc
{

  namespace abstract {

    /*! \class mlc::abstract::boolean
    **
    ** Abstract base class for Booleans represented as types.  This
    ** class has two important sub-classes: mlc::true_ and
    ** mlc::false_.
    **
    ** Any mlc Boolean expression type derives from either mlc::true_
    ** or mlc::false_ so derives from mlc::abstract::boolean.
    */
    
    struct boolean : public value {};

  } // end of mlc::abstract


  // fwd decl
  template <bool> struct bool_;


  namespace internal
  {

    /*! \class mlc::get_bool<T>
    **
    ** Internal class so do not use it.  This class gives access to a
    ** Boolean value from a Boolean expression type; it is used in the
    ** mlc_bool(T) macro.
    **
    ** \see mlc_bool(T)
    */
    template <typename T>
    struct get_bool
    {
      static const bool value = T::bool_value_;
    };


    /*! \class mlc::value_<bool, b>
    **
    ** Specialization of value_<T, val> for the Boolean case.
    **
    ** Design notes: 1) This specialization is defined so that mlc
    ** Booleans derive from mlc::abstract::boolean.  2) This
    ** specialization conforms to the interface of the generic version
    ** of mlc::internal::value_.
    */

    template <bool b>
    struct value_ <bool, b> : public abstract::boolean
    {
      /*! \typedef type
      **
      ** Gives the regular type, that is bool, corresponding to this
      ** class.
      */
      typedef bool type;

      /*! \member value
      **
      ** Gives the regular value of the Boolean represented by this
      ** class.  The result is either true or false.
      **
      ** This member is provided for any mlc value class such as
      ** mlc::int_<51> and mlc::char_<'c'>.  However, to access the
      ** regular value of a type T that you know to be a mlc Boolean
      ** expression, you should prefer to use the macro mlc_bool(T).
      ** This macro also ensures that T really is an expression.
      ** 
      */
      static const bool value = b;

      /*! \typedef eval
      **
      ** Returns mlc::true_ or mlc::false_.
      **
      ** Since Boolean expression types derive from either mlc::true_
      ** or mlc::false_, these expression types are never exactly
      ** mlc::true_ nor mlc::false_.  This typedef thus allows for the
      ** expression evaluation.
      **
      ** Please note that, however, we usually do not need expression
      ** evaluation.  The most common use of a Boolean expression is
      ** to check that it is verified (true) and, for that, we provide
      ** "expr::ensure();" and "ensure_<expr..>".  For instance:
      **   or_<mlc_is_a(T, int), mlc_is_a(T, unsigned)>::ensure();
      ** ensures that T is int or unsigned without using ::eval.
      **
      ** \see mlc::true_, mlc::false_, mlc::ensure_<expr..>.
      */
      typedef bool_<b> eval;

    private:

      /*! \member bool_value_
      **
      ** This member is redundant with the member 'value'.  It is used
      ** by the mlc_bool(T) macro to ensure that T actually derives
      ** from mlc::true_ or mlc::false_.
      */
      static const bool bool_value_ = b;

      template <typename T> friend class get_bool;
    };


    /*! \class mlc::internal::ensure_item_<i, expr, check>
    **
    ** Internal so do not use it.  This class is for use in the
    ** definition of mlc::ensure_<..>.
    **
    ** Design note: this class does not derive from abstract::type
    ** because it is used in inheritance so a ctor should exist.
    **
    ** \see mlc::ensure_<..>
    */

    template <unsigned i, typename expr, typename check>
    struct ensure_item_
    {
    };


    /*! \class mlc::internal::retrieve_ensure_
    **
    ** Internal so do not use it.  This class is for use in the
    ** definition of mlc::ensure_<..>.
    **
    ** Design notes: 1) This class does not derive from abstract::type
    ** because it is used in inheritance so a ctor should exist. 2)
    ** This class provides internal_ensure_ so that it acts like the
    ** value 'true' in a sequence of 'and'; it thus has no effect when
    ** appearing in an ensure_item_.
    **
    ** \see mlc::ensure_<..>
    */

    template <typename bexpr>
    struct retrieve_ensure_
    {
      typedef typename bexpr::internal_ensure_ ret; // provided such as in classes inheriting from true_
    };

    template <>
    struct retrieve_ensure_ < mlc::none >
    {
      typedef mlc::dummy ret;
    };


  } // end of namespace mlc::internal



  /*! \class mlc::ensure_<expr>
  **
  ** This class is a replacement for the instruction "expr::ensure();"
  ** when there is no room for having instruction.  The typical use
  ** is to express a constraint (or several constraints) upon a
  ** parameter (or several parameters) of a templated class.
  **
  ** ensure_<expr> only accepts one parameter, which has to be a
  ** Boolean expression type.  An equivalent version for a variadic
  ** list of parameters is ensure_list_<expr1,..>
  **
  ** Sample use:
  ** 
  **   template <class T>
  **   struct foo : private virtual ensure_< neq_<T, int> >
  **   { ... 
  **   };
  ** means that T can be any type but int.
  **
  **
  ** Please avoid the following code:
  **   template <class T1, class T2>
  **   struct bar : private virtual ensure_< neq_<T1, int> >,
  **                private virtual ensure_< neq_<T2, int> >
  **   { ... 
  **   };
  ** a better replacement is:
  **   template <class T1, class T2>
  **   struct bar : private virtual ensure_list_< neq_<T1, int>,
  **                                              neq_<T2, int> >
  **   { ... 
  **   };
  ** see the design notes below for details.
  **
  ** Also prefer the use of ensure_list_<expr1, expr2> than the
  ** equivalent ensure_< and_<expr1, expr2> >.  Actually, the former
  ** provides better error messages since the compiler is able to
  ** say which expr is not verified, whereas the latter cannot.
  **
  **
  ** Design notes: 1) This class does not derive from abstract::type
  ** because it is used in inheritance so a ctor should exist.  2)
  ** This class relies on mlc::internal::ensure_item_ to check that
  ** the expression is true.  3) When several contrains such as
  ** "private ensure_<..>" appear through a hierarchy of classes or
  ** for a given class, the program may not compile because of
  ** multiple inheritance of the same base class; the solution is to
  ** systematically write "private virtual ensure_<..>".
  **
  ** \see ensure_list_<bexpr1,..>
  **
  */

  template <typename bexpr>
  struct ensure_ :
    private virtual internal::ensure_item_<0, bexpr,
					   typename internal::retrieve_ensure_<bexpr>::ret>
  {
  };


  /*! \class mlc::iff_<T, bexpr>
  **
  ** FIXME: doc
  ** T iff bexpr
  */
  template <typename T, typename bexpr>
  struct iff_ :
    private ensure_<bexpr>
  {
    typedef T ret;
  };



  /*! \class mlc::ensure_list_<expr1..>
  **
  ** This class is a replacement for a sequence of instructions:
  ** "expr1::ensure(); .." when there is no room for having
  ** instructions.  The typical use is to express a constraint (or
  ** several constraints) upon a parameter (or several parameters)
  ** of a templated class.
  **
  ** ensure_list_<..> has a variadic list of parameters.  It expects
  ** at least 2 parameters and handles up to 9 parameters.  Each
  ** parameter has to be a Boolean expression type.  To check only a
  ** single expression, the appropriate tool is ensure_<expr>.
  **
  **
  ** Sample use:
  ** 
  **   template <class T1, class T2>
  **   struct foo : private virtual ensure_list_< neq_<T1, int>,
  **                                              neq_<T2, int> >
  **   { ... 
  **   };
  **
  ** Design notes: 1) This class does not derive from abstract::type
  ** because it is used in inheritance so a ctor should exist.  2)
  ** This class relies on mlc::internal::ensure_item_ to check that
  ** each expression is true. 3) using "virtual" allow to encompass
  ** the multiple base class problem.
  **
  ** \see ensure_<expr>
  */

  template <typename bexpr1,
	    typename bexpr2,
	    typename bexpr3 = none, 
	    typename bexpr4 = none,
	    typename bexpr5 = none,
	    typename bexpr6 = none, 
	    typename bexpr7 = none,
	    typename bexpr8 = none,
	    typename bexpr9 = none>
  struct ensure_list_ :
    private virtual internal::ensure_item_<1, bexpr1,
					   typename internal::retrieve_ensure_<bexpr1>::ret>,
    private virtual internal::ensure_item_<2, bexpr2,
					   typename internal::retrieve_ensure_<bexpr2>::ret>,
    private virtual internal::ensure_item_<3, bexpr3,
					   typename internal::retrieve_ensure_<bexpr3>::ret>,
    private virtual internal::ensure_item_<4, bexpr4,
					   typename internal::retrieve_ensure_<bexpr4>::ret>,
    private virtual internal::ensure_item_<5, bexpr5,
					   typename internal::retrieve_ensure_<bexpr5>::ret>,
    private virtual internal::ensure_item_<6, bexpr6,
					   typename internal::retrieve_ensure_<bexpr6>::ret>,
    private virtual internal::ensure_item_<7, bexpr7,
					   typename internal::retrieve_ensure_<bexpr7>::ret>,
    private virtual internal::ensure_item_<8, bexpr8,
					   typename internal::retrieve_ensure_<bexpr8>::ret>,
    private virtual internal::ensure_item_<9, bexpr9,
					   typename internal::retrieve_ensure_<bexpr9>::ret>
  {
  };




  /*! \class mlc::bool_<true>
  **
  ** Specialization of mlc::bool_<b> for b set to true.  This type
  ** is also known as mlc::true_.  Every Boolean expression types
  ** derive either from this type or from mlc::false_.
  **
  ** Design notes: 1) This specialization is defined so that mlc
  ** Booleans derive from mlc::abstract::boolean.  2) This
  ** specialization conforms to the interface of the generic version
  ** of mlc::internal::value_.
  **
  ** \see mlc::bool_<false>
  */

  template <>
  struct bool_<true> : public internal::value_<bool, true>
  {
    /*! \member ensure()
    **
    ** This member is inherited in every Boolean expression types that
    ** derive from mlc::true_.  This member is not provided in
    ** mlc::false_.
    ** 
    ** A static check to ensure that a Boolean expression type, say
    ** Expr, is verified can thus be written as an instruction:
    ** "Expr::ensure();"
    **
    ** When there is no room in code for an instruction, use
    ** mlc::ensure_<expr1..> instead.
    **
    ** Design note:  This member is a no-op (it has no cost at
    ** run-time).
    **
    ** \see: mlc::ensure<expr1..>
    */
    static void ensure() {}

    /*! \typedef internal_ensure_
    **
    ** This is internal stuff so do not use it.
    **
    ** This typedef is inherited in every Boolean expression types
    ** that derive from mlc::true_.  This typedef is not provided in
    ** mlc::false_.  The type returned by this typedef has no meaning
    ** (and thus no significant value).  A static check via
    ** "mlc::ensure_<..>" uses this typedef.
    **
    ** \see mlc::internal::ensure_item_<i, expr>
    */
    typedef dummy internal_ensure_;

  };

  typedef bool_<true>  true_;


  /*! \class mlc::bool_<false>
  **
  ** Specialization of mlc::bool_<b> for b set to false.  This type
  ** is also known as mlc::false_.  Every Boolean expression types
  ** derive either from this type or from mlc::true_.
  **
  ** Conversely to mlc::true_, this class does not feature ensure()
  ** nor ensure_.  So, when a Boolean expression type, say Expr,
  ** is evaluated to false, the static checks "Expr::ensure();" and
  ** "Expr::ensure_" do not compile.
  **
  ** Design notes: 1) This specialization is defined so that mlc
  ** Booleans derive from mlc::abstract::boolean.  2) This
  ** specialization conforms to the interface of the generic version
  ** of mlc::internal::value_.
  **
  ** \see mlc::bool_<true>
  */

  template <>
  struct bool_<false> : public internal::value_<bool, false>
  {
  };

  typedef bool_<false> false_;


} // end of namespace mlc


# include <mlc/logic.hh>


#endif // ! METALIC_BOOL_HH
