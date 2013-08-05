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

class MetroplisIsing : public Ising {
public:
    void try_flip();
private:
    void initExp();
    float dE(int);
    std::map<float,float> exp_calc;
};
