/*
 * Copyright (c) 2015 - 2020, Nordic Semiconductor ASA
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice, this
 *    list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from this
 *    software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef NRFX_PWM_H__
#define NRFX_PWM_H__

#include <nrfx.h>
#include <hal/nrf_pwm.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @defgroup nrfx_pwm PWM driver
 * @{
 * @ingroup nrf_pwm
 * @brief   Pulse Width Modulation (PWM) peripheral driver.
 */

/** @brief PWM driver instance data structure. */
typedef struct
{
    NRF_PWM_Type * p_registers;  ///< Pointer to the structure with PWM peripheral instance registers.
    uint8_t        drv_inst_idx; ///< Index of the driver instance. For internal use only.
} nrfx_pwm_t;

/** @brief Macro for creating a PWM driver instance. */
#define NRFX_PWM_INSTANCE(id)                               \
{                                                           \
    .p_registers  = NRFX_CONCAT_2(NRF_PWM, id),             \
    .drv_inst_idx = NRFX_CONCAT_3(NRFX_PWM, id, _INST_IDX), \
}

#ifndef __NRFX_DOXYGEN__
enum {
#if NRFX_CHECK(NRFX_PWM0_ENABLED)
    NRFX_PWM0_INST_IDX,
#endif
#if NRFX_CHECK(NRFX_PWM1_ENABLED)
    NRFX_PWM1_INST_IDX,
#endif
#if NRFX_CHECK(NRFX_PWM2_ENABLED)
    NRFX_PWM2_INST_IDX,
#endif
#if NRFX_CHECK(NRFX_PWM3_ENABLED)
    NRFX_PWM3_INST_IDX,
#endif
    NRFX_PWM_ENABLED_COUNT
};
#endif

/**
 * @brief This value can be provided instead of a pin number for any channel
 *        to specify that its output is not used and therefore does not need
 *        to be connected to a pin.
 */
#define NRFX_PWM_PIN_NOT_USED    0xFF

/** @brief This value can be added to a pin number to invert its polarity (set idle state = 1). */
#define NRFX_PWM_PIN_INVERTED    0x80

/** @brief PWM driver configuration structure. */
typedef struct
{
    uint8_t output_pins[NRF_PWM_CHANNEL_COUNT]; ///< Pin numbers for individual output channels (optional).
                                                /**< Use @ref NRFX_PWM_PIN_NOT_USED
                                                 *   if a given output channel is not needed. */
    uint8_t            irq_priority; ///< Interrupt priority.
    nrf_pwm_clk_t      base_clock;   ///< Base clock frequency.
    nrf_pwm_mode_t     count_mode;   ///< Operating mode of the pulse generator counter.
    uint16_t           top_value;    ///< Value up to which the pulse generator counter counts.
    nrf_pwm_dec_load_t load_mode;    ///< Mode of loading sequence data from RAM.
    nrf_pwm_dec_step_t step_mode;    ///< Mode of advancing the active sequence.
    bool               skip_gpio_cfg; ///< Skip the GPIO configuration
                                      /**< When this flag is set, the user is responsible for
                                       *   providing the proper configuration of the output pins,
                                       *   as the driver does not touch it at all. */
} nrfx_pwm_config_t;

/**
 * @brief PWM driver default configuration.
 *
 * This configuration sets up PWM with the following options:
 * - clock frequency: 1 MHz
 * - count up
 * - top value: 1000 clock ticks
 * - load mode: common
 * - step mode: auto
 *
 * @param[in] _out_0 PWM output 0 pin.
 * @param[in] _out_1 PWM output 1 pin.
 * @param[in] _out_2 PWM output 2 pin.
 * @param[in] _out_3 PWM output 3 pin.
 */
#define NRFX_PWM_DEFAULT_CONFIG(_out_0, _out_1, _out_2, _out_3) \
{                                                               \
    .output_pins   = { _out_0,                                  \
                       _out_1,                                  \
                       _out_2,                                  \
                       _out_3                                   \
                     },                                         \
    .irq_priority  = NRFX_PWM_DEFAULT_CONFIG_IRQ_PRIORITY,      \
    .base_clock    = NRF_PWM_CLK_1MHz,                          \
    .count_mode    = NRF_PWM_MODE_UP,                           \
    .top_value     = 1000,                                      \
    .load_mode     = NRF_PWM_LOAD_COMMON,                       \
    .step_mode     = NRF_PWM_STEP_AUTO,                         \
    .skip_gpio_cfg = false                                      \
}

