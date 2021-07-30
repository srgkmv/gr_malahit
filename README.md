gr-malahit is an linux oot-module for gnuradio to implement a malahit sdr source.  
On linux it autodetects the correct soundcard from /proc/asound/cards.  
This idea was taken from the osmosdr drivers.

To control the device the usb serial is used.


1.   Dependencies:  
    &nbsp;   
gnuradio (>= 3.9 ) with pybind  
alsa usb drivers activated  

2.   Installation:  
    &nbsp;  
The build process is cmake based.  
So get the latest code from https://github.com/mihaly4/gr-malahit  
and change to the code directory. There   
&nbsp;  
$mkdir build  
$cd build  
$cmake -DCMAKE_INSTALL_PREFIX= "path to gnuradio installation" ../  
$make  
$su  
$make install  
&nbsp;      
After that executing grc  you'll find in the section Malahit   
&nbsp;  
Malahit SDR  
Malahit SDR control  

3.    Usage  
    &nbsp;  
Two modules are available:  
   - malahit_control  
   - malahit  
&nbsp;  
    The control module only controls the sdr, while the other modules control the sdr and provide the IQ samples.
All modules accept messages to control the frequency. That was the reason to introduce the control modules.
&nbsp;  
    The malahit module was introduced, as gnuradio 3.8 does not contain gr-malahit at the moment.  


5.    Credits  
    &nbsp;  
Some code is taken from gr-fcdproplus by Volker Schroer.  
