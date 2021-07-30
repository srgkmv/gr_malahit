/* -*- c++ -*- */
/*
 * Copyright 2020 dl1ksv.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MALAHIT_FCDPP_CONTROL_IMPL_H
#define INCLUDED_MALAHIT_FCDPP_CONTROL_IMPL_H

#include "hidapi/hidapi.h"
#include <malahit/malahitpp_control.h>

namespace gr {
namespace malahit {

class malahitpp_control_impl : public malahitpp_control
{
private:
    int serial_port; /*!< handle to control the device, set frequency, etc */
    unsigned char aucBuf[65]; /*!< Buffers to read/write control messages to the dongle */

public:
    malahitpp_control_impl();
    ~malahitpp_control_impl();

    /* Public API functions documented in include/malahit/malahitpp_control.h */
    void set_freq(float freq);
    void set_lna(int gain);
    void set_mixer_gain(int gain);
    void set_if_gain(int gain);
    void set_frequency_msg(pmt::pmt_t msg);
};

} // namespace malahit
} // namespace gr

#endif /* INCLUDED_MALAHIT_FCDPP_CONTROL_IMPL_H */
