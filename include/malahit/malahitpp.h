/* -*- c++ -*- */
/*
 * Copyright 2020 dl1ksv.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MALAHIT_FCDPP_H
#define INCLUDED_MALAHIT_FCDPP_H

#include <gnuradio/hier_block2.h>
#include <malahit/api.h>

namespace gr {
namespace malahit {

/*!
 * \brief Malahit Pro+ Dongle source block.
 *
 * This class provides a soure block for the Malahit Pro+ Dongle by wrapping the
 * alsa audio interface and the USB control interface of the Malahit Dongle
 * into one convenient source block.
 * The hadware audio device is autodetected by the card name. If more than one Pro+ are
 * connected it takes the first card.
 * The block was tested with  firmware version 20.3
 *
 * \ingroup malahit
 *
 */
class MALAHIT_API malahitpp : virtual public gr::hier_block2
{
public:
    typedef std::shared_ptr<malahitpp> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of malahit::malahitpp.
     *
     * To avoid accidental use of raw pointers, malahit::malahitpp's
     * constructor is in a private implementation
     * class. malahit::malahitpp::make is the public interface for
     * creating new instances.
     */
    static sptr make(const std::string device_name = "", int unit = 1);

    /*! \brief Set frequency with resolution defined by unit.
     *  \param freq The frequency in unit Hz
     *
     * Sets the frequency of the Malahit Dongle with Hz or Khz resolution
     * depending on the unit paramater ( 1: Hz , 1000 Khz )
     * applying the frequency correction set by set_freq_corr().
     *
     */
    virtual void set_freq(float freq) = 0;

    /*! \brief Switches the LNA on/off.
     *  \param gain  lna on/off.
     *
     */
    virtual void set_lna(int gain) = 0;

    /*! \brief Switches mixer gain onf/off.
     *  \param gain true means on.
     *
     */
    virtual void set_mixer_gain(int gain) = 0;

    /*! \brief Set new frequency correction.
     *  \param ppm The new frequency correction in parts per million
     *
     * Malahit recommends 0 for the Pro+.
     *
     * Ref: http://www.malahitdongle.com/MyImages/FCD2ManualV4.pdf
     */
    virtual void set_freq_corr(int ppm) = 0;

    /*! \brief Set If gain.
     *  \param gain The If gain value between 0 and 59 db
     *
     */
    virtual void set_if_gain(int gain) = 0;
};

} // namespace malahit
} // namespace gr

#endif /* INCLUDED_MALAHIT_FCDPP_H */
