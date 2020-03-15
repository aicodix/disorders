/*
Additive white Gaussian noise

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <cmath>
#include <random>
#include <functional>
#include "complex.hh"
#include "wav.hh"

int main(int argc, char **argv)
{
	if (argc != 4) {
		std::cerr << "usage: " << argv[0] << " OUTPUT INPUT AWGN" << std::endl;
		return 1;
	}

	typedef float value;
	typedef DSP::Complex<value> cmplx;

	const char *out_name = argv[1];
	const char *inp_name = argv[2];
	value awgn = std::atof(argv[3]);

	DSP::ReadWAV<value> inp_file(inp_name);

	if (inp_file.channels() < 1 || inp_file.channels() > 2) {
		std::cerr << "Only real or analytic signal (one or two channels) supported." << std::endl;
		return 1;
	}

	DSP::WriteWAV<value> out_file(out_name, inp_file.rate(), inp_file.bits(), inp_file.channels());

	value mean = 0;
	value sigma = std::sqrt(2 * std::pow(10, awgn / 10));
	std::random_device rd;
	typedef std::default_random_engine generator;
	typedef std::normal_distribution<value> normal;
	auto noise = std::bind(normal(mean, sigma), generator(rd()));

	while (out_file.good() && inp_file.good()) {
		cmplx tmp;
		inp_file.read(reinterpret_cast<value *>(&tmp), 1);
		tmp += cmplx(noise(), noise());
		out_file.write(reinterpret_cast<value *>(&tmp), 1);
	}

	return 0;
}

