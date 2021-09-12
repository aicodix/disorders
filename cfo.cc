/*
Carrier frequency offset

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cmath>
#include "complex.hh"
#include "hilbert.hh"
#include "phasor.hh"
#include "wav.hh"

int main(int argc, char **argv)
{
	if (argc != 4) {
		std::cerr << "usage: " << argv[0] << " OUTPUT INPUT CFO" << std::endl;
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
	value cfo = std::atof(argv[3]);

	DSP::ReadWAV<value> inp_file(inp_name);

	if (inp_file.channels() < 1 || inp_file.channels() > 2) {
		std::cerr << "Only real or analytic signal (one or two channels) supported." << std::endl;
		return 1;
	}

	DSP::WriteWAV<value> out_file(out_name, inp_file.rate(), inp_file.bits(), inp_file.channels());

	bool real = inp_file.channels() == 1;

	DSP::Phasor<cmplx> phasor;
	phasor.freq(cfo / inp_file.rate());

	DSP::Hilbert<cmplx, 513> hilbert;

	while (out_file.good() && inp_file.good()) {
		cmplx tmp;
		inp_file.read(reinterpret_cast<value *>(&tmp), 1);
		if (real)
			tmp = hilbert(tmp.real());
		tmp *= phasor();
		out_file.write(reinterpret_cast<value *>(&tmp), 1);
	}

	return 0;
}

