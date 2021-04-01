/*********************                                                        */
/*! \file sep_skolem_emp.h
 ** \verbatim
 ** Top contributors (to current version):
 **   Yoni Zohar, Mathias Preiner
 ** This file is part of the CVC4 project.
 ** Copyright (c) 2009-2021 by the authors listed in the file AUTHORS
 ** in the top-level source directory and their institutional affiliations.
 ** All rights reserved.  See the file COPYING in the top-level source
 ** directory for licensing information.\endverbatim
 **
 ** \brief The sep-pre-skolem-emp eprocessing pass
 **
 **/

#include "cvc4_private.h"

#ifndef CVC4__PREPROCESSING__PASSES__SEP_SKOLEM_EMP_H
#define CVC4__PREPROCESSING__PASSES__SEP_SKOLEM_EMP_H

#include "preprocessing/preprocessing_pass.h"

namespace cvc5 {
namespace preprocessing {
namespace passes {

class SepSkolemEmp : public PreprocessingPass
{
 public:
  SepSkolemEmp(PreprocessingPassContext* preprocContext);

 protected:
  PreprocessingPassResult applyInternal(
      AssertionPipeline* assertionsToPreprocess) override;
};

}  // namespace passes
}  // namespace preprocessing
}  // namespace cvc5

#endif /* CVC4__PREPROCESSING__PASSES__SEP_SKOLEM_EMP_H */
