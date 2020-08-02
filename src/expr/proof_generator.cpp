/*********************                                                        */
/*! \file proof_generator.cpp
 ** \verbatim
 ** Top contributors (to current version):
 **   Andrew Reynolds
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2020 by the authors listed in the file AUTHORS
 ** in the top-level source directory) and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief Implementation of proof generator utility
 **/

#include "expr/proof_generator.h"

#include "expr/proof.h"
#include "options/smt_options.h"

namespace CVC4 {

std::ostream& operator<<(std::ostream& out, CDPOverwrite opol)
{
  switch (opol)
  {
    case CDPOverwrite::ALWAYS: out << "ALWAYS"; break;
    case CDPOverwrite::ASSUME_ONLY: out << "ASSUME_ONLY"; break;
    case CDPOverwrite::NEVER: out << "NEVER"; break;
    default: out << "CDPOverwrite:unknown"; break;
  }
  return out;
}

ProofGenerator::ProofGenerator() {}

ProofGenerator::~ProofGenerator() {}

std::shared_ptr<ProofNode> ProofGenerator::getProofFor(Node f)
{
  Unreachable() << "ProofGenerator::getProofFor: " << identify()
                << " has no implementation" << std::endl;
  return nullptr;
}

bool ProofGenerator::addProofTo(Node f, CDProof* pf, CDPOverwrite opolicy)
{
  Trace("pfgen") << "ProofGenerator::addProofTo: " << f << "..." << std::endl;
  Assert(pf != nullptr);
  // plug in the proof provided by the generator, if it exists
  std::shared_ptr<ProofNode> apf = getProofFor(f);
  if (apf != nullptr)
  {
    Trace("pfgen") << "...got proof " << *apf.get() << std::endl;
    // Add the proof, without deep copying.
    if (pf->addProof(apf, opolicy, false))
    {
      Trace("pfgen") << "...success!" << std::endl;
      return true;
    }
    Trace("pfgen") << "...failed to add proof" << std::endl;
  }
  else
  {
    Trace("pfgen") << "...failed, no proof" << std::endl;
    Assert(false) << "Failed to get proof from generator for fact " << f;
  }
  return false;
}

void pfgEnsureClosed(Node proven, ProofGenerator * pg, const char* c, const char* ctx, bool reqGen)
{
  if (!options::proofNew())
  {
    // proofs not enabled, do not do check
    return;
  }
  bool isTraceDebug = Trace.isOn(c);
  if (!options::proofNewEagerChecking() && !isTraceDebug)
  {
    // trace is off and proof new eager checking is off, do not do check
    return;
  }
  std::stringstream ss;
  ss << (pg==nullptr ? "null" : pg->identify()) << " in context " << ctx;
  std::stringstream sdiag;
  sdiag  << ", use -t " << c << " for details";
  Trace(c) << "=== TrustNode::debugCheckClosed: " << ss.str() << std::endl;
  Trace(c) << "Check proof of " << proven << std::endl;
  if (pg == nullptr)
  {
    // only failure if flag is true
    if (reqGen)
    {
      AlwaysAssert(false)
          << "...TrustNode::debugCheckClosed: no generator in context " << ctx << sdiag.str();
    }
    Trace(c) << "...TrustNode::debugCheckClosed: no generator in context "
             << ctx << std::endl;
    return;
  }
  std::shared_ptr<ProofNode> pn = pg->getProofFor(proven);
  if (pn == nullptr)
  {
    AlwaysAssert(false) << "...TrustNode::debugCheckClosed: null proof from "
                        << ss.str() << sdiag.str();
  }
  Trace(c) << *pn.get();
  Trace(c) << std::endl << "====" << std::endl;
  AlwaysAssert(pn->isClosed())
      << "...TrustNode::debugCheckClosed: open proof from " << ss.str() << sdiag.str();
}

}  // namespace CVC4