/** @brief PWM flags providing additional playback options. */
typedef enum
{
    NRFX_PWM_FLAG_STOP = 0x01, /**< When the requested playback is finished,
                                    the peripheral will be stopped.
                                    @note The STOP task is triggered when
                                    the last value of the final sequence is
                                    loaded from RAM, and the peripheral stops
                                    at the end of the current PWM period.
                                    For sequences with configured repeating
                                    of duty cycle values, this might result in
                                    less than the requested number of repeats
                                    of the last value. */
    NRFX_PWM_FLAG_LOOP = 0x02, /**< When the requested playback is finished,
                                    it will be started from the beginning.
                                    This flag is ignored if used together
                                    with @ref NRFX_PWM_FLAG_STOP.
                                    @note The playback restart is done via a
                                    shortcut configured in the PWM peripheral.
                                    This shortcut triggers the proper starting
                                    task when the final value of previous
                                    playback is read from RAM and applied to
                                    the pulse generator counter.
                                    When this mechanism is used together with
                                    the @ref NRF_PWM_STEP_TRIGGERED mode,
                                    the playback restart will occur right
                                    after switching to the final value (this
                                    final value will be played only once). */
    NRFX_PWM_FLAG_SIGNAL_END_SEQ0 = 0x04, /**< The event handler is to be
                                               called when the last value
                                               from sequence 0 is loaded. */
    NRFX_PWM_FLAG_SIGNAL_END_SEQ1 = 0x08, /**< The event handler is to be
                                               called when the last value
                                               from sequence 1 is loaded. */
    NRFX_PWM_FLAG_NO_EVT_FINISHED = 0x10, /**< The playback finished event
                                               (enabled by default) is to be
                                               suppressed. */
    NRFX_PWM_FLAG_START_VIA_TASK = 0x80, /**< The playback must not be
                                              started directly by the called
                                              function. Instead, the function
                                              must only prepare it and
                                              return the address of the task
                                              to be triggered to start the
                                              playback. */
} nrfx_pwm_flag_t;

/** @brief PWM driver event type. */
typedef enum
{
    NRFX_PWM_EVT_FINISHED, ///< Sequence playback finished.
    NRFX_PWM_EVT_END_SEQ0, /**< End of sequence 0 reached. Its data can be
                                safely modified now. */
    NRFX_PWM_EVT_END_SEQ1, /**< End of sequence 1 reached. Its data can be
                                safely modified now. */
    NRFX_PWM_EVT_STOPPED,  ///< The PWM peripheral has been stopped.
} nrfx_pwm_evt_type_t;

/** @brief PWM driver event handler type. */
typedef void (* nrfx_pwm_handler_t)(nrfx_pwm_evt_type_t event_type, void * p_context);

/**
 * @brief Function for initializing the PWM driver.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] p_config   Pointer to the structure with the initial configuration.
 * @param[in] handler    Event handler provided by the user. If NULL is passed
 *                       instead, event notifications are not done and PWM
 *                       interrupts are disabled.
 * @param[in] p_context  Context passed to the event handler.
 *
 * @retval NRFX_SUCCESS             Initialization was successful.
 * @retval NRFX_ERROR_INVALID_STATE The driver was already initialized.
 */
nrfx_err_t nrfx_pwm_init(nrfx_pwm_t const *        p_instance,
                         nrfx_pwm_config_t const * p_config,
                         nrfx_pwm_handler_t        handler,
                         void *                    p_context);

/**
 * @brief Function for uninitializing the PWM driver.
 *
 * If any sequence playback is in progress, it is stopped immediately.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 */
void nrfx_pwm_uninit(nrfx_pwm_t const * p_instance);

/**
 * @brief Function for starting a single sequence playback.
 *
 * To take advantage of the looping mechanism in the PWM peripheral, both
 * sequences must be used (single sequence can be played back only once by
 * the peripheral). Therefore, the provided sequence is internally set and
 * played back as both sequence 0 and sequence 1. Consequently, if the end of
 * sequence notifications are required, events for both sequences must be
 * used (that is, both the @ref NRFX_PWM_FLAG_SIGNAL_END_SEQ0 flag
 * and the @ref NRFX_PWM_FLAG_SIGNAL_END_SEQ1 flag must be specified, and
 * the @ref NRFX_PWM_EVT_END_SEQ0 event and the @ref NRFX_PWM_EVT_END_SEQ1
 * event must be handled in the same way).
 *
 * Use the @ref NRFX_PWM_FLAG_START_VIA_TASK flag if you want the playback
 * to be only prepared by this function, and you want to start it later by
 * triggering a task (for example, by using PPI). The function will then return
 * the address of the task to be triggered.
 *
 * @note The array containing the duty cycle values for the specified sequence
 *       must be in RAM and cannot be allocated on the stack.
 *       For detailed information, see @ref nrf_pwm_sequence_t.
 *
 * @param[in] p_instance     Pointer to the driver instance structure.
 * @param[in] p_sequence     Sequence to be played back.
 * @param[in] playback_count Number of playbacks to be performed (must not be 0).
 * @param[in] flags          Additional options. Pass any combination of
 *                           @ref nrfx_pwm_flag_t "playback flags", or 0
 *                           for default settings.
 *
 * @return Address of the task to be triggered to start the playback if the @ref
 *         NRFX_PWM_FLAG_START_VIA_TASK flag was used, 0 otherwise.
 */
