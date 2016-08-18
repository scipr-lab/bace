/** @file
 *****************************************************************************
 Implementation of functions for profiler.
 *****************************************************************************
 * @author     This file is part of bace, developed by SCIPR Lab
 *             and contributors (see AUTHORS).
 * @copyright  MIT license (see LICENSE file)
 *****************************************************************************/

#include <ctime>
#include <iostream>
#include <fstream>
#include <omp.h>
#include <sstream>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <sys/resource.h>
#include <unistd.h>
#include <vector>

#include "algebra/curves/alt_bn128/alt_bn128_pp.hpp"
#include "common/double.hpp"

#include "src/proof_system/prover.hpp"
#include "src/proof_system/verifier.hpp"
#include "src/proof_system/naive_evaluation.hpp"

using namespace bace;

void plot(const std::string &path)
{
    /* System Call */
    std::string cmd = "gnuplot -e \"input_directory=\'" + path + "\'\" src/profiling/plot/runtime_plot.gp";
    if (system(cmd.c_str()) == 0) printf("Plotted profile to %s\n", path.c_str());
}

template <typename FieldT>
std::pair<std::pair<double, double>, std::pair<size_t, size_t> > profile(const std::vector<std::vector<FieldT> > &input_batch,
                                                                         const size_t &input_size,
                                                                         const std::string &type)
{
    double start;
    double end;

    double start_prover;
    double end_prover;

    proof_t<FieldT> proof;
    std::vector<FieldT> output_batch;

    arithmetic_circuit_t<FieldT> circuit = arithmetic_circuit_t<FieldT>(input_size);
    circuit.add_quadratic_inner_product_gates();

    /* Perform operation */
    if (type == "naive")
    {
        start = omp_get_wtime();
        naive_evaluate(circuit, input_batch, output_batch);
        end = omp_get_wtime() - start;
    }
    else
    {
        start_prover = omp_get_wtime();
        prover(circuit, input_batch, proof);
        end_prover = omp_get_wtime() - start_prover;

        start = omp_get_wtime();
        verifier(circuit, input_batch, output_batch, proof);
        end = omp_get_wtime() - start;
    }

    return std::make_pair(std::make_pair(end, end_prover), std::make_pair(circuit.degree(), circuit.size()));
}

template <typename FieldT>
void initialize(const std::string &path, const std::string &type, const int &num_threads)
{
    size_t batch_size;
    size_t input_size;

    double runtime;
    double runtime_prover;
    size_t degree;
    size_t circuit_size;

    std::pair<std::pair<double, double>, std::pair<size_t, size_t> > profile_stats;

    /* Runtime File */
    std::ofstream runtime_file;
    runtime_file.open(path + type + "-" + std::to_string(num_threads) + "-thread.csv");
    runtime_file << "batch_size, input_size, circuit_size, degree, time (in sec)\n";

    std::ofstream prover_runtime_file;
    if (type == "verifier")
    {
        prover_runtime_file.open(path + "prover-" + std::to_string(num_threads) + "-thread.csv");
        prover_runtime_file << "batch_size, input_size, circuit_size, degree, time (in sec)\n";
    }

    printf("\n%s-%d-thread\n", type.c_str(), num_threads);

    /* Assess on varying domain sizes */
    std::vector<std::vector<FieldT> > input_batch;
    for (int i = 1; i < 8; i++) /* batch_size */
    {
        for (int j = 1; j < 13; j++) /* input_size */
        {
            /* Initialization */
            batch_size = pow(2, i);
            input_size = pow(2, j);

            input_batch.resize(batch_size);
            for (int k = 0; k < batch_size; k++)
            {
                input_batch[k] = std::vector<FieldT>(input_size);
                for (int l = 0; l < input_size; l++)
                {
                    input_batch[k][l] = FieldT::random_element();
                }
            }
            
            /* Profiling */
            profile_stats = profile<FieldT>(input_batch, input_size, type);
            runtime = profile_stats.first.first;
            runtime_prover = profile_stats.first.second;
            degree = profile_stats.second.first;
            circuit_size = profile_stats.second.second;

            /* Logging */
            runtime_file << batch_size << "," << input_size << "," << circuit_size << "," << degree << "," << runtime << "\n";
            if (type == "verifier")
            {
                prover_runtime_file << batch_size << "," << input_size << "," << circuit_size << "," << degree << "," << runtime_prover << "\n";
                prover_runtime_file.flush();
            }
            printf("batch_size %ld, input_size %ld, circuit_size %ld, degree %ld, %f seconds\n", batch_size, input_size, circuit_size, degree, runtime);
            
            runtime_file.flush();
            input_batch.clear();
        }

        runtime_file << ",,,,\n";
        runtime_file.flush();
        if (type == "verifier")
        {
            prover_runtime_file << ",,,,\n";
            prover_runtime_file.flush(); 
        }
        printf("\n");
    }

    /* Close file */
    runtime_file.close();
    if (type == "verifier") prover_runtime_file.close();
}

int main()
{
  /* Get Current Timestamp */
  time_t rawtime;
  time(&rawtime);
  struct tm* timeinfo = localtime(&rawtime);
  char buffer[40];
  strftime(buffer, 40, "%m-%d_%I:%M", timeinfo);
  std::string datetime(buffer);

  /* Make log file directory */
  std::string path = "src/profiling/logs/" + datetime + "/";
  if (system( ("mkdir -p " + path).c_str() )) return 0;

  /* Profile on 1, 2, 4, or 8 threads */
  const int max_threads = omp_get_max_threads();
  for (int num_threads = 1; num_threads <= max_threads; num_threads *= 2)
  {
    /* Fix number of threads, no dynamic adjustment */
    omp_set_dynamic(0);
    omp_set_num_threads(num_threads);

#ifndef PROF_DOUBLE
    printf("Profiling with alt_bn128_pp\n");
    libff::alt_bn128_pp::init_public_params();
    initialize<libff::Fr<libff::alt_bn128_pp> >(path, "naive", num_threads);
    initialize<libff::Fr<libff::alt_bn128_pp> >(path, "verifier", num_threads);
#else
    printf("Profiling with Double\n");
    initialize<libff::Double>(path, "naive", num_threads);
    initialize<libff::Double>(path, "verifier", num_threads);
#endif
  }

  plot(path);
  
  return 0;
}
