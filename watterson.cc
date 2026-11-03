/*
Watterson fading channel

Copyright 2026 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <random>
#include <cmath>
#include "complex.hh"
#include "hilbert.hh"
#include "phasor.hh"
#include "const.hh"
#include "wav.hh"

int main(int argc, char **argv)
{
	if (argc != 5 && argc != 6) {
		std::cerr << "usage: " << argv[0] << " OUTPUT INPUT DELAY SPREAD [SEED]" << std::endl;
		return 1;
	}

	typedef float value;
	typedef DSP::Complex<value> cmplx;

	const char *out_name = argv[1];
	if (out_name[0] == '-' && out_name[1] == 0)
		out_name = "/dev/stdout";
	const char *inp_name = argv[2];
	if (inp_name[0] == '-' && inp_name[1] == 0)
		inp_name = "/dev/stdin";
	value msec = std::atof(argv[3]);
	value spread = std::atof(argv[4]);
	if (msec < 0 || spread < 0) {
		std::cerr << "Delay and spread must not be negative." << std::endl;
		return 1;
	}

	DSP::ReadWAV<value> inp_file(inp_name);

	if (inp_file.channels() < 1 || inp_file.channels() > 2) {
		std::cerr << "Only real or analytic signal (one or two channels) supported." << std::endl;
		return 1;
	}

	DSP::WriteWAV<value> out_file(out_name, inp_file.rate(), inp_file.bits(), inp_file.channels());

	bool real = inp_file.channels() == 1;
	DSP::Hilbert<cmplx, 513> hilbert;

	std::random_device rd;
	std::default_random_engine generator(argc == 6 ? std::atoi(argv[5]) : rd());
	// Gaussian Doppler spectrum, frequency spread is two sigma
	std::normal_distribution<value> doppler(0, spread / 2);
	std::uniform_real_distribution<value> phase(0, DSP::Const<value>::TwoPi());

	const int paths = 2, phasors = 64;
	DSP::Phasor<cmplx> osc[paths][phasors];
	cmplx init[paths][phasors];
	for (int p = 0; p < paths; ++p) {
		for (int n = 0; n < phasors; ++n) {
			osc[p][n].freq(doppler(generator) / inp_file.rate());
			init[p][n] = DSP::polar<value>(1 / std::sqrt(value(paths * phasors)), phase(generator));
		}
	}

	int delay = std::nearbyint(inp_file.rate() * msec / 1000);
	cmplx *buf = new cmplx[delay+1];
	for (int i = 0; i <= delay; ++i)
		buf[i] = 0;

	while (out_file.good() && inp_file.good()) {
		cmplx input;
		inp_file.read(reinterpret_cast<value *>(&input), 1);
		if (!inp_file.good())
			break;
		if (real)
			input = hilbert(input.real());
		for (int i = delay; i; --i)
			buf[i] = buf[i-1];
		buf[0] = input;
		cmplx gain[paths];
		for (int p = 0; p < paths; ++p) {
			gain[p] = 0;
			for (int n = 0; n < phasors; ++n)
				gain[p] += init[p][n] * osc[p][n]();
		}
		cmplx sum = gain[0] * buf[0] + gain[1] * buf[delay];
		out_file.write(reinterpret_cast<value *>(&sum), 1);
	}

	delete[] buf;
	return 0;
}
