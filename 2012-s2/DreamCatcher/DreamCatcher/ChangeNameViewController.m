//
//  ChangeNameViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 9/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "ChangeNameViewController.h"

@interface ChangeNameViewController ()<UITextFieldDelegate>

@end

@implementation ChangeNameViewController 
@synthesize nameTestField = _nameTestField;
@synthesize delegate = _delegate;
@synthesize name = _name;

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

// hide the keybord only after user input someting
-(BOOL)textFieldShouldReturn:(UITextField *)textField{
    if ([textField.text length]) {
        [textField resignFirstResponder];
        return YES;
    }else{
        return NO;
    }
}

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view.
    UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 480)];
    self.view.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];
    
    self.nameTestField.text = self.name;
    
}

- (void)viewDidUnload
{
    [self setNameTestField:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

-(void)viewWillDisappear:(BOOL)animated{
    self.name = self.nameTestField.text;
    [self.delegate changeNameViewController:self getTheNewName:self.name];
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

@end
