/*
Multipath propagation

Copyright 2020 Ahmet Inan <inan@aicodix.de>
*/

#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <vector>
#include "complex.hh"
#include "decibel.hh"
#include "hilbert.hh"
#include "phasor.hh"
#include "wav.hh"

int main(int argc, char **argv)
{
	if (argc != 5) {
		std::cerr << "usage: " << argv[0] << " OUTPUT INPUT TABLE FACTOR" << std::endl;
		return 1;
	}

	typedef float value;
	typedef DSP::Complex<value> cmplx;

	const char *out_name = argv[1];
	const char *inp_name = argv[2];
	const char *table_name = argv[3];
	value factor = DSP::idecibel(std::atof(argv[4]));

	DSP::ReadWAV<value> inp_file(inp_name);

	if (inp_file.channels() < 1 || inp_file.channels() > 2) {
		std::cerr << "Only real or analytic signal (one or two channels) supported." << std::endl;
		return 1;
	}

	DSP::WriteWAV<value> out_file(out_name, inp_file.rate(), inp_file.bits(), inp_file.channels());

	bool real = inp_file.channels() == 1;
	DSP::Hilbert<cmplx, 513> hilbert;

	typedef struct {
		cmplx camp;
		int delay;
	} Path;
	std::vector<Path> paths;

	std::ifstream table_file(table_name);

	std::string line;
	int dmax = 0;
	value power = 0;
	while (getline(table_file, line)) {
		std::istringstream iss(line);
		value ampl, msec, rad;
		iss >> ampl >> msec >> rad;
		cmplx camp = DSP::polar(ampl, rad);
		power += norm(camp);
		int delay = nearbyint(inp_file.rate() * msec / 1000);
		dmax = std::max(dmax, delay);
		paths.emplace_back(Path{camp, delay});
	}
	value ampl0 = sqrt(factor * power);
	power += ampl0 * ampl0;

	ampl0 /= sqrt(power);
	for (auto &path: paths)
		path.camp /= sqrt(power);

	cmplx *buf = new cmplx[dmax+1];

	while (out_file.good() && inp_file.good()) {
		cmplx input;
		inp_file.read(reinterpret_cast<value *>(&input), 1);
		if (real)
			input = hilbert(input.real());
		for (int i = dmax; i; --i)
			buf[i] = buf[i-1];
		buf[0] = input;
		cmplx sum = ampl0 * buf[0];
		for (const auto &path: paths)
			sum += path.camp * buf[path.delay];
		out_file.write(reinterpret_cast<value *>(&sum), 1);
	}

	return 0;
}

