/********************************************************************
 * Firmware lib
 *
 * To use: `import Firmware`
 *******************************************************************/
#include "be_constobj.h"
#include "be_ctypes.h"

extern struct FirmwareGlobal_t FirmwareGlobal;
extern struct TSettings * Settings;
extern const be_ctypes_structure_t be_Firmware_global_struct;
extern const be_ctypes_structure_t be_Firmware_settings_struct;

extern int l_getFreeHeap(bvm *vm);
extern int l_arch(bvm *vm);
extern int be_mqtt_publish(bvm *vm);
extern int l_publish_result(bvm *vm);
extern int l_publish_rule(bvm *vm);
extern int l_cmd(bvm *vm);
extern int l_getoption(bvm *vm);
extern int l_millis(bvm *vm);
extern int l_timereached(bvm *vm);
extern int l_rtc(bvm *vm);
extern int l_time_dump(bvm *vm);
extern int l_strftime(bvm *vm);
extern int l_strptime(bvm *vm);
extern int l_memory(bvm *vm);
extern int l_wifi(bvm *vm);
extern int l_eth(bvm *vm);
extern int l_hostname(bvm *vm);
extern int l_yield(bvm *vm);
extern int l_delay(bvm *vm);
extern int l_delay_microseconds(bvm *vm);
extern int l_scaleuint(bvm *vm);
extern int l_logInfo(bvm *vm);
extern int l_save(bvm *vm);
extern int t_random_byte(bvm *vm);
extern int l_locale(bvm *vm);

extern int l_read_sensors(bvm *vm);

extern int l_respCmnd(bvm *vm);
extern int l_respCmndStr(bvm *vm);
extern int l_respCmndDone(bvm *vm);
extern int l_respCmndError(bvm *vm);
extern int l_respCmndFailed(bvm *vm);
extern int l_resolveCmnd(bvm *vm);

extern int l_respAppend(bvm *vm);
extern int l_webSend(bvm *vm);
extern int l_webSendDecimal(bvm *vm);

extern int l_getlight(bvm *vm);
extern int l_setlight(bvm *vm);
extern int l_getpower(bvm *vm);
extern int l_setpower(bvm *vm);
extern int l_getswitch(bvm *vm);

extern int l_i2cenabled(bvm *vm);
extern int tasm_find_op(bvm *vm);
extern int tasm_apply_str_op(bvm *vm);

#include "solidify/solidified_Firmware_class.h"
#include "solidify/solidified_rule_matcher.h"
#include "solidify/solidified_trigger_class.h"

#include "be_fixed_be_class_Firmware.h"

/* @const_object_info_begin
class be_class_Firmware (scope: global, name: Firmware) {
    _fl, var                            // list of active fast-loop object (faster than drivers)
    _rules, var                         // list of active rules
    _timers, var                        // list of active timers
    _crons, var                         // list of active crons
    _ccmd, var                          // list of active Firmware commands implemented in Berry
    _drivers, var                       // list of active drivers
    wire1, var                          // Firmware I2C Wire1
    wire2, var                          // Firmware I2C Wire2
    cmd_res, var                        // store the command result, nil if disables, true if capture enabled, contains return value
    global, var                         // mapping to FirmwareGlobal
    settings, var                       // mapping to Firmware Settings
    wd, var                             // working dir
    _debug_present, var                 // is `import debug` present?

    _global_def, comptr(&be_Firmware_global_struct)
    _settings_def, comptr(&be_Firmware_settings_struct)
    _global_addr, comptr(&FirmwareGlobal)
    _settings_ptr, comptr(&Settings)

    init, closure(Firmware_init_closure)

    get_free_heap, func(l_getFreeHeap)
    arch, func(l_arch)
    publish, func(be_mqtt_publish)
    publish_result, func(l_publish_result)
    publish_rule, func(l_publish_rule)
    _cmd, func(l_cmd)
    get_option, func(l_getoption)
    millis, func(l_millis)
    time_reached, func(l_timereached)
    rtc, func(l_rtc)
    time_dump, func(l_time_dump)
    strftime, func(l_strftime)
    strptime, func(l_strptime)
    memory, func(l_memory)
    wifi, func(l_wifi)
    eth, func(l_eth)
    hostname, func(l_hostname)
    yield, func(l_yield)
    delay, func(l_delay)
    delay_microseconds, func(l_delay_microseconds)
    scale_uint, func(l_scaleuint)
    log, func(l_logInfo)
    save, func(l_save)
    locale, func(l_locale)

    read_sensors, func(l_read_sensors)

    resp_cmnd, func(l_respCmnd)
    resp_cmnd_str, func(l_respCmndStr)
    resp_cmnd_done, func(l_respCmndDone)
    resp_cmnd_error, func(l_respCmndError)
    resp_cmnd_failed, func(l_respCmndFailed)
    resolvecmnd, func(l_resolveCmnd)

    response_append, func(l_respAppend)
    web_send, func(l_webSend)
    web_send_decimal, func(l_webSendDecimal)

    get_power, func(l_getpower)
    set_power, func(l_setpower)
    get_switch, func(l_getswitch)     // deprecated
    get_switches, func(l_getswitch)

    i2c_enabled, func(l_i2cenabled)

    fast_loop, closure(Firmware_fast_loop_closure)
    add_fast_loop, closure(Firmware_add_fast_loop_closure)
    remove_fast_loop, closure(Firmware_remove_fast_loop_closure)
    cmd, closure(Firmware_cmd_closure)
    _find_op, func(tasm_find_op)        // new C version for finding a rule operator
    _apply_str_op, func(tasm_apply_str_op)
    find_key_i, closure(Firmware_find_key_i_closure)
    find_op, closure(Firmware_find_op_closure)
    add_rule, closure(Firmware_add_rule_closure)
    remove_rule, closure(Firmware_remove_rule_closure)
    try_rule, closure(Firmware_try_rule_closure)
    exec_rules, closure(Firmware_exec_rules_closure)
    exec_tele, closure(Firmware_exec_tele_closure)
    set_timer, closure(Firmware_set_timer_closure)
    run_deferred, closure(Firmware_run_deferred_closure)
    remove_timer, closure(Firmware_remove_timer_closure)
    add_cmd, closure(Firmware_add_cmd_closure)
    remove_cmd, closure(Firmware_remove_cmd_closure)
    exec_cmd, closure(Firmware_exec_cmd_closure)
    gc, closure(Firmware_gc_closure)
    event, closure(Firmware_event_closure)
    add_driver, closure(Firmware_add_driver_closure)
    remove_driver, closure(Firmware_remove_driver_closure)
    load, closure(Firmware_load_closure)
    wire_scan, closure(Firmware_wire_scan_closure)
    time_str, closure(Firmware_time_str_closure)
    urlfetch, closure(Firmware_urlfetch_closure)
    urlfetch_cmd, closure(Firmware_urlfetch_cmd_closure)

    add_cron, closure(Firmware_add_cron_closure)
    run_cron, closure(Firmware_run_cron_closure)
    next_cron, closure(Firmware_next_cron_closure)
    remove_cron, closure(Firmware_remove_cron_closure)

    check_not_method, closure(Firmware_check_not_method_closure)

    hs2rgb, closure(Firmware_hs2rgb_closure)

    gen_cb, closure(Firmware_gen_cb_closure)

    get_light, closure(Firmware_get_light_closure)
    set_light, closure(Firmware_set_light_closure)

    Rule_Matcher, class(be_class_Rule_Matcher)
}
@const_object_info_end */
