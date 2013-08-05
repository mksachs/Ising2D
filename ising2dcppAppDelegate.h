//
//  ising2dcppAppDelegate.h
//  ising2dcpp
//
//  Created by Michael Sachs on 3/9/11.
//  Copyright 2011 Me. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface ising2dcppAppDelegate : NSObject <NSApplicationDelegate> {
    NSWindow *__strong window;
}

@property (strong) IBOutlet NSWindow *window;

@end
