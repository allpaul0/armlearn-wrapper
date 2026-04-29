/**
 * File generated with GEGELATI v2.0.0
 * On the 2025-11-21 13:02:17
 * With the CodeGen::TPGGenerationEngine.
 */

#ifndef C_TPG_H
#define C_TPG_H

#ifdef __cplusplus
extern "C" {
#endif

#include "TPG_program.h"

#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <float.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

#include "externHeader.h"

void inferenceTPG(typeInf* actions, 
					const typeInf *  in1,
                	const typeInf *  in2,
					const typeInf *  in3,
                	const typeInf *  in4);

#ifdef __cplusplus
}
#endif

#endif
