//
//  CalendarViewController.h
//  DreamCatcher
//
//  Created by Chao Ruan on 22/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import <UIKit/UIKit.h>
#import "PMCalendar/PMCalendar.h"

@interface CalendarViewController : UIViewController <PMCalendarControllerDelegate>
@property (nonatomic, strong) PMCalendarController *pmCC;

@end
