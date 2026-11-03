# CLAUDE.md

Command-line tools that add channel impairments to WAV audio files, used to
simulate transmissions for the modems, such as
[modem](https://github.com/aicodix/modem). See README.md for usage.

## Layout

One program per impairment, each a single `main()` that reads a WAV file,
processes it sample by sample and writes a WAV file in the same format:

- `awgn.cc`: additive white Gaussian noise.
- `cfo.cc`: carrier frequency offset, by mixing with a `DSP::Phasor`.
- `sfo.cc`: sampling frequency offset in ppm, by resampling with `DSP::Resampler2`.
- `multipath.cc`: sum of delayed, attenuated and phase shifted copies, read
  from a table, plus a direct path set by a Rician factor in dB.
- `watterson.cc`: Watterson fading channel as in ITU-R F.1487, two Rayleigh
  fading paths with Gaussian Doppler spectrum, from sums of `DSP::Phasor`s.
  An optional seed makes the fading repeatable.
- `multipath.txt`: example table, one path per row: amplitude, delay in
  milliseconds, phase in radians. Generated with
  [rir](https://github.com/aicodix/rir).

All building blocks come from the sibling checkout
[dsp](https://github.com/aicodix/dsp) (namespace `DSP`), found by `-I../dsp`.
When a change belongs in that library, such as in `wav.hh`, say so instead of
working around it in every tool here.

## Building and testing

```
make
```

The default compiler is `clang++ -stdlib=libc++` with `-march=native`. If
libc++ is not installed, build with `make CXX=g++`. Run `make clean` to remove
the binaries.

Everything is compiled with `-std=c++11 -O3 -ffast-math -fno-exceptions -fno-rtti`,
so code must stay within C++11, not use exceptions or RTTI, and stay correct
under `-ffast-math`.

There is no test suite. `cfo`, `sfo` and `multipath` are deterministic, so
check a refactoring by comparing outputs of old and new binaries with `cmp`.
`awgn` and `watterson` seed from `std::random_device`, so their output differs
on every run, unless a seed is given as last argument.
A full check is the modem's simulation pipeline in its README, which chains
these tools between encoder and decoder.

## Behavior worth knowing

- Input and output name `-` means stdin or stdout, so the tools chain in pipes.
- One channel is a real signal, two channels are an analytic signal (I and Q).
  The output keeps the input's rate, bits and channels. Use 32-bit float for
  simulations: 8- and 16-bit output is quantized and clipped at full scale.
- `cfo` and `multipath` turn a real input into an analytic one with a 513-tap
  `DSP::Hilbert` and output only the real part. That delays the signal by
  257 samples, and the last 257 input samples never reach the output. Analytic
  input is processed directly.
- The `awgn` level is absolute, relative to digital full scale, not to the
  signal: each channel gets noise of variance `2 * 10^(dB/10)`. The SNR
  therefore depends on the input level, and the noise is white across the whole
  sample rate, so the in-band SNR of a narrowband signal is much higher than the
  wideband SNR.
- `good()` only turns false after a read has failed at the end of the input,
  so each loop checks it again after reading, before using the sample.

## Code style

- Every file starts with a short C comment block: a one-line description,
  a blank line, then `Copyright <year> Ahmet Inan <inan@aicodix.de>`.
- Indent with tabs and use K&R braces. Keep each tool a short, self-contained
  `main()` and avoid unnecessary abstraction.
- Usage errors print `usage: <program> OUTPUT INPUT ...` to stderr and return 1.

## Workflow

- Work on the `master` branch.
- Commit messages are a single short lowercase line, such as
  "replaced deprecated -Ofast with -O3 -ffast-math". There is no body and
  no trailers: no `Co-Authored-By`, no session links.
- Don't push. Deliver changes as a `git format-patch` patch for the
  maintainer to review and apply.
- When adding a tool, add it to `all` and `clean` in the Makefile and give it
  a section in README.md.
