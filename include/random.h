/*
 * random.h
 *
 *  Created on: Mar 27, 2014
 *      Author: vlad
 */

#ifndef RANDOM_H_
#define RANDOM_H_

#include "types.h"
#include "stat.h"
#include "constants.h"
#include "exception.h"

namespace qpp
{

// Random double matrix with entries in Uniform[0,1]
inline types::dmat rand(size_t rows, size_t cols) throw (Exception)
{
	if (rows == 0 || cols == 0)
		throw Exception("rand", Exception::Type::MATRIX_ZERO_SIZE);

	return types::dmat::Random(rows, cols);
}

// Random double square matrix with entries in Uniform[0,1]
inline types::dmat rand(size_t rows) throw (Exception)
{
	if (rows == 0)
		throw Exception("rand", Exception::Type::MATRIX_ZERO_SIZE);

	return rand(rows, rows);
}

// Random double matrix with entries in Normal(0,1)
inline types::dmat randn(size_t rows, size_t cols) throw (Exception)
{
	if (rows == 0 || cols == 0)
		throw Exception("randn", Exception::Type::MATRIX_ZERO_SIZE);

	stat::NormalDistribution nd; // N(0,1)
	types::dmat A(rows, cols);

	for (size_t j = 0; j < cols; j++)
		for (size_t i = 0; i < rows; i++)
			A(i, j) = nd.sample();

	return A;
}

// Random double square matrix with entries in Normal(0,1)
inline types::dmat randn(size_t rows) throw (Exception)
{
	if (rows == 0)
		throw Exception("randn", Exception::Type::MATRIX_ZERO_SIZE);

	return randn(rows, rows);
}

// Random unitary matrix
inline types::cmat rand_unitary(size_t D) throw (Exception)
{
	if (D == 0)
		throw Exception("rand_unitary", Exception::Type::DIMS_HAVE_ZERO);

	types::cmat X(D, D);

	X.real() = 1. / std::sqrt(2.) * randn(D);
	X.imag() = 1. / std::sqrt(2.) * randn(D);
	Eigen::HouseholderQR<types::cmat> qr(X);

	types::cmat Q = qr.householderQ();
	// phase correction so that the resultant matrix is
	// uniformly distributed according to the Haar measure

	Eigen::VectorXcd phases = (rand(D, 1)).cast<types::cplx>();
	for (size_t i = 0; i < static_cast<size_t>(phases.rows()); i++)
		phases(i) = std::exp((types::cplx) (2 * ct::pi * ct::ii * phases(i)));

	Q = Q * phases.asDiagonal();

	return Q;
}

}

#endif /* RANDOM_H_ */
