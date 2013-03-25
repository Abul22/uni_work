//
//  DreamDetailViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 1/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "DreamDetailViewController.h"

@interface DreamDetailViewController ()<AVAudioPlayerDelegate,ChangeNameViewControllerDelgate,ChangeDescriptionViewControllerDelegate,AwesomeMenuDelegate,MBProgressHUDDelegate>

@end

@implementation DreamDetailViewController
@synthesize zenPlayerButton = _zenPlayerButton;
@synthesize uploadButton = _uploadButton;
@synthesize uploadURL = _uploadURL;
@synthesize menu = _menu;
@synthesize nameLable = _nameLable;
@synthesize descriptionLable = _descriptionLable;
@synthesize timeLable = _timeLable;
@synthesize delegate = _delegate;
@synthesize indexPath = _indexPath;
@synthesize dream = _dream;
@synthesize type = _type;
@synthesize urlOfDream = urlOfDream;

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
    
    self.navigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
    
    self.navigationItem.title = self.dream.name;
    self.nameLable.text = self.dream.name;
    self.descriptionLable.text = self.dream.descriotion;
    self.type = self.dream.type;
    NSDateFormatter *df = [[NSDateFormatter alloc]init];
    [df setDateFormat:@"MM-dd-h:mm:ss a"];
    self.timeLable.text =[df stringFromDate:self.dream.time];
    self.urlOfDream = [NSURL URLWithString:self.dream.url];
    
    SVSegmentedControl *redSC = [[SVSegmentedControl alloc] initWithSectionTitles:[NSArray arrayWithObjects:@"Sweet Dream", @"Nightmare", @"Other", nil]];
    [redSC addTarget:self action:@selector(segmentedControlChangedValue:) forControlEvents:UIControlEventValueChanged];
	
	redSC.crossFadeLabelsOnDrag = YES;
    redSC.font = [UIFont boldSystemFontOfSize:12];
    
	redSC.thumb.tintColor = [UIColor colorWithRed:0.6 green:0.2 blue:0.2 alpha:1];
    
    if ([self.type isEqualToString:@"Sweet Dream"])
        redSC.selectedIndex = 0;
    else if([self.type isEqualToString:@"Nightmare"])
        redSC.selectedIndex = 1;
    else
        redSC.selectedIndex = 2;
    
    
    
	[self.view addSubview:redSC];
	
	redSC.center = CGPointMake(160, 120);
    
    
    
    // create new zen player button
    self.zenPlayerButton = [[ZenPlayerButton alloc] initWithFrame:CGRectMake(200, 310, 95, 95)];
    // listening to tap event on the button
    [self.zenPlayerButton addTarget:self
                             action:@selector(zenPlayerButtonDidTouchUpInside:)
                   forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:self.zenPlayerButton];
        
    
    UIImage *storyMenuItemImage = [UIImage imageNamed:@"bg-menuitem.png"];
    UIImage *storyMenuItemImagePressed = [UIImage imageNamed:@"bg-menuitem-highlighted.png"];
    
    UIImage *facebookImage = [UIImage imageNamed:@"facebook.png"];
    UIImage *twitterImage = [UIImage imageNamed:@"twitter.png"];
    UIImage *mailImage = [UIImage imageNamed:@"mail.png"];
    UIImage *weiboImage = [UIImage imageNamed:@"weibo.png"];

    
    
    AwesomeMenuItem *facebookItem = [[AwesomeMenuItem alloc] initWithImage:storyMenuItemImage
                                                          highlightedImage:storyMenuItemImagePressed
                                                              ContentImage:facebookImage
                                                   highlightedContentImage:nil];
    AwesomeMenuItem *twitterItem = [[AwesomeMenuItem alloc] initWithImage:storyMenuItemImage
                                                         highlightedImage:storyMenuItemImagePressed
                                                             ContentImage:twitterImage
                                                  highlightedContentImage:nil];
    AwesomeMenuItem *mailItem = [[AwesomeMenuItem alloc] initWithImage:storyMenuItemImage
                                                      highlightedImage:storyMenuItemImagePressed
                                                          ContentImage:mailImage
                                               highlightedContentImage:nil];
    AwesomeMenuItem *weiboItem = [[AwesomeMenuItem alloc] initWithImage:storyMenuItemImage
                                                      highlightedImage:storyMenuItemImagePressed
                                                          ContentImage:weiboImage
                                               highlightedContentImage:nil];
    NSArray *menus = [NSArray arrayWithObjects:twitterItem,facebookItem,weiboItem,mailItem, nil];
    self.menu = [[AwesomeMenu alloc] initWithFrame:self.view.bounds menus:menus];
    
    self.menu.startPoint = CGPointMake(60.0, 355.0);
	self.menu.menuWholeAngle = M_PI * 0.8;
	self.menu.farRadius = 90.0f;
	self.menu.endRadius = 80.0f;
    self.menu.delegate = self;
    
    [self.view addSubview:self.menu];

    if ([self.dream.uploadURL isEqualToString:@""]) {
        [self addUpladButton];
    }
    
}

