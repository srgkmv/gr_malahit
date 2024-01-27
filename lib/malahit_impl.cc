/* -*- c++ -*- */
/*
 * Copyright 2021 Sergej Komov.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/blocks/float_to_complex.h>
#include <gnuradio/io_signature.h>
#include <gnuradio/logger.h>

#include "malahit_impl.h"

#include <exception>
#include <fstream>
#include <iostream>

namespace gr {
namespace malahit {

malahit::sptr malahit::make(const std::string device_name, int unit)
{
    return gnuradio::get_initial_sptr(new malahit_impl(device_name, unit));
}

/*
 * The private constructor
 */
malahit_impl::malahit_impl(const std::string user_device_name, int unit)
    : gr::hier_block2("malahit",
                      gr::io_signature::make(0, 0, 0),
                      gr::io_signature::make(1, 1, sizeof(gr_complex))),
                      d_logger("malahit " + user_device_name)
{
    prefs* p = prefs::singleton();

    d_logger.set_level(gr::log_level::off);

    std::string device_name;
    bool success;
    gr::blocks::float_to_complex::sptr f2c;

    success = false;
    d_freq_req = 0;
    d_corr = 0;
    d_unit = unit;

    if (!user_device_name.empty()) {
        try {
            /* Audio source; sample rate fixed at 160kHz */
            malahit = gr::audio::source::make(160000, user_device_name, true);
            success = true;
        } catch (std::exception) {
            d_logger.info("Could not open device: {}", user_device_name);
            success = false;
        }
    }
    if (success) {
        device_name = user_device_name;
    } else {
        device_name.clear();
        std::string line;
        std::ifstream cards("/proc/asound/cards");
        if (cards.is_open()) {
            while (cards.good()) {
                getline(cards, line);

                if (line.find("USB-Audio - Malahit reciever") != std::string::npos) {
                    int id;
                    std::istringstream(line) >> id;

                    std::ostringstream hw_id;
                    hw_id << "hw:" << id << ",1"; // build alsa identifier
                    device_name = hw_id.str();
                }
            }
            cards.close();
            if (device_name.empty()) {
                throw std::runtime_error("No Malahit DSP found.");
            }
        } else {
            throw std::runtime_error("Alsa not found.");
        }
        /* Audio source; sample rate fixed at 160kHz */
        malahit = gr::audio::source::make(160000, device_name, true);
    }
    if (success) {
        d_logger.info("Audio device {} opened", device_name);
    } else {
        d_logger.info("Malahit DSP found as: {}", device_name);
    }

    /* block to convert stereo audio to a complex stream */
    f2c = gr::blocks::float_to_complex::make(1);

    connect(malahit, 0, f2c, 0);
    connect(malahit, 1, f2c, 1);
    connect(f2c, 0, self(), 0);

    malahit_control_block = malahit_control::make();

    message_port_register_hier_in(pmt::mp("freq"));
    msg_connect(self(), pmt::mp("freq"), malahit_control_block, pmt::mp("freq"));
}

/*
 * Our virtual destructor.
 */
malahit_impl::~malahit_impl() {}

void malahit_impl::set_freq(float freq)
{
    float setfreq;
    if (d_freq_req == (int)freq)
        return; // Frequency did not change
    d_freq_req = (int)freq;
    if (d_corr == 0) {
        setfreq = freq * d_unit;
    } else {
        setfreq = ((float)d_unit + ((float)d_corr) / (1000000. / d_unit)) * freq;
    }
    malahit_control_block->set_freq(setfreq);
}

void malahit_impl::set_freq_corr(int ppm)
{
    float freq;
    if (d_corr == ppm)
        return;
    d_corr = ppm;
    d_logger.info("Set frequency correction to: {} ppm ", ppm);
    freq = d_freq_req;
    d_freq_req = 0;
    set_freq(freq);
}

} /* namespace malahit */
} /* namespace gr */
