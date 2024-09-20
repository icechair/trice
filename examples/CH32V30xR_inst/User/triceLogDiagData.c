/*! \file triceLogDiagnosticData.c
\author Thomas.Hoehenleitner [at] seerose.net
*******************************************************************************/

#include "trice.h"

#ifdef __cplusplus
extern "C" {
#endif

#if TRICE_DIAGNOSTICS == 1

//! TriceLogDiagnosticValues shows the max used buffer space.
void TriceLogDiagnosticValues(void) {

#ifdef SEGGER_RTT
	int RTT0_wrSpaceMin = BUFFER_SIZE_UP - RTT0_writeDepthMax;
	if ((RTT0_wrSpaceMin < TRICE_BUFFER_SIZE)) {
		trice( iD(3303), "WARNING:RTT0_writeDepthMax=%u (BUFFER_SIZE_UP=%u)\n", RTT0_writeDepthMax, BUFFER_SIZE_UP);
	} else {
		trice( iD(7884), "diag:RTT0_writeDepthMax=%u (BUFFER_SIZE_UP=%u)\n", RTT0_writeDepthMax, BUFFER_SIZE_UP);
	}
#endif // #ifdef SEGGER_RTT

	if (TriceErrorCount > 0) {
		trice( iD(4971), "err: TriceErrorCount = %u\n", TriceErrorCount);
	}
#if TRICE_PROTECT == 1
	if (TriceDirectOverflowCount != 0) {
		trice( iD(6124), "err: TriceDirectOverflowCount = %u\n", TriceDirectOverflowCount);
	}
	if (TriceDeferredOverflowCount != 0) {
		trice( iD(4360), "err: TriceDeferredOverflowCount = %u\n", TriceDeferredOverflowCount);
	}
#endif

	unsigned triceSingleDepthMax = TRICE_DATA_OFFSET + (TriceSingleMaxWordCount << 2); // lint !e845 Info 845: The left argument to operator '<<' is certain to be 0
	if (triceSingleDepthMax <= TRICE_BUFFER_SIZE) {
		trice16( iD(2196), "diag:triceSingleDepthMax =%4u of %d (TRICE_BUFFER_SIZE)\n", triceSingleDepthMax, TRICE_BUFFER_SIZE);
	} else {
		trice16( iD(4462), "err:triceSingleDepthMax =%4u of %d (TRICE_BUFFER_SIZE overflow!)\n", triceSingleDepthMax, TRICE_BUFFER_SIZE);
	}

	if (TriceDataOffsetDepthMax < 0 || TriceDataOffsetDepthMax >= TRICE_DATA_OFFSET) {
		trice16( iD(7287), "err:TriceDataOffsetDepthMax = %d of %d (TRICE_DATA_OFFSET)\n", TriceDataOffsetDepthMax, TRICE_DATA_OFFSET);
	} else if (TriceDataOffsetDepthMax != 0) {
		trice16( iD(6016), "diag:TriceDataOffsetDepthMax = %d of %d (TRICE_DATA_OFFSET)\n", TriceDataOffsetDepthMax, TRICE_DATA_OFFSET);
	}

	if (TriceDynBufTruncateCount != 0) {
		trice8( iD(6724), "err: TriceDynBufTruncateCount = %u\n", TriceDynBufTruncateCount);
	}

#if TRICE_BUFFER == TRICE_STACK_BUFFER
#endif // #if TRICE_BUFFER == TRICE_STACK_BUFFER

#if TRICE_BUFFER == TRICE_STATIC_BUFFER
#endif // #if TRICE_BUFFER == TRICE_STATIC_BUFFER

#if TRICE_BUFFER == TRICE_DOUBLE_BUFFER
	if (TriceHalfBufferDepthMax <= TRICE_DEFERRED_BUFFER_SIZE / 2) {
		TRice16( iD(4707), "diag:TriceHalfBufferDepthMax =%4u of%5d\n", TriceHalfBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE / 2);
	} else {
		TRice16( iD(1986), "err:TriceHalfBufferDepthMax =%4u of%5d (overflow!)\n", TriceHalfBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE / 2);
	}
#endif // #if TRICE_BUFFER == TRICE_DOUBLE_BUFFER

#if TRICE_BUFFER == TRICE_RING_BUFFER
	if (TriceRingBufferDepthMax <= TRICE_DEFERRED_BUFFER_SIZE) {
		trice16( iD(5704), "diag:triceRingBufferDepthMax =%4u of%5d\n", TriceRingBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE);
	} else {
		trice16( iD(3908), "err:triceRingBufferDepthMax =%4u of%5d (overflow!)\n", TriceRingBufferDepthMax, TRICE_DEFERRED_BUFFER_SIZE);
	}
#endif // #if TRICE_BUFFER == TRICE_RING_BUFFER
}

#endif // #if TRICE_DIAGNOSTICS == 1
