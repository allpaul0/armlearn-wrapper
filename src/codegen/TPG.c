/**
 * File generated with GEGELATI v2.0.0
 * Refactored version: TX logic moved to functions.
 */

#include "TPG_program.h"
#include "TPG.h"

int bestProgram(fixedpt *results, int nb) {
	int bestProgram = 0;
	fixedpt bestScore = results[0];
	for (int i = 1; i < nb; i++) {
		fixedpt challengerScore = results[i];
		if (challengerScore >= bestScore) {
			bestProgram = i;
			bestScore = challengerScore;
		}
	}
	return bestProgram;
}

enum vertices {T0, A1, A2, A3, A4, A5, A6};

/* ========================= */
/* ===== TX FUNCTIONS ====== */
/* ========================= */

enum vertices T0_func(const typeInf * restrict in1,
                      const typeInf * restrict in2,
                      const typeInf * restrict in3,
                      const typeInf * restrict in4)
{
	const enum vertices next[6] = { A1, A2, A3, A4, A5, A6 };
	typeInf scores[6];
	scores[0] = P0(in1, in2, in3, in4);
	scores[1] = P1(in1, in2, in3, in4);
	scores[2] = P2(in1, in2, in3, in4);
	scores[3] = P3(in1, in2, in3, in4);
	scores[4] = P4(in1, in2, in3, in4);
	scores[5] = P5(in1, in2, in3, in4);
	return next[bestProgram(scores, 6)];
}

/* ========================= */
/* ===== INFERENCE LOOP ==== */
/* ========================= */

void inferenceTPG(typeInf* actions,
                  const typeInf * restrict in1,
                  const typeInf * restrict in2,
                  const typeInf * restrict in3,
                  const typeInf * restrict in4)
{
	enum vertices currentVertex = T0;

	while (1) {
		switch (currentVertex) {
			case T0:  currentVertex = T0_func(in1, in2, in3, in4); break;
			case A1: actions[0] = 1; return;
			case A2: actions[0] = 2; return;
			case A3: actions[0] = 3; return;	
			case A4: actions[0] = 4; return;
			case A5: actions[0] = 5; return;
			case A6: actions[0] = 6; return;
		}
	}
}