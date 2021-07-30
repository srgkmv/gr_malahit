# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################

list(APPEND malahit_python_files
    malahitpp_control_python.cc
    malahitpp_python.cc python_bindings.cc)

GR_PYBIND_MAKE_OOT(malahit 
   ../..
   gr::malahit
   "${malahit_python_files}")

install(TARGETS malahit_python DESTINATION ${GR_PYTHON_DIR}/malahit COMPONENT pythonapi)
