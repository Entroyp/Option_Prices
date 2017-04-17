#include "stdafx.h"
#include<vector>
#include "nr3.h"
#include "ludcmp.h"
#include "LSRegression.h"
#include <cmath>

LSRegression::LSRegression(){}

std::vector< double > LSRegression::OLS( 
	                           const std::vector< std::vector<double> >& X, /* n*p */
		                       const std::vector< double >& y ) const /*n*1*/
{

	MatDoub Input( X.size(), X[0].size() );
	MatDoub b( y.size(), 1 );

	for( size_t i = 0; i < X.size(); i++ )
	{	
		b[ i ][0] = y[ i ];
		for( size_t j = 0; j < X[0].size(); j++ )
		Input[ i ][ j ] = X[ i ][ j ];
	};

	MatDoub X2 = Input.transpose()*Input;

	MatDoub XY = Input.transpose()*b;

	LUdcmp LU( X2 );

	MatDoub X2_inv( X2 );

	LU.inverse( X2_inv );

	MatDoub sol = X2_inv*XY;

	std::vector<double> output;

	for( int i = 0; i < sol.nrows(); i++ )
		output.push_back( sol[ i ][0] );

	return( output );
};

MC_Price LSRegression_GBM::Regression() const
{
	int M = path.size();
	int N = path[0].size();

	double discount = exp( -r*dt );

	std::vector< std::vector< double > > cashflow_matrix( M, std::vector< double >( N, 0.) );

	for( int i = 0; i < M; i++ )
		cashflow_matrix[ i ][ N-1 ] = (*pay_off )( path[ i ][ N-1 ] );
	
	std::vector< int > optimal_stopping( M, N-1 );

	for( int j = N-2; j >=0; j-- )
	{
		std::vector< double > y_sel;
		std::vector< double > x_sel;
		std::vector< double > exercise;
		std::vector< int > mask_indices;

		for( int i = 0; i < M; i++ )
		{
			double payoff = (*pay_off )( path[ i ][ j ] );

			if( payoff <= 0 )
				continue;

			mask_indices.push_back( i );
			x_sel.push_back( path[ i ][ j ]*1./spot ); //normalization
			y_sel.push_back( cashflow_matrix[ i ][ optimal_stopping[ i ] ]
			*pow( discount, optimal_stopping[ i ] - j ) );
			exercise.push_back( payoff );
		};

		if( !x_sel.size() ) continue;

		std::vector< std::vector< double > > X;
		for( size_t i = 0; i < x_sel.size(); i++ )
			X.push_back( ( *basis )( x_sel[ i ] ) );

		std::vector< double > beta = OLS( X, y_sel );

		for( size_t i = 0; i < x_sel.size(); i++ )
		{
			double cont = 0;
			for( size_t inx = 0; inx < beta.size(); inx ++ )
				cont += X[ i ][ inx ]*beta[ inx ];

			if( cont > exercise[ i ] ) continue;

			cashflow_matrix[ mask_indices[ i ] ] [ j ] = exercise[ i ];
			cashflow_matrix[ mask_indices[ i ] ] [ optimal_stopping[ mask_indices[ i ] ] ] = exercise[ i ];
			optimal_stopping[ mask_indices[ i ] ] = j;
		};
	};

	double discounted_cashflow = 0.;
	double discounted_cashflow_Squared = 0.;

	for( int i = 0; i < M; i++ )
	{
		double tmp = cashflow_matrix[ i ] [ optimal_stopping[ i ] ]*pow( discount,
		optimal_stopping[ i ] + 1 );
		
		discounted_cashflow += tmp;
		discounted_cashflow_Squared += tmp*tmp;
	};

	double mean = discounted_cashflow*(1./M);
	double sig2 = discounted_cashflow_Squared*(1./M ) - mean*mean;
	sig2/=double(M);

	MC_Price res( mean, std::sqrt( sig2 ) );
	return res;

};