- (void)viewDidUnload
{
    [self setNameLable:nil];
    [self setDescriptionLable:nil];
    [self setTimeLable:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

- (void)segmentedControlChangedValue:(SVSegmentedControl*)segmentedControl {
    switch ((int)segmentedControl.selectedIndex) {
        case 0:
            self.type = @"Sweet Dream";
            break;
        case 1:
            self.type = @"Nightmare";
            break;
        case 2:
            self.type = @"Other";
            break;
        default:
            break;
    }
    self.dream.type = self.type;
}

- (void) zenPlayerButtonDidTouchUpInside:(id)sender;
{
    if (!player.playing) {
        NSError *error = nil;
        player = [[AVAudioPlayer alloc]initWithContentsOfURL:self.urlOfDream error:&error];
        
        player.delegate = self;
        if (error) {
            NSLog(@"error: %@",[error localizedDescription]);
        }
        else
            [player play];
    }
    else
        [player stop];
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if ([segue.identifier isEqualToString:@"Change Name"]) {
        ChangeNameViewController *namer = (ChangeNameViewController *)segue.destinationViewController;
        namer.name = self.dream.name;
        namer.delegate = self;
        
    }
    else if ([segue.identifier isEqualToString:@"Change Description"]){
        ChangeDescriptionViewController *descriptioner = (ChangeDescriptionViewController *)segue.destinationViewController;
        descriptioner.description = self.dream.descriotion;
        descriptioner.delegate = self;
    }
    
}

-(void)changeNameViewController:(ChangeNameViewController *)sender getTheNewName:(NSString *)name{
    self.nameLable.text = name;
    self.dream.name = name;
    self.navigationItem.title = name;
    
}
-(void)changeDescriptionViewController:(ChangeDescriptionViewController *)sender getThenNewDescription:(NSString *)description{
    self.descriptionLable.text = description;
    self.dream.descriotion = description;
}
- (void)AwesomeMenu:(AwesomeMenu *)menu didSelectIndex:(NSInteger)idx
{
    if (![self.dream.uploadURL isEqualToString:@""]) {
        
        
        switch (idx) {
            case 0:
                [self openTwitter];
                break;
            case 1:
                [self openFacebook];
                break;
            case 2:
                [self openWeibo];
                break;
            case 3:
                [self openMail];
                break;
                
            default:
                break;
        }
    }
    else
        [SVProgressHUD showErrorWithStatus:@"Please Upload Dream!"];

}

-(void)openTwitter{
    [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        [self performTwitterOnMainThread];
        dispatch_async(dispatch_get_main_queue(), ^{
            [MBProgressHUD hideAllHUDsForView:self.view animated:YES];
        });
    });

}
-(void) performTwitterOnMainThread    {
    [self performSelectorOnMainThread:@selector(shareDreamWithTwitter) withObject:nil waitUntilDone:YES];
}

-(void)openFacebook{
    [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        [self performFacebookOnMainThread];
        dispatch_async(dispatch_get_main_queue(), ^{
            [MBProgressHUD hideAllHUDsForView:self.view animated:YES];
        });
    });
    
}
-(void) performFacebookOnMainThread    {
    [self performSelectorOnMainThread:@selector(shareDreamWithFacebook) withObject:nil waitUntilDone:YES];
}

