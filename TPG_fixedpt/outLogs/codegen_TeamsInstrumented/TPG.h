/**
 * File generated with GEGELATI v2.0.0
 * On the 2026-05-28 21:28:33
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

# define NB_TEAMS 22

void inferenceTPG(fixedpt* actions, 
					const fixedpt * __restrict__ in1, 
					const fixedpt * __restrict__ in2, 
					const fixedpt * __restrict__ in3, 
					const fixedpt * __restrict__ in4, 
					uint32_t * team_cycles);

#ifdef __cplusplus
}
#endif

#endif