uint32_t nrfx_pwm_simple_playback(nrfx_pwm_t const *         p_instance,
                                  nrf_pwm_sequence_t const * p_sequence,
                                  uint16_t                   playback_count,
                                  uint32_t                   flags);

/**
 * @brief Function for starting a two-sequence playback.
 *
 * Use the @ref NRFX_PWM_FLAG_START_VIA_TASK flag if you want the playback
 * to be only prepared by this function, and you want to start it later by
 * triggering a task (using PPI for instance). The function will then return
 * the address of the task to be triggered.
 *
 * @note The array containing the duty cycle values for the specified sequence
 *       must be in RAM and cannot be allocated on the stack.
 *       For detailed information, see @ref nrf_pwm_sequence_t.
 *
 * @param[in] p_instance     Pointer to the driver instance structure.
 * @param[in] p_sequence_0   First sequence to be played back.
 * @param[in] p_sequence_1   Second sequence to be played back.
 * @param[in] playback_count Number of playbacks to be performed (must not be 0).
 * @param[in] flags          Additional options. Pass any combination of
 *                           @ref nrfx_pwm_flag_t "playback flags", or 0
 *                           for default settings.
 *
 * @return Address of the task to be triggered to start the playback if the @ref
 *         NRFX_PWM_FLAG_START_VIA_TASK flag was used, 0 otherwise.
 */
uint32_t nrfx_pwm_complex_playback(nrfx_pwm_t const *         p_instance,
                                   nrf_pwm_sequence_t const * p_sequence_0,
                                   nrf_pwm_sequence_t const * p_sequence_1,
                                   uint16_t                   playback_count,
                                   uint32_t                   flags);

/**
 * @brief Function for advancing the active sequence.
 *
 * This function only applies to @ref NRF_PWM_STEP_TRIGGERED mode.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 */
NRFX_STATIC_INLINE void nrfx_pwm_step(nrfx_pwm_t const * p_instance);

/**
 * @brief Function for stopping the sequence playback.
 *
 * The playback is stopped at the end of the current PWM period.
 * This means that if the active sequence is configured to repeat each duty
 * cycle value for a certain number of PWM periods, the last played value
 * might appear on the output less times than requested.
 *
 * @note This function can be instructed to wait until the playback is stopped
 *       (by setting @p wait_until_stopped to true). Depending on
 *       the length of the PMW period, this might take a significant amount of
 *       time. Alternatively, the @ref nrfx_pwm_is_stopped function can be
 *       used to poll the status, or the @ref NRFX_PWM_EVT_STOPPED event can
 *       be used to get the notification when the playback is stopped, provided
 *       the event handler is defined.
 *
 * @param[in] p_instance         Pointer to the driver instance structure.
 * @param[in] wait_until_stopped If true, the function will not return until
 *                               the playback is stopped.
 *
 * @retval true  The PWM peripheral is stopped.
 * @retval false The PWM peripheral is not stopped.
 */
bool nrfx_pwm_stop(nrfx_pwm_t const * p_instance, bool wait_until_stopped);

/**
 * @brief Function for checking the status of the PWM peripheral.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 *
 * @retval true  The PWM peripheral is stopped.
 * @retval false The PWM peripheral is not stopped.
 */
bool nrfx_pwm_is_stopped(nrfx_pwm_t const * p_instance);

/**
 * @brief Function for updating the sequence data during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] p_sequence Pointer to the new sequence definition.
 */
NRFX_STATIC_INLINE void nrfx_pwm_sequence_update(nrfx_pwm_t const *         p_instance,
                                                 uint8_t                    seq_id,
                                                 nrf_pwm_sequence_t const * p_sequence);

/**
 * @brief Function for updating the pointer to the duty cycle values
 *        in the specified sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] values     New pointer to the duty cycle values.
 */
NRFX_STATIC_INLINE void nrfx_pwm_sequence_values_update(nrfx_pwm_t const * p_instance,
                                                        uint8_t            seq_id,
                                                        nrf_pwm_values_t   values);

/**
 * @brief Function for updating the number of duty cycle values
 *        in the specified sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] length     New number of the duty cycle values.
 */
