#include "be_constobj.h"

static be_define_const_map_slots(m_libenergy_map) {
    { be_const_key(init, -1), be_const_closure(energy_init_closure) },
    { be_const_key(_ptr, 3), be_const_comptr(&Energy) },
    { be_const_key(setmember, -1), be_const_closure(energy_setmember_closure) },
    { be_const_key(member, 2), be_const_closure(energy_member_closure) },
    { be_const_key(read, -1), be_const_closure(energy_read_closure) },
};

static be_define_const_map(
    m_libenergy_map,
    5
);

static be_define_const_module(
    m_libenergy,
    "energy"
);

BE_EXPORT_VARIABLE be_define_const_native_module(energy);
