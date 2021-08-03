/* -*- c++ -*- */
/*
 * Copyright 2021 Sergej Komov.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MALAHIT_CONTROL_IMPL_H
#define INCLUDED_MALAHIT_CONTROL_IMPL_H

#include "hidapi/hidapi.h"
#include <malahit/malahit_control.h>

namespace gr {
namespace malahit {

class malahit_control_impl : public malahit_control
{
private:
    int serial_port; /*!< handle to control the device, set frequency, etc */

public:
    malahit_control_impl();
    ~malahit_control_impl();

    /* Public API functions documented in include/malahit/malahit_control.h */
    void set_freq(float freq);
    void set_frequency_msg(pmt::pmt_t msg);
};

} // namespace malahit
} // namespace gr

#endif /* INCLUDED_MALAHIT_CONTROL_IMPL_H */
