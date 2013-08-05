/*
 *  Ising.cpp
 *  ising2dcpp
 *
 *  Created by Michael Sachs on 3/6/11.
 *  Copyright 2011 Me. All rights reserved.
 *
 */

#include <iostream>
#include "Ising.h"
#include <time.h>

double Ising::get_E() {
    return E;
}

float Ising::get_M() {
    return M;
}

int Ising::get_N() {
    return N;
}

int Ising::mod(int a, int b) {
    int result = a%b;
    
    if (result < 0)
        return result + b;
    else
        return result;
}

int * Ising::get_states() {
    return states;
}

int Ising::get_state(int i) {
    return states[i];
}

int Ising::n_sum(int i) {
    int the_sum;
    
    if (range == 0) {
        the_sum = states[mod(i+1, N)]+states[mod(i-1, N)]+states[mod(i+n, N)]+states[mod(i-n, N)];
        return the_sum;
    }
    else {
        //new stuff
        the_sum = 0;
        int start_p;
        
        //sum over the inline row
        start_p = mod((i-range),N);
        //std::cout << start_p << std::endl;
        for (int p = 0; p<range_n; p++) {
            the_sum += states[mod(start_p+p,N)];
            //std::cout << mod(start_p+p,N) << " ";
        }
        //std::cout << std::endl;
        
        //sum over rows above and below
        for (int d_row = 1; d_row<=range; d_row++) {
            start_p = mod((i-d_row*n),N) - range;
            for (int p = 0; p<range_n; p++) {
                the_sum += states[mod(start_p+p,N)];
                //std::cout << mod(start_p+p,N) << " ";
            }
           // std::cout << std::endl;
            start_p = mod((i+d_row*n),N) - range;
            for (int p = 0; p<range_n; p++) {
                the_sum += states[mod(start_p+p,N)];
                //std::cout << mod(start_p+p,N) << " ";
            }
            //std::cout << std::endl;
        }
        
        return the_sum - states[i];
    }
    //std::cout << the_sum - states[i] << std::endl;
    
    
    
}

void Ising::set_kbT(float in_kbT) {
    kbT = in_kbT;
    initExp();
    initE();
}

void Ising::set_h(float in_h) {
    h = in_h;
    initExp();
    initE();
}

void Ising::initSim(int in_n, float in_kbT, float in_h, float in_J, int in_range, bool startInf) {
    
    int state;
    
    n = in_n;
    N = n*n;
    kbT = in_kbT;
    h = in_h;
    J = in_J;
    range = in_range;
    range_n = 2*range + 1;
    
    states = new (std::nothrow) int[N];
    
    if (states == 0)
        std::cout << "Error: memory could not be allocated";
    else
    {
        if (startInf) {
            srand ( time(NULL) );
            
            for (int i = 0; i<N; i++) {
                state = rand();
                
                if (state % 2 == 1)
                    states[i] = 1;
                else 
                    states[i] = -1;
            }
            
        }
        else {
            for (int i = 0; i<N; i++) {
                states[i] = -1;
            }
        }
        
        /*
        for (int i = 1; i<=N; i++) {
            int currstate = states[i-1];
            if (currstate == 1) 
                std::cout << " " << states[i-1];
            else
                std::cout << states[i-1];
            
            if (i % n == 0)
                std::cout << "\n";
        }
        */
        //n_sum(6);
        
        initE();
        initM();
        initExp();
    }
}

void Ising::initE() {
    E = 0.0;
    for (int i = 0; i<N; i++) {
        E += -1*(0.5*J*states[i]*(n_sum(i)) + h*states[i]);
    }
    
    //std::cout << "Energy: " << E << "\n";
}

void Ising::initM() {
    M = 0.0;
    for (int i = 0; i<N; i++) {
        M += states[i];
    }
    
    //std::cout << "Mag: " << M << "\n";
}
