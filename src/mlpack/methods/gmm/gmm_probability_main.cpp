/**
 * @file gmm_probability_main.cpp
 * @author Ryan Curtin
 *
 * Given a GMM, calculate the probability of points coming from it.
 *
 * This file is part of mlpack 2.0.2.
 *
 * mlpack is free software; you may redistribute it and/or modify it under the
 * terms of the 3-clause BSD license.  You should have received a copy of the
 * 3-clause BSD license along with mlpack.  If not, see
 * http://www.opensource.org/licenses/BSD-3-Clause for more information.
 */
#include <mlpack/core.hpp>
#include "gmm.hpp"

using namespace std;
using namespace mlpack;
using namespace mlpack::gmm;

PROGRAM_INFO("GMM Probability Calculator",
    "This program calculates the probability that given points came from a "
    "given GMM (that is, P(X | gmm)).  The GMM is specified with the "
    "--input_model_file option, and the points are specified with the "
    "--input_file option.  The output probabilities are stored in the file "
    "specified by the --output_file option.");

PARAM_STRING_REQ("input_model_file", "File containing input GMM.", "m");
PARAM_STRING_REQ("input_file", "File containing points.", "i");

PARAM_STRING("output_file", "File to save calculated probabilities to.", "o",
    "");

int main(int argc, char** argv)
{
  CLI::ParseCommandLine(argc, argv);

  const string inputFile = CLI::GetParam<string>("input_file");
  const string inputModelFile = CLI::GetParam<string>("input_model_file");
  const string outputFile = CLI::GetParam<string>("output_file");

  if (CLI::HasParam("output_file"))
    Log::Warn << "--output_file (-o) is not specified;"
        << "no results will be saved!" << endl;

  // Get the GMM and the points.
  GMM gmm;
  data::Load(inputModelFile, "gmm", gmm);

  arma::mat dataset;
  data::Load(inputFile, dataset);

  // Now calculate the probabilities.
  arma::rowvec probabilities(dataset.n_cols);
  for (size_t i = 0; i < dataset.n_cols; ++i)
    probabilities[i] = gmm.Probability(dataset.unsafe_col(i));

  // And save the result.
  if (CLI::HasParam("output_file"))
    data::Save(outputFile, probabilities);
}
