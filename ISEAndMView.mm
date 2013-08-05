//
//  ISEAndMView.m
//  ising2dcpp
//
//  Created by Michael Sachs on 3/14/11.
//  Copyright 2011 Me. All rights reserved.
//

#import "ISEAndMView.h"


@implementation ISEAndMView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        float leftMargin = 13;
        float bottomMargin = 20;
        plotWidth = self.bounds.size.width - leftMargin;
        plotHeight = (self.bounds.size.height/2.0) - bottomMargin;
        mPlotBorder = NSMakeRect(leftMargin, bottomMargin, plotWidth, plotHeight);
        ePlotBorder = NSMakeRect(leftMargin, self.bounds.size.height - plotHeight, plotWidth, plotHeight);
        plot_label_font_attributes = [NSMutableDictionary new];
        plot_label_font = [NSFont fontWithName:@"LucidaGrande" size:9];
        plot_label_font_attributes[NSFontAttributeName] = plot_label_font;
        e_label_point = NSMakePoint(0, self.bounds.size.height - (plotHeight/2.0));
        m_label_point = NSMakePoint(0, plotHeight/2.0 + bottomMargin);
        e_sweep_label_point = NSMakePoint(self.bounds.size.width - plotWidth/2.0 - 15, self.bounds.size.height - plotHeight-10);
        m_sweep_label_point = NSMakePoint(self.bounds.size.width - plotWidth/2.0 - 15, bottomMargin-10);
        e_plot_line = [NSBezierPath bezierPath];
        m_plot_line = [NSBezierPath bezierPath];
        plotYPadding = 4.0;
        
        [self resetAllVals];
        
        [e_plot_line setLineWidth: 2];
        [m_plot_line setLineWidth: 2];
        
        mMax = 1.0;
        mMin = -1.0;
    }
    return self;
}

- (void)setEMax:(float)inEMax andEMin:(float)inEMin {
    //[self rescaleEtoEMax:inEMax EMin:inEMin];
    NSLog(@"%f -> %f, %f -> %f",eMax,inEMax,eMin,inEMin);
    eMax = inEMax;
    eMin = inEMin;
    
}

- (void)rescaleEtoEMax:(float)newMax EMin:(float)newMin {
    for (int i = 0; i<PLOT_POINTS; i++) {
        if (e_vals[i] != NULL_VAL) {
            float pre = e_vals[i];
            if (e_vals[i] < 0)
                e_vals[i] *= (newMin/eMin);
            else if (e_vals[i] >= 0)
                e_vals[i] *= (newMax/eMax);
            NSLog(@"%f -> %f",pre,e_vals[i]);
        }
    }
    NSLog(@"");
}

//- (void)setMMax:(float)inMMax andMMin:(float)inMMin {
//    mMax = inMMax;
//    mMin = inMMin;
//}

- (void)setE:(float)inE andM:(float)inM {
    if (vals_set < PLOT_POINTS) {
        //add to vals
        e_vals[vals_set] = inE;
        m_vals[vals_set] = inM;
        vals_set++;
    } else {
        //move vals down
        for (int i = 0; i < PLOT_POINTS - 1; i++) {
            e_vals[i] = e_vals[i+1];
            m_vals[i] = m_vals[i+1];
        }
        
        e_vals[PLOT_POINTS-1] = inE;
        m_vals[PLOT_POINTS-1] = inM;
        
    }
}

- (void)resetAllVals {
    for (int i = 0; i<PLOT_POINTS; i++) {
        //NSLog(@"%f %f",e_vals[i],m_vals[i]);
        //e_vals[i] = (self.bounds.size.height - plotHeight) + (i/100)*plotHeight;
        //m_vals[i] = (leftMargin) + (i/100)*plotHeight;
        e_vals[i] = NULL_VAL;
        m_vals[i] = NULL_VAL;
    }
    
    vals_set = 0;
}



- (float)ePlotCoordWithValue:(float)inEVal {
    //NSLog(@"e %f %f",inEVal,((plotHeight/(eMax - eMin))*(inEVal - eMin) ) + ePlotBorder.origin.y);
    return (((plotHeight-2*plotYPadding)/(eMax - eMin))*(inEVal - eMin) ) + ePlotBorder.origin.y + plotYPadding;
}

- (float)mPlotCoordWithValue:(float)inMVal {
    //NSLog(@"m %f %f",inMVal,((plotHeight/(mMax - mMin))*(inMVal - mMin) ) + mPlotBorder.origin.y);
    return (((plotHeight-2*plotYPadding)/(mMax - mMin))*(inMVal - mMin) ) + mPlotBorder.origin.y + plotYPadding;
}

- (void)drawEandMPaths {
    if (![e_plot_line isEmpty]) {
        [e_plot_line removeAllPoints];
    }
    
    if (![m_plot_line isEmpty]) {
        [m_plot_line removeAllPoints];
    }
    
    for (int i = 0; i < vals_set - 1; i++) {
        if (i == 0) {
            [e_plot_line  moveToPoint: NSMakePoint(ePlotBorder.origin.x, [self ePlotCoordWithValue:e_vals[i]])];
            [m_plot_line  moveToPoint: NSMakePoint(mPlotBorder.origin.x, [self mPlotCoordWithValue:m_vals[i]])];
        }
        
        float e_plot_y = [self ePlotCoordWithValue:e_vals[i + 1]];
        float curr_e_plot_y = [self ePlotCoordWithValue:e_vals[i]];
        
        if (e_plot_y < ePlotBorder.origin.y && curr_e_plot_y < ePlotBorder.origin.y)
            [e_plot_line  moveToPoint: NSMakePoint(ePlotBorder.origin.x + ((i+1)/float(PLOT_POINTS))*plotWidth, ePlotBorder.origin.y)];
        else if (e_plot_y < ePlotBorder.origin.y && curr_e_plot_y > ePlotBorder.origin.y)
            [e_plot_line  lineToPoint: NSMakePoint(ePlotBorder.origin.x + ((i+1)/float(PLOT_POINTS))*plotWidth, ePlotBorder.origin.y)];
        else
            [e_plot_line  lineToPoint: NSMakePoint(ePlotBorder.origin.x + ((i+1)/float(PLOT_POINTS))*plotWidth, e_plot_y)];
        
        [m_plot_line  lineToPoint: NSMakePoint(mPlotBorder.origin.x + ((i+1)/float(PLOT_POINTS))*plotWidth, [self mPlotCoordWithValue:m_vals[i + 1]])];
    }
    
    [[NSColor grayColor] set]; 
    [e_plot_line stroke];
    [m_plot_line stroke];
    
    //NSLog(@"%@",[e_plot_line description]);
    
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
    
    [[NSColor whiteColor] set];
    NSRectFill ( mPlotBorder );
    NSRectFill ( ePlotBorder );
    
    [[NSColor grayColor] set];
    NSFrameRectWithWidth ( mPlotBorder, 1 );
    NSFrameRectWithWidth ( ePlotBorder, 1 );
    
    [@"e" drawAtPoint:e_label_point withAttributes:plot_label_font_attributes];
    [@"m" drawAtPoint:m_label_point withAttributes:plot_label_font_attributes];
    [@"sweeps" drawAtPoint:e_sweep_label_point withAttributes:plot_label_font_attributes];
    [@"sweeps" drawAtPoint:m_sweep_label_point withAttributes:plot_label_font_attributes];
    
    [self drawEandMPaths];
    
}

@end
