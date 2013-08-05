/*
 *  MetroplisIsing.cpp
 *  ising2dcpp
 *
 *  Created by Michael Sachs on 3/6/11.
 *  Copyright 2011 Me. All rights reserved.
 *
 */

#include <iostream>
#include "MetroplisIsing.h"
#include <math.h>
#include <time.h>
#include <map>

void MetroplisIsing::initExp() {
    
    //std::cout << "calc exp\n";
    
    //min_dE = -4.0*J*2.0 - 2.0*h;
    //max_dE = 4.0*J*2.0 + 2.0*h;
    
    float min_NN;
    float max_NN;
    
    if (range == 0) {
        min_NN = -4.0*J;
        max_NN = 4.0*J;
    }
    else {
        min_NN = -1*(range_n*range_n-1)*J;
        max_NN = (range_n*range_n-1)*J;
    }

    //std::cout << int(max_NN) +1 << " " << min_NN << " " << max_NN <<"\n\n";
    
    float * exp_calc_base_1 = new (std::nothrow) float [int(max_NN) + 1];
    //float * exp_calc_base_2 = new (std::nothrow) float [(int(max_NN) + 1)*2];
    
    
    if (exp_calc_base_1 == 0)
        std::cout << "Error: memory could not be allocated";
    else
    {
        int p = 0;
        
        for (float f = min_NN; f<=max_NN; f += 2) {
            exp_calc_base_1[p] = f + h;
            //exp_calc[p] = exp(2.0*f);
            //std::cout << p << " " << exp_calc[p] << " " << f << " " << 2.0*f <<"\n";
            //std::cout << p << " " << f + h << " " << 2.0*f <<"\n";
            p += 1;
        }
        
        for (int i = 0; i < (int(max_NN) + 1)*2; i++) {
            if (i < int(max_NN) + 1) {
                exp_calc[2.0*exp_calc_base_1[i]] = exp(-2.0*exp_calc_base_1[i]/kbT);
            }
            else {
                exp_calc[-2.0*exp_calc_base_1[i - (int(max_NN) + 1)]] = exp(2.0*exp_calc_base_1[i - (int(max_NN) + 1)]/kbT);
            }
            
            //exp_calc[p] = exp(2.0*f);
            //std::cout << i << " " << i - (int(max_NN) + 1) << " " <<exp_calc_base_2[i] <<"\n";
            //std::cout << p << " " << f + h << " " << 2.0*f <<"\n";
        }
        /*
        std::map<float,float>::iterator it;
        
        for ( it=exp_calc.begin() ; it != exp_calc.end(); it++ )
            std::cout << (*it).first << " => " << (*it).second << "\n";
         */
        
        
        //delete[] exp_calc_base_1;
        
        
        
        //std::cout << p << " " << max_dE - min_dE + 1;
    }
    
}

float MetroplisIsing::dE(int i) {
    //return 2.0*J*states[i]*(states[mod(i+1, N)]+states[mod(i-1, N)]+states[mod(i+n, N)]+states[mod(i-n, N)]) + 2.0*h*states[i];
    return 2.0*J*states[i]*(n_sum(i)) + 2.0*h*states[i];
    //return 2.0*J*states[i]*(states[(i+1)%N]+states[(i-1)%N]+states[(i+n)%N]+states[(i-n)%N]) + 2.0*h*states[i];
    /*
    int thesum = states[mod(i+1, N)]+states[mod(i-1, N)]+states[mod(i+n, N)]+states[mod(i-n, N)];
    if (thesum == 3) {
        std::cout << i << "\n";
        std::cout << states[mod(i+1, N)] << "\t"  << states[mod(i-1, N)] << "\t"  << states[mod(i+n, N)] << "\t" << states[mod(i-n, N)] << "\n";
        std::cout << mod(i+1, N) << "\t"  << mod(i-1, N) << "\t"  << mod(i+n, N) << "\t" << mod(i-n, N) << "\n\n";
        
    }
    return thesum;
     */
}

void MetroplisIsing::try_flip() {
    //srand ( time(0) );
    
    //std::cout << rand() % N;
    
    int try_index = rand() % N;
    float try_dE = dE(try_index);
    
    if (try_dE <= 0.0) {
        states[try_index] = -1*states[try_index];
        //std::cout << 1 << "\n";
        E += try_dE;
        M += 2.0*states[try_index];
    }
    else {
        //std::cout << float(rand())/float(RAND_MAX) << "\n";
        if (float(rand())/float(RAND_MAX) < exp_calc[try_dE]) {
            states[try_index] = -1*states[try_index];
            //std::cout << 0 << "\n";
            E += try_dE;
            M += 2.0*states[try_index];
        }
    }

    
    //std::cout << try_dE << "\n";
    
    //return try_dE;
}
