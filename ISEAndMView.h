//
//  ISEAndMView.h
//  ising2dcpp
//
//  Created by Michael Sachs on 3/14/11.
//  Copyright 2011 Me. All rights reserved.
//

#define PLOT_POINTS 500
#define NULL_VAL -1234567.0

#import <Cocoa/Cocoa.h>


@interface ISEAndMView : NSView {
    float plotHeight;
    float plotWidth;
    NSRect ePlotBorder;
    NSRect mPlotBorder;
    NSMutableDictionary* plot_label_font_attributes;
    NSFont* plot_label_font;
    NSPoint e_label_point;
    NSPoint m_label_point;
    NSPoint e_sweep_label_point;
    NSPoint m_sweep_label_point;
    float e_vals[PLOT_POINTS];
    float m_vals[PLOT_POINTS];
    int vals_set;
    NSBezierPath *e_plot_line;
    NSBezierPath *m_plot_line;
    float eMax;
    float eMin;
    float mMax;
    float mMin;
    float plotYPadding;

}

- (void)setE:(float)inE andM:(float)inM;
- (void)drawEandMPaths;
- (void)setEMax:(float)inEMax andEMin:(float)inEMin;
//- (void)setMMax:(float)inMMax andMMin:(float)inMMin;
- (void)rescaleEtoEMax:(float)newMax EMin:(float)newMin;
- (float)ePlotCoordWithValue:(float)inEVal;
- (float)mPlotCoordWithValue:(float)inMVal;
- (void)resetAllVals;

@end
