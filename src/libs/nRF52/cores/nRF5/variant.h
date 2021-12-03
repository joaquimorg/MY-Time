
#ifndef _VARIANT_NR52832_
#define _VARIANT_NR52832_


/** Master clock frequency */
#define VARIANT_MCK       (64000000ul)

#define USE_LFXO      // Board uses 32khz crystal for LF
// define USE_LFRC    // Board uses RC for LF


#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

#define PINS_COUNT           (32u)
#define NUM_DIGITAL_PINS     (32u)
#define NUM_ANALOG_INPUTS    (8u)
#define NUM_ANALOG_OUTPUTS   (0u)

#define WIRE_INTERFACES_COUNT 1

#ifdef __cplusplus
}
#endif

#endif