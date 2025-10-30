/**
 * File generated with GEGELATI v2.0.0
 * On the 2025-10-29 10:27:45
 * With the CodeGen::TPGGenerationEngine.
 */

#include "TPG.h"

int bestProgram(double *results, int nb) {
	int bestProgram = 0;
	double bestScore = (isnan(results[0]))? -INFINITY : results[0];
	for (int i = 1; i < nb; i++) {
		double challengerScore = (isnan(results[i]))? -INFINITY : results[i];
		if (challengerScore >= bestScore) {
			bestProgram = i;
			bestScore = challengerScore;
		}
	}
	return bestProgram;
}

enum vertices {T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16, T17, T18, A19, A20, A21, A22, A23, A24, A25, A26, };

void inferenceTPG(double* actions) {

	enum vertices currentVertex = T18;
	while(1) {
		switch (currentVertex) {
		case T0: {
				const enum vertices next[1] = { A20,  };

				double T0Scores[1];

				T0Scores[0] = P0();

				int best = bestProgram(T0Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T1: {
				const enum vertices next[3] = { A19, T0, A22,  };

				double T1Scores[3];

				T1Scores[0] = P1();
				T1Scores[1] = P2();
				T1Scores[2] = P3();

				int best = bestProgram(T1Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T2: {
				const enum vertices next[2] = { A23, A21,  };

				double T2Scores[2];

				T2Scores[0] = P4();
				T2Scores[1] = P5();

				int best = bestProgram(T2Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T3: {
				const enum vertices next[1] = { T1,  };

				double T3Scores[1];

				T3Scores[0] = P6();

				int best = bestProgram(T3Scores, 1);
				currentVertex = next[best];
				break;
			}
		case T4: {
				const enum vertices next[3] = { T3, T1, A23,  };

				double T4Scores[3];

				T4Scores[0] = P7();
				T4Scores[1] = P8();
				T4Scores[2] = P9();

				int best = bestProgram(T4Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T5: {
				const enum vertices next[3] = { A24, A25, A23,  };

				double T5Scores[3];

				T5Scores[0] = P10();
				T5Scores[1] = P11();
				T5Scores[2] = P12();

				int best = bestProgram(T5Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T6: {
				const enum vertices next[5] = { T1, A24, A23, T1, A21,  };

				double T6Scores[5];

				T6Scores[0] = P13();
				T6Scores[1] = P14();
				T6Scores[2] = P15();
				T6Scores[3] = P16();
				T6Scores[4] = P17();

				int best = bestProgram(T6Scores, 5);
				currentVertex = next[best];
				break;
			}
		case T7: {
				const enum vertices next[3] = { T2, T1, T1,  };

				double T7Scores[3];

				T7Scores[0] = P18();
				T7Scores[1] = P19();
				T7Scores[2] = P20();

				int best = bestProgram(T7Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T8: {
				const enum vertices next[4] = { T7, A21, A24, A24,  };

				double T8Scores[4];

				T8Scores[0] = P21();
				T8Scores[1] = P22();
				T8Scores[2] = P10();
				T8Scores[3] = P23();

				int best = bestProgram(T8Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T9: {
				const enum vertices next[3] = { T6, T1, T8,  };

				double T9Scores[3];

				T9Scores[0] = P24();
				T9Scores[1] = P19();
				T9Scores[2] = P25();

				int best = bestProgram(T9Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T10: {
				const enum vertices next[6] = { T4, T1, A24, T1, T8, A23,  };

				double T10Scores[6];

				T10Scores[0] = P26();
				T10Scores[1] = P19();
				T10Scores[2] = P10();
				T10Scores[3] = P27();
				T10Scores[4] = P28();
				T10Scores[5] = P29();

				int best = bestProgram(T10Scores, 6);
				currentVertex = next[best];
				break;
			}
		case T11: {
				const enum vertices next[4] = { T6, T8, A23, T1,  };

				double T11Scores[4];

				T11Scores[0] = P24();
				T11Scores[1] = P25();
				T11Scores[2] = P30();
				T11Scores[3] = P19();

				int best = bestProgram(T11Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T12: {
				const enum vertices next[3] = { A24, T11, T10,  };

				double T12Scores[3];

				T12Scores[0] = P31();
				T12Scores[1] = P32();
				T12Scores[2] = P33();

				int best = bestProgram(T12Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T13: {
				const enum vertices next[2] = { T1, A24,  };

				double T13Scores[2];

				T13Scores[0] = P34();
				T13Scores[1] = P35();

				int best = bestProgram(T13Scores, 2);
				currentVertex = next[best];
				break;
			}
		case T14: {
				const enum vertices next[4] = { T12, T11, A20, T13,  };

				double T14Scores[4];

				T14Scores[0] = P36();
				T14Scores[1] = P32();
				T14Scores[2] = P37();
				T14Scores[3] = P38();

				int best = bestProgram(T14Scores, 4);
				currentVertex = next[best];
				break;
			}
		case T15: {
				const enum vertices next[3] = { T14, T10, T5,  };

				double T15Scores[3];

				T15Scores[0] = P39();
				T15Scores[1] = P40();
				T15Scores[2] = P41();

				int best = bestProgram(T15Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T16: {
				const enum vertices next[7] = { T9, A24, A24, A24, T10, T10, T11,  };

				double T16Scores[7];

				T16Scores[0] = P42();
				T16Scores[1] = P43();
				T16Scores[2] = P44();
				T16Scores[3] = P45();
				T16Scores[4] = P46();
				T16Scores[5] = P47();
				T16Scores[6] = P48();

				int best = bestProgram(T16Scores, 7);
				currentVertex = next[best];
				break;
			}
		case T17: {
				const enum vertices next[3] = { A26, T16, T15,  };

				double T17Scores[3];

				T17Scores[0] = P49();
				T17Scores[1] = P50();
				T17Scores[2] = P51();

				int best = bestProgram(T17Scores, 3);
				currentVertex = next[best];
				break;
			}
		case T18: {
				const enum vertices next[4] = { A26, T16, A21, T17,  };

				double T18Scores[4];

				T18Scores[0] = P52();
				T18Scores[1] = P50();
				T18Scores[2] = P22();
				T18Scores[3] = P53();

				int best = bestProgram(T18Scores, 4);
				currentVertex = next[best];
				break;
			}
		case A19: {
				actions[0] = 4;
				return;
			}
		case A20: {
				actions[0] = 0;
				return;
			}
		case A21: {
				actions[0] = 7;
				return;
			}
		case A22: {
				actions[0] = 8;
				return;
			}
		case A23: {
				actions[0] = 5;
				return;
			}
		case A24: {
				actions[0] = 6;
				return;
			}
		case A25: {
				actions[0] = 1;
				return;
			}
		case A26: {
				actions[0] = 2;
				return;
			}
		}
	}
}
