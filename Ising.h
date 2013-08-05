/*
 *  Ising.h
 *  ising2dcpp
 *
 *  Created by Michael Sachs on 3/6/11.
 *  Copyright 2011 Me. All rights reserved.
 *
 */

class Ising {
public:
    double get_E();
    float get_M();
    int get_N();
    int get_state(int);
    int * get_states();
    void initSim(int,float,float,float,int,bool);
    void set_kbT(float);
    void set_h(float);
protected:
    int * states;
    float h;
    float kbT;
    //neighborLevel = None
    int n;
    int N;
    //neighbors = None
    float * exp_calc;
    //can_flip = True
    double E;
    float M;
    float J;
    void initE();
    void initM();
    virtual void initExp() = 0;
    int mod(int,int);
    int range;
    int n_sum(int);
    int range_n;
};

