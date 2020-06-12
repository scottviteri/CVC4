/*********************                                                        */
/*! \file theory_preprocessor.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Dejan Jovanovic, Morgan Deters, Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2019 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The theory preprocessor.
 **/

#include "cvc4_private.h"

#ifndef CVC4__THEORY__THEORY_PREPROCESSOR_H
#define CVC4__THEORY__THEORY_PREPROCESSOR_H

#include <unordered_map>

#include "expr/node.h"
#include "preprocessing/assertion_pipeline.h"

namespace CVC4 {

class LogicInfo;
class TheoryEngine;
class RemoveTermFormulas;
class LazyCDProof;

namespace theory {

/**
 * The preprocessor used in TheoryEngine.
 */
class TheoryPreprocessor
{
  typedef std::unordered_map<Node, Node, NodeHashFunction> NodeMap;

 public:
  /** Constructs a theory preprocessor */
  TheoryPreprocessor(TheoryEngine& engine, RemoveTermFormulas& tfr);
  /** Destroys a theory preprocessor */
  ~TheoryPreprocessor();
  /** Clear the cache of this class */
  void clearCache();
  /**
   * Preprocesses node and stores it along with lemmas generated by
   * preprocessing into the assertion pipeline lemmas. The (optional) argument
   * lcp is the proof that stores a proof of all top-level formulas in lemmas,
   * assuming that lcp initially contains a proof of node. The flag
   * doTheoryPreprocess is whether we should run theory-specific preprocessing.
   */
  void preprocess(TNode node,
                  preprocessing::AssertionPipeline& lemmas,
                  bool doTheoryPreprocess,
                  LazyCDProof* lcp);
  /**
   * Runs theory specific preprocessing on the non-Boolean parts of
   * the formula.  This is only called on input assertions, after ITEs
   * have been removed.
   */
  Node theoryPreprocess(TNode node, LazyCDProof* lp);

 private:
  /** Reference to owning theory engine */
  TheoryEngine& d_engine;
  /** Logic info of theory engine */
  const LogicInfo& d_logicInfo;
  /** Cache for theory-preprocessing of assertions */
  NodeMap d_ppCache;
  /** The term formula remover */
  RemoveTermFormulas& d_tfr;
  /** Helper for theoryPreprocess */
  Node ppTheoryRewrite(TNode term, LazyCDProof* lp);
};

}  // namespace theory
}  // namespace CVC4

#endif /* CVC4__THEORY__THEORY_PREPROCESSOR_H */