-(void)openWeibo{
    [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        [self performWeiboOnMainThread];
        dispatch_async(dispatch_get_main_queue(), ^{
            [MBProgressHUD hideAllHUDsForView:self.view animated:YES];
        });
    });
    
}
-(void) performWeiboOnMainThread    {
    [self performSelectorOnMainThread:@selector(shareDreamWithWeibo) withObject:nil waitUntilDone:YES];
}


-(void)openMail{
    [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_LOW, 0), ^{
        [self performMailOnMainThread];
        dispatch_async(dispatch_get_main_queue(), ^{
            [MBProgressHUD hideAllHUDsForView:self.view animated:YES];
        });
    });
    
}
-(void) performMailOnMainThread    {
    [self performSelectorOnMainThread:@selector(shareDreamWihtMail) withObject:nil waitUntilDone:YES];
}

-(void)addUpladButton{
    if ([self.dream.uploadURL isEqualToString:@""]) {
        self.uploadButton = [MAConfirmButton buttonWithTitle:@"Upload Dream" confirm:@"Are You Sure?"];
        [self.uploadButton addTarget:self action:@selector(confirmActionOfUpload:) forControlEvents:UIControlEventTouchUpInside];
        [self.uploadButton setAnchor:CGPointMake(300, 270)];
        [self.view addSubview:self.uploadButton];
    }
}
-(void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{
    self.zenPlayerButton.state = ZenPlayerButtonStateNormal;
}

- (void)confirmActionOfUpload:(id)sender{
    SCShareViewController *shareViewController;
    NSURL *trackURL = self.urlOfDream;
    
    shareViewController = [SCShareViewController shareViewControllerWithFileURL:trackURL
                                                              completionHandler:^(NSDictionary *trackInfo, NSError *error){
                                                                  
                                                                  if (SC_CANCELED(error)) {
                                                                      [SVProgressHUD showErrorWithStatus:@"Cancele Upload!"];
                                                                      [self addUpladButton];
                                                                  } else if (error) {
                                                                      [SVProgressHUD showErrorWithStatus:@"Upload Field!"];
                                                                      [self addUpladButton];
                                                                      [self.view setNeedsDisplay];
                                                                  } else {
                                                                      [SVProgressHUD showSuccessWithStatus:@"Upload Success!"];
                                                                      self.dream.uploadURL = [trackInfo objectForKey:@"permalink_url"];
                                                                      [sender disableWithTitle:@"Dream Uploaded"];
                                                                  }
                                                              }];
    
    
    
    // Now present the share view controller.
    [self presentViewController:shareViewController animated:YES completion:nil];

}

-(void)shareDreamWihtMail{
    Class mailClass = (NSClassFromString(@"MFMailComposeViewController"));
	if (mailClass != nil)
	{
		// We must always check whether the current device is configured for sending emails
		if ([mailClass canSendMail])
		{
			[self displayComposerSheet];
		}
		else
		{
			[self launchMailAppOnDevice];
		}
	}
	else
	{
		[self launchMailAppOnDevice];
	}
    
}

-(void)shareDreamWithTwitter{
    
    
    SLComposeViewController *twitter = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeTwitter];
    
    
    if([SLComposeViewController isAvailableForServiceType:SLServiceTypeTwitter])
    {
        SLComposeViewControllerCompletionHandler __block completionHandler=^(SLComposeViewControllerResult result){
            
            [twitter dismissViewControllerAnimated:YES completion:nil];
            
            if (result == SLComposeViewControllerResultCancelled)
                [SVProgressHUD showErrorWithStatus: @"Twiiter: Canceled"];
            else if (result == SLComposeViewControllerResultDone)
                [SVProgressHUD showSuccessWithStatus: @"Twiiter: Post"];
        };
        
        [twitter addURL:[NSURL URLWithString:self.dream.uploadURL]];
        [twitter setInitialText:@"This is my dream record by Dream Catcher"];
        [twitter setCompletionHandler:completionHandler];
        [self presentViewController:twitter animated:YES completion:nil];
    }

}

