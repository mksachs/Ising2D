//
//  ISController.h
//  ising2dcpp
//
//  Created by Michael Sachs on 3/9/11.
//  Copyright 2011 Me. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MetropolisIsing.h"
#import "ISStateView.h"
#import "ISEAndMView.h"

@interface ISController : NSObject {
    IBOutlet NSTextField *nField;
    IBOutlet NSTextField *hField;
    IBOutlet NSTextField *kbTField;
    IBOutlet NSTextField *rangeField;
    IBOutlet NSSegmentedControl *initState;
    IBOutlet NSButton *resetButton;
    IBOutlet NSButton *runButton;
    IBOutlet NSBox *plotBox;
    IBOutlet NSButton *plotRevealButton;
    
    IBOutlet NSTextField *sweepsField;
    IBOutlet NSTextField *mField;
    IBOutlet NSTextField *eField;
    
    IBOutlet ISStateView *stateView;
    IBOutlet ISEAndMView *eAndMView;
    
    NSMutableDictionary *flips;
    Boolean clear_flips;
    Boolean show_plots;
    Boolean running;
    NSTimer *updateEAndMPlots;
    NSTimer *updateStateView;
    int N;
    
    MetropolisIsing * MI;
}

- (IBAction)runModel:(id)sender;
- (IBAction)resetModel:(id)sender;
- (IBAction)initStateChanged:(id)sender;
- (IBAction)tempChange:(id)sender;
- (IBAction)hChange:(id)sender;
- (IBAction)nChange:(id)sender;
- (IBAction)rangeChange:(id)sender;
- (IBAction)showHidePlots:(id)sender;

- (void)doResetModel;
- (void)doRunModel:(id)sender;
- (int)getNumberOfNeighbors;
- (void)doUpdateEandMPlots:(id)sender;

- (void)startPlotTimer;
- (void)stopPlotTimer;

@end
