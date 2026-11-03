
All tools keep the sample rate, bits and channels of their input.
Use WAV files with 32 bit float samples for simulations: with 8 or 16 bits, the output is quantized and clipped at full scale.

### [cfo.cc](cfo.cc)

Simulating a [Carrier frequency offset](https://en.wikipedia.org/wiki/Carrier_frequency_offset) by mixing the [Analytic signal](https://en.wikipedia.org/wiki/Analytic_signal) with an [Numerically controlled oscillator](https://en.wikipedia.org/wiki/Numerically_controlled_oscillator).

If the input is a real signal, the [Discrete Hilbert transform](https://en.wikipedia.org/wiki/Hilbert_transform#Discrete_Hilbert_transform) is used to get the analytic signal and only the real part is used as output after mixing.

Add an CFO of 23.5 Hz:
```
./cfo output.wav input.wav 23.5
```

### [sfo.cc](sfo.cc)

Simulating a Sampling frequency offset by [Resampling](https://en.wikipedia.org/wiki/Sample-rate_conversion) the sampled data.

Add an SFO of -15 ppm:
```
./sfo output.wav input.wav -15
```

### [awgn.cc](awgn.cc)

Simulating [Additive white Gaussian noise](https://en.wikipedia.org/wiki/Additive_white_Gaussian_noise]).

Add -34dB of AWGN:
```
./awgn output.wav input.wav -34
```

An optional last argument seeds the random generator, so the same noise can be repeated:
```
./awgn output.wav input.wav -34 1234
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

### [watterson.cc](watterson.cc)

Simulating an [Ionospheric](https://en.wikipedia.org/wiki/Ionosphere) [HF](https://en.wikipedia.org/wiki/High_frequency) channel with the Watterson model as in ITU-R F.1487: two independent paths of equal power, the second one delayed, each with [Rayleigh fading](https://en.wikipedia.org/wiki/Rayleigh_fading) and a [Gaussian](https://en.wikipedia.org/wiki/Gaussian_function) [Doppler spread](https://en.wikipedia.org/wiki/Doppler_spread).

If the input is a real signal, the [Discrete Hilbert transform](https://en.wikipedia.org/wiki/Hilbert_transform#Discrete_Hilbert_transform) is used to get the analytic signal and only the real part is used as output after fading.

Use a differential time delay of 1 millisecond and a frequency spread (two sigma) of 0.5 Hz:
```
./watterson output.wav input.wav 1 0.5
```

The mid-latitude conditions from ITU-R F.1487 and the flutter condition from CCIR 520:

| Condition | Delay | Spread |
| --------- | ----- | ------ |
| Quiet     | 0.5ms | 0.1Hz  |
| Moderate  |   1ms | 0.5Hz  |
| Disturbed |   2ms |   1Hz  |
| Flutter   | 0.5ms |  10Hz  |

The tap gains are sums of 64 phasors with random phases and Gaussian distributed frequencies.
An optional last argument seeds the random generator, so the same fading can be repeated:
```
./watterson output.wav input.wav 1 0.5 1234
```
