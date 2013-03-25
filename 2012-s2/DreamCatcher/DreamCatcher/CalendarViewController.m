//
//  CalendarViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 22/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "CalendarViewController.h"

@interface CalendarViewController ()

@end

@implementation CalendarViewController
@synthesize pmCC = _pmCC;

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization
    }
    return self;
}

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
	// Do any additional setup after loading the view.
    UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 480)];
    self.view.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];
    self.pmCC = [[PMCalendarController alloc] initWithThemeName:@"apple calendar"];
    
    self.pmCC.delegate = self;
    self.pmCC.mondayFirstDayOfWeek = NO;
    
    [self.pmCC presentCalendarFromRect:CGRectMake(0, -100, 300, 200)
                           inView:self.view
         permittedArrowDirections:PMCalendarArrowDirectionAny
                         animated:YES];

    
}

-(BOOL)calendarControllerShouldDismissCalendar:(PMCalendarController *)calendarController{
    calendarController.delegate = self;
    return NO;
}

- (void)viewDidUnload
{
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
