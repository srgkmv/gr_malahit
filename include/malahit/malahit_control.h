/* -*- c++ -*- */
/*
 * Copyright 2021 Sergej Komov.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_MALAHIT_CONTROL_H
#define INCLUDED_MALAHIT_CONTROL_H

#include <gnuradio/block.h>
#include <malahit/api.h>

namespace gr {
namespace malahit {

/*!
 * \brief <+description of block+>
 * \ingroup malahit
 *
 */
class MALAHIT_API malahit_control : virtual public gr::block
{
public:
    typedef std::shared_ptr<malahit_control> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of malahit::malahit_control.
     *
     * To avoid accidental use of raw pointers, malahit::malahit_control's
     * constructor is in a private implementation
     * class. malahit::malahit_control::make is the public interface for
     * creating new instances.
     */
    static sptr make();
    /*! \brief Sets the frequency .
     *  \param freq The frequency in unit Hz
     *
     * Sets the frequency of the Malahit DSP with Hz resolution
     *
     */
    virtual void set_freq(float freq) = 0;
};

} // namespace malahit
} // namespace gr

#endif /* INCLUDED_MALAHIT_CONTROL_H */
