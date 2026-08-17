/**
 * File generated with GEGELATI v2.0.0
 * On the 2026-07-16 10:06:50
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

# define NB_TEAMS 20

void inferenceTPG(int* actions, 
					const float * __restrict__ in1, 
					const float * __restrict__ in2, 
					const float * __restrict__ in3, 
					const float * __restrict__ in4);

#ifdef __cplusplus
}
#endif

#endif
