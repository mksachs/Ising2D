//
//  ISView.h
//  ising2dcpp
//
//  Created by Michael Sachs on 3/9/11.
//  Copyright 2011 Me. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface ISStateView : NSView {
    float E;
    float M;
    float blockSpacing;
    float blockWidth;
    float blockHeight;
    NSMutableArray *blocks;
    NSMutableArray *colors;
    float N;
@public
    NSColor *upColor;
    NSColor *downColor;
    //NSString *upColorDescription;
    //NSString *downColorDescription;
}

//- (void)setE:(float)newE andM:(float)newM;
- (void)initGridWithStates:(int*)states atWidth:(int)n;
- (void)doFlip:(NSMutableDictionary*)flips;
- (NSColor*)getUpColor;
- (NSColor*)getDownColor;

@end
