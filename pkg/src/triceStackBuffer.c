
#include "trice.h"



//! TriceDepthMax returns the max trice buffer depth until now.
size_t TriceDepthMax( void ){
    return triceDepthMax;
}

unsigned triceDepthMax = 0; //!< triceDepthMax is a diagnostics value usable to optimize buffer size.


void TriceLogBufferInfo( void ){
    TRICE32( Id(13778), "att:Single Trice Stack buf size:%4u", TRICE_SINGLE_MAX_SIZE + TRICE_DATA_OFFSET );
}


#if defined( TRICE_UART ) // direct out to UART
void TriceWrite( uint8_t const * buf, unsigned len ){
    TriceBlockingWrite( buf, len );
}
#endif
