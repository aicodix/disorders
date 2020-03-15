
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

