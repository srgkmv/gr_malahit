/* -*- c++ -*- */
/*
 * Copyright 2021 Sergej Komov.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MALAHIT_IMPL_H
#define INCLUDED_MALAHIT_IMPL_H

#include <gnuradio/audio/source.h>
#include <gnuradio/logger.h>
#include <gnuradio/prefs.h>
#include <malahit/malahit.h>
#include <malahit/malahit_control.h>


namespace gr {
namespace malahit {

class malahit_impl : public malahit
{
private:
    gr::audio::source::sptr malahit;           /*!< The audio input source */
    malahit_control::sptr malahit_control_block; /*!< The malahit control block */
    unsigned int d_freq_req;               /*!< The latest requested frequency in Khz */
    int d_corr;
    int d_unit;
    gr::logger_ptr d_logger;

public:
    malahit_impl(const std::string device_name, int unit);
    ~malahit_impl();
    /* Public API functions documented in include/malahit/malahit.h */
    void set_freq(float freq);
    void set_lna(int gain);
    void set_mixer_gain(int gain);
    void set_freq_corr(int ppm);
    void set_if_gain(int gain);
};

} // namespace malahit
} // namespace gr

#endif /* INCLUDED_MALAHIT_IMPL_H */
