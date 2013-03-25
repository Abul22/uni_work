//
//  AlremViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 22/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "AlremViewController.h"

@interface AlremViewController ()

@end

@implementation AlremViewController
@synthesize timeSwitch = _timeSwitch;
@synthesize timeLable = _timeLable;
@synthesize glowLable = _glowLable;
@synthesize datePicker=_datePicker;

-(void)setDatePicker:(UIDatePicker *)datePicker{

    if (_datePicker != datePicker) {
        _datePicker = datePicker;
    }
}

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

// set the image size 
-(UIImage*)scaleToSize:(UIImage*)img size:(CGSize)size{
    UIGraphicsBeginImageContext(size);
    [img drawInRect:CGRectMake(0, 0, size.width, size.height)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}


- (void)viewDidLoad
{
    [super viewDidLoad];
	// Add the backgrond image
    if (iPhone5) {
        UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 586)];
        self.view.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];
    }
    else{
        UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 480)];
        self.view.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];
    }
    
    // set the lable color
    self.timeLable.textColor = [UIColor colorWithRed:0.20 green:0.70 blue:1.0 alpha:1.0];
    self.glowLable.textColor = [UIColor colorWithRed:0.20 green:0.70 blue:1.0 alpha:1.0];
    

    
}

- (void)viewDidUnload
{
    [self setDatePicker:nil];
    [self setTimeSwitch:nil];
    [self setTimeLable:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

// set the time with out second 
- (NSDate *)dateWithZeroSeconds:(NSDate *)date
{
    NSTimeInterval time = floor([date timeIntervalSinceReferenceDate] / 60.0) * 60.0;
    return  [NSDate dateWithTimeIntervalSinceReferenceDate:time];
}


- (IBAction)setTheAlremTime:(id)sender {
    UIApplication *app = [UIApplication sharedApplication];

    if (self.timeSwitch.on) {
        // get the notification time
        int daysToAdd = 1;
        NSDate *time = self.datePicker.date;
        time = [self dateWithZeroSeconds:time];
        NSDate *now = [NSDate date];
        now = [self dateWithZeroSeconds:now];
        if ([time compare:now] == NSOrderedAscending) {
            time = [time dateByAddingTimeInterval:60*60*24*daysToAdd];
        }
        else
            [time dateByAddingTimeInterval:-60];

        NSDateFormatter *df = [[NSDateFormatter alloc]init];
        [df setDateFormat:@"MMMM dd, h:mm a"];
        NSString *timeString = [df stringFromDate:time];
        UIAlertView *setTimeAlert = [[UIAlertView alloc]initWithTitle:@"Set the alrem at" message:timeString delegate:nil cancelButtonTitle:@"Dismiss" otherButtonTitles:nil, nil];
        dispatch_async(dispatch_get_main_queue(), ^{
            [setTimeAlert show];
        });

        self.timeLable.text = timeString;
        // clean up the old notifications
        NSArray *oldNotifications = [app scheduledLocalNotifications];
        if ([oldNotifications count] > 0) {
            [app cancelAllLocalNotifications];
        }
        // creat a new notification for alarm
        UILocalNotification *notification=[[UILocalNotification alloc] init];
        if (notification != nil) {
            notification.fireDate = time;
            notification.timeZone=[NSTimeZone defaultTimeZone];
            notification.soundName = @"ring.caf";
            notification.repeatInterval = NSMinuteCalendarUnit;
            notification.alertBody = [NSString stringWithFormat:@"WAKE UP!!"];
            notification.alertAction = [NSString stringWithFormat:@"Record Dream"];
            [[UIApplication sharedApplication]scheduleLocalNotification:notification];
        }

    }
    else{
        [app cancelAllLocalNotifications];
        self.timeLable.text = [[NSString alloc]initWithFormat:@""];
    }
}
@end
