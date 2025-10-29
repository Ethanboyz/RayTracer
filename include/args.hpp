#ifndef ARGS_H
#define ARGS_H

#include <arpa/inet.h>
#include <argp.h>
#include <cstring>
#include <iostream>
#include <random>
#include <string>

struct run_arguments {
    uint64_t seed;              // Random number generator seed, affects noise function for terrain
    int spp;                    // Parent rays per pixel
};

inline error_t arg_parser(int key, char *arg, argp_state *state);

/**
 * @brief CLI argument parsing.
 *
 * Defines all the available options and calls arg_parse() to actually parse argc.
 * @param argc Number of CLI arguments.
 * @param argv Values of the CLI arguments.
 * @return the resolved CLI values.
 */
inline run_arguments arg_parseopt(const int argc, char *argv[]) {
    argp_option options[] = {
        { "seed", 's', "seed", 0, "Seed for terrain generation, can be any non-negative integer up to 18446744073709551615. Default: random seed", 0},
        { "spp", 'n', "samples", 0, "Samples (number of parent/camera rays) per pixel. Increase for less noise. Default: 10", 0}
    };

    const argp argp_settings = {
        options,
        arg_parser,
        nullptr,
        nullptr,
        nullptr,
        nullptr,
        nullptr
    };

    // Defaults
    run_arguments args = {};
    std::random_device rd;
    args.seed = rd();
    args.spp = 10;

    if (argp_parse(&argp_settings, argc, argv, 0, nullptr, &args) != 0) {
        std::cerr << "Error while parsing" << std::endl;
        exit(1);
    }

    return args;
}

/**
 * @brief Parses the CLI arguments.
 * @param key CLI arg key.
 * @param arg CLI arg value.
 * @param state State of arg parsing.
 * @return error_t If an error occurs while parsing.
 */
inline error_t arg_parser(const int key, char *arg, argp_state *state) {
	auto *args = static_cast<run_arguments*>(state->input);
	error_t ret = 0;

	switch (key) {
    case 's': {
		args->seed = std::stoull(arg);
		break;
	}
	case 'n': {
		args->spp = std::stoi(arg);
		if (args->spp < 0) {
			argp_error(state, "Invalid spp, must be a positive number");
		}
		break;
	}
	default:
		ret = ARGP_ERR_UNKNOWN;
		break;
	}
	return ret;
}


#endif