NRFX_STATIC_INLINE void nrfx_pwm_sequence_length_update(nrfx_pwm_t const * p_instance,
                                                        uint8_t            seq_id,
                                                        uint16_t           length);

/**
 * @brief Function for updating the number of repeats for duty cycle values
 *        in the specified sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] repeats    New number of repeats.
 */
NRFX_STATIC_INLINE void nrfx_pwm_sequence_repeats_update(nrfx_pwm_t const * p_instance,
                                                         uint8_t            seq_id,
                                                         uint32_t           repeats);

/**
 * @brief Function for updating the additional delay after the specified
 *        sequence during playback.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] seq_id     Identifier of the sequence (0 or 1).
 * @param[in] end_delay  New end delay value (in PWM periods).
 */
NRFX_STATIC_INLINE void nrfx_pwm_sequence_end_delay_update(nrfx_pwm_t const * p_instance,
                                                           uint8_t            seq_id,
                                                           uint32_t           end_delay);

/**
 * @brief Function for returning the address of a specified PWM task that can
 *        be used in PPI module.
 *
 * @param[in] p_instance Pointer to the driver instance structure.
 * @param[in] task       Requested task.
 *
 * @return Task address.
 */
NRFX_STATIC_INLINE uint32_t nrfx_pwm_task_address_get(nrfx_pwm_t const * p_instance,
                                                      nrf_pwm_task_t     task);

/**
 * @brief Function for returning the address of a specified PWM event that can
 *        be used in PPI module.
 *
 * @param[in] p_instance  Pointer to the driver instance structure.
 * @param[in] event       Requested event.
 *
 * @return Event address.
 */
NRFX_STATIC_INLINE uint32_t nrfx_pwm_event_address_get(nrfx_pwm_t const * p_instance,
                                                       nrf_pwm_event_t    event);

#ifndef NRFX_DECLARE_ONLY
NRFX_STATIC_INLINE void nrfx_pwm_step(nrfx_pwm_t const * p_instance)
{
    nrf_pwm_task_trigger(p_instance->p_registers, NRF_PWM_TASK_NEXTSTEP);
}

NRFX_STATIC_INLINE void nrfx_pwm_sequence_update(nrfx_pwm_t const *         p_instance,
                                                 uint8_t                    seq_id,
                                                 nrf_pwm_sequence_t const * p_sequence)
{
    nrf_pwm_sequence_set(p_instance->p_registers, seq_id, p_sequence);
}

NRFX_STATIC_INLINE void nrfx_pwm_sequence_values_update(nrfx_pwm_t const * p_instance,
                                                        uint8_t            seq_id,
                                                        nrf_pwm_values_t   values)
{
    nrf_pwm_seq_ptr_set(p_instance->p_registers, seq_id, values.p_raw);
}

NRFX_STATIC_INLINE void nrfx_pwm_sequence_length_update(nrfx_pwm_t const * p_instance,
                                                        uint8_t            seq_id,
                                                        uint16_t           length)
{
    nrf_pwm_seq_cnt_set(p_instance->p_registers, seq_id, length);
}

NRFX_STATIC_INLINE void nrfx_pwm_sequence_repeats_update(nrfx_pwm_t const * p_instance,
                                                         uint8_t            seq_id,
                                                         uint32_t           repeats)
{
    nrf_pwm_seq_refresh_set(p_instance->p_registers, seq_id, repeats);
}

NRFX_STATIC_INLINE void nrfx_pwm_sequence_end_delay_update(nrfx_pwm_t const * p_instance,
                                                           uint8_t            seq_id,
                                                           uint32_t           end_delay)
{
    nrf_pwm_seq_end_delay_set(p_instance->p_registers, seq_id, end_delay);
}

NRFX_STATIC_INLINE uint32_t nrfx_pwm_task_address_get(nrfx_pwm_t const * p_instance,
                                                      nrf_pwm_task_t     task)
{
    return nrf_pwm_task_address_get(p_instance->p_registers, task);
}

NRFX_STATIC_INLINE uint32_t nrfx_pwm_event_address_get(nrfx_pwm_t const * p_instance,
                                                       nrf_pwm_event_t    event)
{
    return nrf_pwm_event_address_get(p_instance->p_registers, event);
}
#endif // NRFX_DECLARE_ONLY

/** @} */


void nrfx_pwm_0_irq_handler(void);
void nrfx_pwm_1_irq_handler(void);
void nrfx_pwm_2_irq_handler(void);
void nrfx_pwm_3_irq_handler(void);


#ifdef __cplusplus
}
#endif

#endif // NRFX_PWM_H__
