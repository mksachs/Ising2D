//
//  ISController.m
//  ising2dcpp
//
//  Created by Michael Sachs on 3/9/11.
//  Copyright 2011 Me. All rights reserved.
//

#import "ISController.h"
#import <vector>
#import <math.h>


@implementation ISController

- (IBAction)runModel:(id)sender {
    
    //float E = MI->get_E();
    //float M = MI->get_M();
    
    if (!running) {
    
        clear_flips = false;
        running = true;
        
        [initState setEnabled: NO];
        [nField setEnabled: NO];
        [rangeField setEnabled: NO];
        [resetButton setEnabled: NO];
        
        //[initState setState: NSOffState];
        
        if (show_plots)
            [self startPlotTimer];
        
        flips = [NSMutableDictionary dictionaryWithCapacity:N];
        
        NSThread* model_thread = [[NSThread alloc] initWithTarget:self selector:@selector(doRunModel:) object:nil];
        
        updateStateView = [NSTimer scheduledTimerWithTimeInterval:0.03 target:self selector:@selector(doUpdateStateView:) userInfo:nil repeats:YES];
        
        //updateEAndMPlots = [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(doUpdateEandMPlots:) userInfo:nil repeats:YES];
        
        [model_thread start];  // Actually create the thread
    }
    else {
        running = false;
        [initState setEnabled: YES];
        [nField setEnabled: YES];
        [rangeField setEnabled: YES];
        [resetButton setEnabled: YES];
        [updateEAndMPlots invalidate];
        [updateStateView invalidate];
    }

    
    //self.repeatingTimer = update_stateView;
    /*
    for (int i = 0; i<100; i++) {
        std::vector<int> flip = MI->try_flip();
        
        if (flip[0] > -1) {
            if (flip[1] == -1) {
                [flips setObject:[stateView getUpColor] forKey:[NSNumber numberWithInt:flip[0]]];
            }
            else {
                [flips setObject:[stateView getDownColor] forKey:[NSNumber numberWithInt:flip[0]]];
            }

            
            
            
           // NSLog(@"%i %i %i",flip[0],flip[1],flip[2]);
            //[stateView doFlip:index];
        }
        
        
        //[stateView setE:E andM:M];
        
    }
    */
    
    
    
}


- (void)doUpdateStateView:(id)sender {
    [stateView setNeedsDisplay:YES];
}

- (void)doRunModel:(id)sender {
    @autoreleasepool { // Top-level pool
        int i = 0;
        //int sweeps = 0;
        float e;
        float m;
        
        while (running) {
            if (clear_flips) {
                //clear the flips array
                flips = [NSMutableDictionary dictionaryWithCapacity:N];
                clear_flips = false;
            }
            std::vector<int> flip = MI->try_flip_exp();
            if (flip[0] > -1) {
                if (flip[1] == -1) {
                    flips[@(flip[0])] = [stateView getUpColor];
                }
                else {
                    flips[@(flip[0])] = [stateView getDownColor];
                }
            }
            i++;
            
            if (i == N) {
                e = MI->get_E()/N;
                m = MI->get_M()/N;
                [sweepsField setIntValue:[sweepsField intValue]+1];
                [mField setFloatValue:m];
                [eField setFloatValue:e];
                [stateView doFlip:flips];
                clear_flips = true;
                //[stateView setNeedsDisplay:YES];
                i = 0;
                //sweeps++;
            }
            
            //if (sweeps == 10) {
            //    [stateView setNeedsDisplay:YES];
            //    sweeps = 0;
            //}
        }
    
    }
    
}

- (IBAction)resetModel:(id)sender {
    if (!running) {
        [self doResetModel];
        [stateView setNeedsDisplay:YES];
    }
}

- (void)doResetModel {
    
    [resetButton setEnabled: NO];
    
    int n = [nField intValue];
    float kbT = [kbTField floatValue];
    float h = [hField floatValue];
    int range = [rangeField intValue];
    [sweepsField setIntValue:0];
    N = n*n;
    
    MI = new MetropolisIsing;
    
    if ([initState selectedSegment] == 1) {
        MI->initSim(n, kbT, h, 1.0, range, false);
    }
    else if ([initState selectedSegment] == 0) {
        MI->initSim(n, kbT, h, 1.0, range, true);
    }
    
    [mField setFloatValue:MI->get_M()/N];
    [eField setFloatValue:MI->get_E()/N];
    
    [stateView initGridWithStates:MI->get_states() atWidth:n];
    
    [eAndMView resetAllVals];
    [eAndMView setEMax:0.5*[self getNumberOfNeighbors] - fabs(h) andEMin:-1*0.5*[self getNumberOfNeighbors] - fabs(h)];
    [eAndMView setNeedsDisplay:YES];
    
    [runButton setEnabled: YES];
    
}

- (int)getNumberOfNeighbors {
    if ([rangeField intValue] == 0) {
        return 4;
    } else {
        //NSLog(@"%f",pow(2*[rangeField intValue] + 1,2) - 1);
        return pow(2*[rangeField intValue] + 1,2) - 1;
    }

}

- (IBAction)showHidePlots:(id)sender {
    if (show_plots) {
        [plotBox setHidden:YES];
        show_plots = NO;
        [eAndMView resetAllVals];
        [eAndMView setEMax:0.5*[self getNumberOfNeighbors] - fabs([hField floatValue]) andEMin:-1*0.5*[self getNumberOfNeighbors] - fabs([hField floatValue])];
        [self stopPlotTimer];
    } else {
        [plotBox setHidden:NO];
        show_plots = YES;
        [self startPlotTimer];
    }

}

- (void)doUpdateEandMPlots:(id)sender {
    float e = MI->get_E()/N;
    float m = MI->get_M()/N;
    [eAndMView setE:e andM:m];
    [eAndMView setNeedsDisplay:YES];
}

- (IBAction)initStateChanged:(id)sender{
    if (!running) {
        [resetButton setEnabled: YES];
        [runButton setEnabled: NO];
    }
    /*
    if ( [IB_timeInterval selectedColumn] == 1 ) {
        timeInterval = (NSTimeInterval)6.0;
    } else {
        timeInterval = (NSTimeInterval)0.5;
    }
     */
}

- (void)startPlotTimer {
    if (running)
        updateEAndMPlots = [NSTimer scheduledTimerWithTimeInterval:0.1 target:self selector:@selector(doUpdateEandMPlots:) userInfo:nil repeats:YES];
}

- (void)stopPlotTimer {
    [updateEAndMPlots invalidate];
}

- (void)awakeFromNib
{
    [nField setIntValue:100.0];
    [kbTField setFloatValue:1.0];
    [hField setFloatValue:0.0];
    [rangeField setIntValue:0];
    
    [plotBox setHidden:YES];
    
    show_plots = NO;
    running = NO;
    
    [self doResetModel];
}

- (IBAction)tempChange:(id)sender {
    MI -> set_kbT([kbTField floatValue]);
}

- (IBAction)nChange:(id)sender {
    if (!running) {
        [resetButton setEnabled: YES];
        [runButton setEnabled: NO];
    }
}

- (IBAction)rangeChange:(id)sender {
    if (!running) {
        [resetButton setEnabled: YES];
        [runButton setEnabled: NO];
    }
}

- (IBAction)hChange:(id)sender {
    MI -> set_h([hField floatValue]);
    if (show_plots) {
        [eAndMView setEMax:0.5*[self getNumberOfNeighbors] - fabs([hField floatValue]) andEMin:-1*0.5*[self getNumberOfNeighbors] - fabs([hField floatValue])];
    }
}


@end
