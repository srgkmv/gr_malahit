/* -*- c++ -*- */
/*
 * Copyright 2020 dl1ksv.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "malahitcmd.h"
#include "malahitpp_control_impl.h"

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

malahitpp_control::sptr malahitpp_control::make()
{
    return gnuradio::get_initial_sptr(new malahitpp_control_impl());
}

/*
 * The private constructor
 */
malahitpp_control_impl::malahitpp_control_impl()
    : gr::block("malahitpp_control",
                gr::io_signature::make(0, 0, 0),
                gr::io_signature::make(0, 0, 0))
{
    /* setup the control part */
    serial_port = open("/dev/ttyACM0", O_RDWR);
    if (serial_port < 0) 
    {
        GR_LOG_ERROR(d_logger, "Malachite SDR not found.");
        throw std::runtime_error("Malachite SDR not found.");
    }
    else 
    {
        GR_LOG_INFO(d_logger, "Malachite SDR initialized.");
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
     *   boost::bind(&malahitpp_control_impl::set_frequency_msg, this, _1));
     */

    set_msg_handler(pmt::mp("freq"), [this](pmt::pmt_t msg) {
        this->malahitpp_control_impl::set_frequency_msg(msg);
    });
}

/*
 * Our virtual destructor.
 */
malahitpp_control_impl::~malahitpp_control_impl()
{
    if (serial_port >= 0) {
        close(serial_port);
    }
}
void malahitpp_control_impl::set_freq(float freq)
{
    unsigned int nfreq = freq;
    char buff[128];
    sprintf(buff,"FA%011i;",nfreq);
    write(serial_port, buff, strlen(buff));
    /*aucBuf[0] = 0;
    aucBuf[1] = FCD_HID_CMD_SET_FREQUENCY_HZ;
    aucBuf[2] = (unsigned char)nfreq;
    aucBuf[3] = (unsigned char)(nfreq >> 8);
    aucBuf[4] = (unsigned char)(nfreq >> 16);
    aucBuf[5] = (unsigned char)(nfreq >> 24);
    hid_write(d_control_handle, aucBuf, 65);
    aucBuf[1] = 0;
    hid_read(d_control_handle, aucBuf, 65);
    if (aucBuf[0] == FCD_HID_CMD_SET_FREQUENCY_HZ && aucBuf[1] == 1) {
        nfreq = 0;
        nfreq = (unsigned int)aucBuf[2];
        nfreq += (unsigned int)(aucBuf[3] << 8);
        nfreq += (unsigned int)(aucBuf[4] << 16);
        nfreq += (unsigned int)(aucBuf[5] << 24);
        GR_LOG_INFO(d_logger, boost::format("Set Frequency to: %1% Hz") % nfreq);

    } else {
        GR_LOG_INFO(d_logger, boost::format("Set Frequency to %1% Hz failed") % nfreq);
    }*/
}
void malahitpp_control_impl::set_frequency_msg(pmt::pmt_t msg)
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
void malahitpp_control_impl::set_lna(int gain)
{
    /*aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd
                   // for one report
    aucBuf[1] = FCD_HID_CMD_SET_LNA_GAIN;
    if (gain != 0) {
        aucBuf[2] = 1;
    } else {
        aucBuf[2] = 0;
    }
    hid_write(d_control_handle, aucBuf, 65);
    hid_read(d_control_handle, aucBuf, 65);
    if (aucBuf[0] == FCD_HID_CMD_SET_LNA_GAIN) {
        if (gain != 0) {
            GR_LOG_INFO(d_logger, "LNA gain enabled");
        } else {
            GR_LOG_INFO(d_logger, "LNA gain disabled");
        }
    } else {
        GR_LOG_ERROR(
            d_logger,
            boost::format("Failed to modify LNA gain. Result of transaction: %1%,%2%") %
                aucBuf[0] % aucBuf[1]);
    }*/
}
void malahitpp_control_impl::set_mixer_gain(int gain)
{
    /*aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd
                   // for one report
    aucBuf[1] = FCD_HID_CMD_SET_MIXER_GAIN;
    if (gain != 0) {
        aucBuf[2] = 1;
    } else {
        aucBuf[2] = 0;
    }
    hid_write(d_control_handle, aucBuf, 65);
    hid_read(d_control_handle, aucBuf, 65);
    if (aucBuf[0] == FCD_HID_CMD_SET_MIXER_GAIN) {
        if (gain != 0) {
            GR_LOG_INFO(d_logger, "Mixer gain enabled");
        } else {
            GR_LOG_INFO(d_logger, "Mixer gain disabled");
        }
    } else {
        GR_LOG_ERROR(
            d_logger,
            boost::format("Failed to modify Mixer gain. Result of transaction: %1%,%2%") %
                aucBuf[0] % aucBuf[1]);
    }*/
}

void malahitpp_control_impl::set_if_gain(int gain)
{
    /*if ((gain < 0) || gain > 59) {
        GR_LOG_ERROR(d_logger, boost::format("Invalid IF gain value: %1%") % gain);
        return;
    }
    aucBuf[0] = 0; // Report ID. Ignored by HID Class firmware as only config'd
                   // for one report
    aucBuf[1] = FCD_HID_CMD_SET_IF_GAIN;
    aucBuf[2] = (unsigned char)gain;
    hid_write(d_control_handle, aucBuf, 65);
    hid_read(d_control_handle, aucBuf, 65);
    if (aucBuf[0] == FCD_HID_CMD_SET_IF_GAIN) {
        GR_LOG_INFO(d_logger, boost::format("IF gain set to: %1%") % gain);
    } else {
        GR_LOG_ERROR(d_logger, "Could not set IF gain");
    }*/
}

} /* namespace malahit */
} /* namespace gr */
