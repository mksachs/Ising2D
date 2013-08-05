//
//  ISView.m
//  ising2dcpp
//
//  Created by Michael Sachs on 3/9/11.
//  Copyright 2011 Me. All rights reserved.
//

#import "ISStateView.h"
#import <stdio.h>

@implementation ISStateView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        E = 0.0;
        M = 0.0;
        upColor  = [NSColor colorWithDeviceRed:58.0/255.0 green:53.0/255.0 blue:54.0/255.0 alpha:1];
        downColor = [NSColor colorWithDeviceRed:242.0/255.0 green:85.0/255.0 blue:44.0/255.0 alpha:1];
        //upColorDescription = [upColor description];
        //downColorDescription = [downColor description];
        blockSpacing = 1.0;
    }
    return self;
}

- (void)initGridWithStates:(int*)states atWidth:(int)n {
    
    int borderWidth = 2;
    float viewHeight = self.bounds.size.height-borderWidth*2;
    float viewWidth = self.bounds.size.width-borderWidth*2;
    N = n*n;
    
    blocks = [NSMutableArray arrayWithCapacity: (N)];
    colors = [NSMutableArray arrayWithCapacity: (N)];
    
    //NSLog(@"%f %f",viewHeight,viewWidth);
    
    if (N > 99) {
        blockSpacing = 0;
    }
    else {
        blockSpacing = 1.0;
    }
    
    blockWidth = (viewWidth - 2.0*blockSpacing - blockSpacing*(n-1))/n;
    blockHeight = (viewHeight - 2.0*blockSpacing - blockSpacing*(n-1))/n;
    
    int r = -1;
    int c = 0;
    for (int i = 0; i<N; i++) {
        if (i%n == 0) {
            r++;
            c = 0;
        }
        //NSLog(@"w:%f h:%f x:%f y:%f",blockWidth,blockHeight,r*(blockHeight+blockSpacing)+blockSpacing, c*(blockWidth+blockSpacing)+blockSpacing);
        
        //NSLog(@"%i",i%n);
        //[blocks addObject: [NSValue value:block]];
        //[blocks addObject: [NSValue value: &block withObjCType:@encode(NSRect)]];
        //blocks[i] = NSMakeRect ( r*(blockHeight+blockSpacing)+blockSpacing, c*(blockWidth+blockSpacing)+blockSpacing, blockWidth,blockHeight );
        // NSLog(@"w:%f h:%f x:%f y:%f",curr_block.size.width,curr_block.size.height,curr_block.origin.x, curr_block.origin.y);
        
        //this works
        NSRect curr_rect = NSMakeRect ( r*(blockHeight+blockSpacing)+blockSpacing+borderWidth, c*(blockWidth+blockSpacing)+blockSpacing+borderWidth, blockWidth,blockHeight );
        
        //NSBezierPath* curr_path = [NSBezierPath bezierPath];
        
        //[curr_path appendBezierPathWithRoundedRect:curr_rect xRadius:blockWidth/2 yRadius:blockHeight/2];
        
        //this works with just curr_rect
        [blocks addObject:NSStringFromRect(curr_rect)];
        
        //[blocks addObject:curr_path];
        
        
        
        if (states[i] == 1) {
            [colors addObject: upColor];
        }
        else {
            [colors addObject: downColor];
        }
        c++;
        
        
    }
    
    //blocks = local_blocks;
    
    //self.bounds.size
    
    //NSLog(@"%f %f",blockWidth,blockHeight);
    /*
    for (int i = 0; i < n*n; i++) {
        NSLog ( @"%i", states[i] );
    }
     */
}

- (NSColor*)getUpColor {
    return upColor;
}

- (NSColor*)getDownColor {
    return downColor;
}

- (void)doFlip:(NSMutableDictionary*)flips {
    
    for( NSNumber *aKey in [flips allKeys] )
    {
        // do something
        //NSLog(@"%@",[flips objectForKey: aKey]);
        colors[[aKey intValue]] = flips[aKey];
    }
    /*
    NSColor *color_to_flip = [colors objectAtIndex: index];
    //NSString *color_to_flip_description = [color_to_flip description];
    
    if (color_to_flip == upColor) {
        //flip to down color
        //NSLog(@"flip down");
        [colors replaceObjectAtIndex:index withObject:downColor];
    }
    else if (color_to_flip == downColor) {
        //flip to up color
        //NSLog(@"flip up");
        [colors replaceObjectAtIndex:index withObject:upColor];
    }
     */
    
    //NSLog ( @"%@ %@ %@", color_to_flip_description, upColorDescription,  downColorDescription);
    
}
- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
     //NSLog ( @"drawRect" );
    
    /*
    NSString *hello = [NSString stringWithFormat:@"The E is %.2f. The M is %.2f.", E, M];
    
    //NSString* hello = @"Hello, World!";
    NSPoint point = NSMakePoint(15, 75);
    NSMutableDictionary* font_attributes = [NSMutableDictionary new];
    NSFont* font = [NSFont fontWithName:@"Futura-MediumItalic" size:42];
    [font_attributes setObject:font forKey:NSFontAttributeName];
    
    [hello drawAtPoint:point withAttributes:font_attributes];
    
    [font_attributes release];
     */
    
    //NSRect local_blocks[N];
    [[NSColor grayColor] set];
    NSRectFill ( [self bounds] );
    
    //[[NSColor blackColor] set];
    //NSFrameRectWithWidth ( [self bounds], 1 );
    
    /* this works with square blocks */
    int i = 0;
    
    for (NSString *b_str in blocks) {
        [(NSColor*)colors[i] set];
        NSRectFill ( NSRectFromString(b_str) );
        i++;
    }
    
    
    /* this works with funny shaped blocks
    int i = 0;
    
    for (NSBezierPath *b_path in blocks) {
        [[colors objectAtIndex: i] set];
        [b_path fill];
        //NSRectFill ( NSRectFromString(b_str) );
        i++;
    }
     */
    
    //for (NSString *col in colors) {
    //    printf("%s\n",[[col description] UTF8String]);
    //}
    
    
    
    //int i = 0;
    
    //for (NSValue *value in blocks)
    
    //[[NSColor whiteColor] set];
    //for (int i = 0; i<N; i++)
    //for (NSValue *value in blocks)
   // {
       // NSValue *value
       // NSString *str = [blocks objectAtIndex: i];
        
       // NSRect curr_block = NSRectFromString([blocks objectAtIndex: i]);
        //NSLog(@"%f",[[blocks objectAtIndex: i] rectValue].origin.x);
        //[value getRectValue:&curr_block];
        //[[colors objectAtIndex: i] set];
        //[[blocks objectAtIndex: i] getValue:&curr_block];
        //NSRectFill ( curr_block );
        //NSLog(@"w:%f h:%f x:%f y:%f",curr_block.size.width,curr_block.size.height,curr_block.origin.x, curr_block.origin.y);
       // NSLog(@"%@",str);
        
        //i++;
    //}
    /*
    [[NSColor grayColor] set];
    NSRectFill ( [self bounds] );
    
    
    
    for (int i = 0; i<N; i++) {
        local_blocks[i] = NSRectFromString(blocks[i]);
    }
    
    //NSRect test = [&(blocks[0]) rectValue];
    
    NSRectFillListWithColors ( local_blocks, colors, N );
     */
}

@end
