#include "stdafx.h"
#include "PathGen.h"
#include "deviates.h"
#include <vector>
#include <cmath>
#include <iostream>

void PathGen_GBM::PathGenerator( std::vector< std::vector<double>>& stock_path ) const {

  stock_path.resize( M );
  int N            = int( T*(1./dt) );  
  double drift     = r*dt - sigma*sigma*0.5*dt;
  double diffusion = sigma*sqrt(dt);

  Normaldev_BM normal_dev( 0.0, 1., seed );

  for( int i = 0; i < M; i++ )
  {
    stock_path[ i ].push_back( spot*exp( drift + diffusion*normal_dev.dev() ) );
     for( int j = 1; j < N; j++ )
      stock_path[ i ].push_back( stock_path[ i ][ j-1 ]*exp( drift + diffusion*normal_dev.dev() ) );
  };
};


void PathGen_StoVol::PathGenerator( std::vector< std::vector< double > >& stock_path, 
                                    std::vector< std::vector< double > >& vol_path ) const{


  stock_path.resize( M );
  vol_path.resize( M );
  Normaldev_BM normal_dev( 0.0, 1., seed );

  int N                   = int( T*( 1./dt ) );
  double stock_innovation = 0;
  double vol_innovation   = 0;
  double stock_drift      = 0;
  double vol_drift        = 0;
  double stock_diffusion  = 0;
  double vol_diffusion    = 0;
  double rho_2            = sqrt( 1. - rho*rho );

  for( int i = 0; i < M; i++ )
  {
       
       if( 2.*kappa*theta <= volofvol*volofvol )
       throw( "Parameters constrains for stochastic vol do not hold." );
     
       stock_innovation = normal_dev.dev();
       vol_innovation   = stock_innovation*rho + normal_dev.dev()*rho_2;

       stock_drift      = ( r - 0.5*v0 )*dt;
       stock_diffusion  = sqrt( dt*v0 ); //sqrt(Vt)
       
       vol_drift        = kappa*( theta - v0 )*dt;
       vol_diffusion    = volofvol*stock_diffusion; //sigma*sqrt(Vt)

        /* The stochastic vol should be simulated from some other distribution, instead of using Gaussian discretization.*/ 
        /* So here, impose some flooring on the simulated stochastic vol. */
       stock_path[ i ].push_back( spot*exp( stock_drift + stock_diffusion*stock_innovation ) );
       vol_path[ i ].push_back( max( v0 + vol_drift + vol_diffusion*vol_innovation, 0. ) );


       for( int j = 1; j < N; j++ )
       {

         stock_innovation = normal_dev.dev();
         vol_innovation   = stock_innovation*rho + normal_dev.dev()*rho_2;

         stock_drift      = ( r - 0.5*vol_path[ i ][ j-1 ] )*dt;
         stock_diffusion  = sqrt( dt*vol_path[ i ][ j-1 ] ); 
         
         vol_drift        = kappa*( theta - vol_path[ i ][ j-1 ] )*dt;
         vol_diffusion    = volofvol*stock_diffusion;

         stock_path[ i ].push_back( stock_path[ i ][ j-1 ]*exp( stock_drift + stock_diffusion*stock_innovation ) );
         vol_path[ i ].push_back( max( vol_path[ i ][ j-1 ] + vol_drift + vol_diffusion*vol_innovation, 0. ) );
       };
  };

};

