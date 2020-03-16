
### [cfo.cc](cfo.cc)

Simulating a [Carrier frequency offset](https://en.wikipedia.org/wiki/Carrier_frequency_offset) by mixing the [Analytic signal](https://en.wikipedia.org/wiki/Analytic_signal) with an [Numerically controlled oscillator](https://en.wikipedia.org/wiki/Numerically_controlled_oscillator).

If the input is a real signal, the [Discrete Hilbert transform](https://en.wikipedia.org/wiki/Hilbert_transform#Discrete_Hilbert_transform) is used to get the analytic signal and only the real part is used as output after mixing.

Add an CFO of 23.5 Hz:
```
./cfo output.wav input.wav 23.5
```

### [sfo.cc](sfo.cc)

Simulating a Sampling frequency offset by [Resampling](https://en.wikipedia.org/wiki/Sample-rate_conversion) the sampled data.

Add an SFO of -0.15 Hz:
```
./sfo output.wav input.wav -0.15
```

### [awgn.cc](awgn.cc)

Simulating [Additive white Gaussian noise](https://en.wikipedia.org/wiki/Additive_white_Gaussian_noise]).

Add -34dB of AWGN:
```
./awgn output.wav input.wav -34
```

### [multipath.cc](multipath.cc)

Simulating [Multipath propagation](https://en.wikipedia.org/wiki/Multipath_propagation) by summing the [Analytic signal](https://en.wikipedia.org/wiki/Analytic_signal) with delayed, attenuated and phase shifted versions of itself.

If the input is a real signal, the [Discrete Hilbert transform](https://en.wikipedia.org/wiki/Hilbert_transform#Discrete_Hilbert_transform) is used to get the analytic signal and only the real part is used as output after summing.

Read paths from table [multipath.txt](multipath.txt) and use a [Rician factor](https://en.wikipedia.org/wiki/Rician_fading) of 10dB:
```
./multipath output.wav input.wav multipath.txt 10
```

The table contains rows of each extra path and its amplitude, time delay in milliseconds and phase shift in radians.

Example table entries generated using [Room Impulse Response](https://github.com/aicodix/rir) tool.

