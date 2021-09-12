/*
Sampling frequency offset

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cmath>
#include "complex.hh"
#include "resampler.hh"
#include "wav.hh"

int main(int argc, char **argv)
{
	if (argc != 4) {
		std::cerr << "usage: " << argv[0] << " OUTPUT INPUT PPM" << std::endl;
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
	value ppm = std::atof(argv[3]);

	DSP::ReadWAV<value> inp_file(inp_name);

	if (inp_file.channels() < 1 || inp_file.channels() > 2) {
		std::cerr << "Only real or analytic signal (one or two channels) supported." << std::endl;
		return 1;
	}

	DSP::WriteWAV<value> out_file(out_name, inp_file.rate(), inp_file.bits(), inp_file.channels());

	DSP::Resampler2<value, cmplx, 129, 3> resample(inp_file.rate(), (inp_file.rate() * 19) / 40, 2);

	value sfo = (inp_file.rate() * ppm) / value(1000000);

	while (out_file.good() && inp_file.good()) {
		cmplx input;
		inp_file.read(reinterpret_cast<value *>(&input), 1);
		cmplx output[2];
		out_file.write(reinterpret_cast<value *>(output), resample(output, input, sfo), 2);
	}

	return 0;
}