-(void)shareDreamWithFacebook{
    
    SLComposeViewController *facebook = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeFacebook];
    
    
    if([SLComposeViewController isAvailableForServiceType:SLServiceTypeFacebook])
    {
        SLComposeViewControllerCompletionHandler __block completionHandler=^(SLComposeViewControllerResult result){
            
            [facebook dismissViewControllerAnimated:YES completion:nil];
            
            if (result == SLComposeViewControllerResultCancelled)
                [SVProgressHUD showErrorWithStatus: @"Facebook: Canceled"];
            else if (result == SLComposeViewControllerResultDone)
                [SVProgressHUD showSuccessWithStatus: @"Facebook: Post"];
        };
        
        [facebook addURL:[NSURL URLWithString:self.dream.uploadURL]];
        [facebook setInitialText:@"This is my dream record by Dream Catcher"];
        [facebook setCompletionHandler:completionHandler];
        [self presentViewController:facebook animated:YES completion:nil];
    }
}

-(void)shareDreamWithWeibo{
    
    SLComposeViewController *weibo = [SLComposeViewController composeViewControllerForServiceType:SLServiceTypeSinaWeibo];
    
    
    if([SLComposeViewController isAvailableForServiceType:SLServiceTypeSinaWeibo])
    {
        SLComposeViewControllerCompletionHandler __block completionHandler=^(SLComposeViewControllerResult result){
            
            [weibo dismissViewControllerAnimated:YES completion:nil];
            
            if (result == SLComposeViewControllerResultCancelled)
                [SVProgressHUD showErrorWithStatus: @"Weibo: Canceled"];
            else if (result == SLComposeViewControllerResultDone)
                [SVProgressHUD showSuccessWithStatus: @"Weibo: Post"];
        };
        
        [weibo addURL:[NSURL URLWithString:self.dream.uploadURL]];
        [weibo setInitialText:@"This is my dream record by Dream Catcher"];
        [weibo setCompletionHandler:completionHandler];
        [self presentViewController:weibo animated:YES completion:nil];
    }
}

-(void)displayComposerSheet{
    MFMailComposeViewController *picker = [[MFMailComposeViewController alloc] init];
	picker.mailComposeDelegate = self;
	
	[picker setSubject:@"My Dream"];
    NSString *url = self.dream.uploadURL;
    NSString *info = @"This my Dream recording by Dream Catcher! Click the link to listen.";
    NSString *emailBody = [NSString stringWithFormat:@"%@\n%@",info,url];
	[picker setMessageBody:emailBody isHTML:NO];
    [self presentViewController:picker animated:YES completion:nil];

    
}

- (void)mailComposeController:(MFMailComposeViewController*)controller didFinishWithResult:(MFMailComposeResult)result error:(NSError*)error
{
	// Notifies users about errors associated with the interface
	switch (result)
	{
		case MFMailComposeResultCancelled:
			[SVProgressHUD showErrorWithStatus: @"Mail: Canceled"];
			break;
		case MFMailComposeResultSaved:
            [SVProgressHUD showSuccessWithStatus: @"Mail: Saved"];
			break;
		case MFMailComposeResultSent:
            [SVProgressHUD showSuccessWithStatus: @"Mail: Sent"];
			break;
		case MFMailComposeResultFailed:
            [SVProgressHUD showErrorWithStatus: @"Mail: Failed"];
			break;
		default:
            [SVProgressHUD showErrorWithStatus: @"Mail: Not Sent"];
			break;
	}
    [self dismissViewControllerAnimated:YES completion:nil];
    
}

-(void)launchMailAppOnDevice{
    NSString *recipients = @"subject=My Dream";
	NSString *body = @"&body=This is my Dream!";
	
	NSString *email = [NSString stringWithFormat:@"%@%@", recipients, body];
	email = [email stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
	
	[[UIApplication sharedApplication] openURL:[NSURL URLWithString:email]];
    
}

@end
