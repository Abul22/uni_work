//
//  ChangeDescriptionViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 9/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "ChangeDescriptionViewController.h"

@interface ChangeDescriptionViewController ()<UITextViewDelegate>

@end

@implementation ChangeDescriptionViewController
@synthesize descriptionTextView = _descriptionTextView;
@synthesize delegate = _delegate;
@synthesize description = _description;

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
    
    self.descriptionTextView.text = self.description;
    
}

- (void)viewDidUnload
{
    [self setDescriptionTextView:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

-(void)viewWillDisappear:(BOOL)animated{
    self.description = self.descriptionTextView.text;
    [self.delegate changeDescriptionViewController:self getThenNewDescription:self.description];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
