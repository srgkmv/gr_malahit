/* -*- c++ -*- */
/*
 * Copyright 2021 Sergej Komov.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "malahit_control_impl.h"

#include <gnuradio/io_signature.h>
#include <gnuradio/logger.h>


// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()


#define TIMEOUT 5000

namespace gr {
namespace malahit {

malahit_control::sptr malahit_control::make()
{
    return gnuradio::get_initial_sptr(new malahit_control_impl());
}

/*
 * The private constructor
 */
malahit_control_impl::malahit_control_impl()
    : gr::block("malahit_control",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    /* setup the control part */
    serial_port = open("/dev/ttyACM0", O_RDWR);
    if (serial_port < 0) 
    {
        GR_LOG_ERROR(d_logger, "Malahit SDR not found.");
        throw std::runtime_error("Malahit SDR not found.");
    }
    else 
    {
        GR_LOG_INFO(d_logger, "Malahit SDR initialized.");
    }
    
    struct termios tty;
    if(tcgetattr(serial_port, &tty) != 0) 
    {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);
    
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) 
    {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }

    /*
     * Initialize message handling
     *
     * Replace boost::function with std::function
     *
     */
    message_port_register_in(pmt::mp("freq"));
    /*
     * set_msg_handler(
     *   pmt::mp("freq"),
     *   boost::bind(&malahit_control_impl::set_frequency_msg, this, _1));
     */

    set_msg_handler(pmt::mp("freq"), [this](pmt::pmt_t msg) {
        this->malahit_control_impl::set_frequency_msg(msg);
    });
}

/*
 * Our virtual destructor.
 */
malahit_control_impl::~malahit_control_impl()
{
    if (serial_port >= 0) {
        close(serial_port);
    }
}
void malahit_control_impl::set_freq(float freq)
{
    unsigned int nfreq = freq;
    char buff[128];
    sprintf(buff,"FA%011i;",nfreq);
    write(serial_port, buff, strlen(buff));
}
void malahit_control_impl::set_frequency_msg(pmt::pmt_t msg)
{
    // Accepts either a number that is assumed to be the new
    // frequency or a key:value pair message where the key must be
    // "freq" and the value is the new frequency.
    GR_LOG_DEBUG(d_logger, "Malahit Control frequency message arrived");    
    if (pmt::is_number(msg)) {
        set_freq(pmt::to_float(msg));
    } else if (pmt::is_pair(msg)) {
        pmt::pmt_t key = pmt::car(msg);
        pmt::pmt_t val = pmt::cdr(msg);
        if (pmt::eq(key, pmt::intern("freq"))) {
            if (pmt::is_number(val)) {
                set_freq(pmt::to_float(val));
            }
        } else {
            GR_LOG_WARN(
                d_logger,
                boost::format(
                    "Set Frequency Message must have the key = 'freq'; got '%1%'.") %
                    pmt::write_string(key));
        }
    } else {
        GR_LOG_WARN(d_logger,
                    "Set Frequency Message must be either a number or a "
                    "key:value pair where the key is 'freq'.");
    }
}
void malahit_control_impl::set_lna(int gain)
{
    
}
void malahit_control_impl::set_mixer_gain(int gain)
{
    
}

void malahit_control_impl::set_if_gain(int gain)
{
    
}

} /* namespace malahit */
} /* namespace gr */
