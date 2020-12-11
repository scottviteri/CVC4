/*********************                                                        */
/*! \file bound_var_manager.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Bound variable manager utility
 **/

#include "cvc4_private.h"

#ifndef CVC4__EXPR__BOUND_VAR_MANAGER_H
#define CVC4__EXPR__BOUND_VAR_MANAGER_H

#include <string>
#include <unordered_set>

#include "expr/node.h"

namespace CVC4 {

/**
 * Bound variable manager.
 * 
 * This class is responsible for constructing BOUND_VARIABLE that are
 * canonical based on cache keys (Node). It does this using expression
 * attributes on these nodes.
 */
class BoundVarManager
{
 public:
  BoundVarManager();
  ~BoundVarManager();
  /**
   * Enable or disable keeping cache values. If we keep cache values, then
   * the bound variables returned by the methods below are deterministic in the
   * lifetime of the NodeManager we are using.
   */
  void enableKeepCacheValues(bool isEnabled=true);
  /** 
   * Make a bound variable of type tn and name tn, cached based on (n, attr),
   * where attr is an attribute class of the form:
   *   expr::Attribute<id, Node>
   * This variable is unique for (attr, n) during the lifetime of n. If
   * this bound variable manager is configured to keep cache values, then
   * n is added to the d_cacheVals set and survives in the lifetime of the
   * current node manager.
   * 
   * Returns the bound variable.
   */
  template <class T>
  Node mkBoundVar(const T& attr, Node n, TypeNode tn)
  {
    if (n.hasAttribute(attr))
    {
      Assert (n.getAttribute(attr).getType()==tn);
      return n.getAttribute(attr);
    }
    Node v = NodeManager::currentNM()->mkBoundVar(tn);
    n.setAttribute(attr, v);
    // if we are keeping cache values, insert it to the set
    if (d_keepCacheVals)
    {
      d_cacheVals.insert(n);
    }
    return v;
  }
  /** Same as above, with a name for the bound variable. */
  template <class T>
  Node mkBoundVar(const T& attr, Node n, const std::string& name, TypeNode tn)
  {
    Node v = mkBoundVar<T>(attr, n, tn);
    // set the name of n
    setNameAttr(n, name);
    return v;
  }
  //---------------------------------- utilities for computing Node hash
  /** get hash value from two nodes, returns SEXPR */
  static Node getHashValue(TNode cv1, TNode cv2);
  /** get hash value, returns a constant rational node */
  static Node getHashValue(uint32_t i);
  /** get hash value, return SEXPR of cv and constant rational node */
  static Node getHashValue(TNode cv, uint32_t i);
  //---------------------------------- end utilities for computing Node hash
private:
  /** Set name of bound variable to name */
  static void setNameAttr(Node v, const std::string& name);
  /** Whether we keep cache values */
  bool d_keepCacheVals;
  /** The set of cache values we have used */
  std::unordered_set<Node, NodeHashFunction> d_cacheVals;
};

}  // namespace CVC4

#endif /* CVC4__EXPR__BOUND_VAR_MANAGER_H */