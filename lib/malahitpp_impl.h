/* -*- c++ -*- */
/*
 * Copyright 2020 dl1ksv.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MALAHIT_FCDPP_IMPL_H
#define INCLUDED_MALAHIT_FCDPP_IMPL_H

#include <gnuradio/audio/source.h>
#include <gnuradio/logger.h>
#include <gnuradio/prefs.h>
#include <malahit/malahitpp.h>
#include <malahit/malahitpp_control.h>


namespace gr {
namespace malahit {

class malahitpp_impl : public malahitpp
{
private:
    gr::audio::source::sptr malahit;           /*!< The audio input source */
    malahitpp_control::sptr malahit_control_block; /*!< The malahit control block */
    unsigned int d_freq_req;               /*!< The latest requested frequency in Khz */
    int d_corr;
    int d_unit;
    gr::logger_ptr d_logger;

public:
    malahitpp_impl(const std::string device_name, int unit);
    ~malahitpp_impl();
    /* Public API functions documented in include/malahit/malahitproplus.h */
    void set_freq(float freq);
    void set_lna(int gain);
    void set_mixer_gain(int gain);
    void set_freq_corr(int ppm);
    void set_if_gain(int gain);
};

} // namespace malahit
} // namespace gr

#endif /* INCLUDED_MALAHIT_FCDPP_IMPL_H */
