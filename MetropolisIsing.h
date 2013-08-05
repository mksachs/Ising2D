/*
 *  MetroplisIsing.h
 *  ising2dcpp
 *
 *  Created by Michael Sachs on 3/6/11.
 *  Copyright 2011 Me. All rights reserved.
 *
 */

#include "Ising.h"
#include <map>
#include <vector>

class MetropolisIsing : public Ising {
public:
    std::vector<int> try_flip();
    std::vector<int> try_flip_exp();
private:
    void initExp();
    float dE(int);
    //std::map<float,float> exp_calc;
    float *exp_calc;
    float max_dE;
};
