//
//  NewDreamDetailViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 1/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "NewDreamDetailViewController.h"

@interface NewDreamDetailViewController ()<AVAudioPlayerDelegate,AwesomeMenuDelegate,MBProgressHUDDelegate,UITextViewDelegate,UITextFieldDelegate>

@end

@implementation NewDreamDetailViewController
@synthesize zenPlayerButton = _zenPlayerButton;
@synthesize uploadButton = _uploadButton;
@synthesize uploadURL = _uploadURL;
@synthesize menu = _menu;
@synthesize cells = _cells;
@synthesize NameCell = _NameCell;
@synthesize descriptionCell = _descriptionCell;
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

- (BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
{
    if([text isEqualToString:@"\n"])
    {
        [textView resignFirstResponder];
        return NO;
    }
    return YES;
}
// hide the keyboad when type return
-(BOOL)textFieldShouldReturn:(UITextField *)textField{
    if ([textField.text length]) {
        [textField resignFirstResponder];
        return YES;
    }else{
        return NO;
    }
}
// reset the image's size 
-(UIImage*)scaleToSize:(UIImage*)img size:(CGSize)size{
    UIGraphicsBeginImageContext(size);
    [img drawInRect:CGRectMake(0, 0, size.width, size.height)];
    UIImage *scaledImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return scaledImage;
}

-(void)viewWillDisappear:(BOOL)animated{
    self.dream.name = self.NameCell.field.text;
    self.dream.descriotion = self.descriptionCell.textView.text;
}

-(void)hideKeyboard{
    [self.NameCell.field resignFirstResponder];
    [self.descriptionCell.textView resignFirstResponder];
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // allow user tap the baackgroud then hide the keyboad
    UITapGestureRecognizer *gestureRecognizer = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(hideKeyboard)];
    gestureRecognizer.cancelsTouchesInView = NO;
    [self.tableView addGestureRecognizer:gestureRecognizer];
    
    // change the background
    if (iPhone5) {
        UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 586)];
        self.tableView.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];

    }
    else{
        UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 480)];
        self.tableView.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];

    }
    
    // set the bay style 
    self.navigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
    
    // set the title of the navigation bar
    self.navigationItem.title = self.dream.name;
    // set the type
    self.type = self.dream.type;
    // set the dream url
    self.urlOfDream = [NSURL URLWithString:self.dream.url];

    
    // set the name, description and the time
    // name cell
    self.NameCell = [[TKLabelTextFieldCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier: nil];
	self.NameCell.label.text = @"Name";
	self.NameCell.field.text = self.dream.name;
    // description cell
    self.descriptionCell = [[TKLabelTextViewCell alloc] initWithStyle:UITableViewCellStyleDefault  reuseIdentifier:nil];
	self.descriptionCell.label.text = @"Description";
    self.descriptionCell.textView.font = [UIFont boldSystemFontOfSize:14.0];
    self.descriptionCell.textView.text = self.dream.descriotion;
    // time cell
    NSDateFormatter *df = [[NSDateFormatter alloc]init];
    [df setDateFormat:@"MM-dd-h:mm:ss a"];
    TKLabelFieldCell *timeCell = [[TKLabelFieldCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:nil];
	timeCell.label.text = @"Create at";
    timeCell.field.font = [UIFont boldSystemFontOfSize:12.0];
	timeCell.field.text = [df stringFromDate:self.dream.time];
    // add these cell 
    self.cells = [[NSArray alloc]initWithObjects:self.NameCell,self.descriptionCell,timeCell,nil];
    
    // add the type swith bar
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
    //add it to the view
	[self.view addSubview:redSC];
    
    // create new zen player button
    if (iPhone5) {
        redSC.center = CGPointMake(160, 225);
        self.zenPlayerButton = [[ZenPlayerButton alloc] initWithFrame:CGRectMake(200, 325, 95, 95)];

    }
	else{
        redSC.center = CGPointMake(160, 195);
        self.zenPlayerButton = [[ZenPlayerButton alloc] initWithFrame:CGRectMake(200, 260, 95, 95)];
    }
    // listening to tap event on the button
    [self.zenPlayerButton addTarget:self
                             action:@selector(zenPlayerButtonDidTouchUpInside:)
                   forControlEvents:UIControlEventTouchUpInside];
    // add zen player button to the view
    [self.view addSubview:self.zenPlayerButton];
    
    // set the image for the share menu
    UIImage *storyMenuItemImage = [UIImage imageNamed:@"bg-menuitem.png"];
    UIImage *storyMenuItemImagePressed = [UIImage imageNamed:@"bg-menuitem-highlighted.png"];
    UIImage *facebookImage = [UIImage imageNamed:@"facebook.png"];
    UIImage *twitterImage = [UIImage imageNamed:@"twitter.png"];
    UIImage *mailImage = [UIImage imageNamed:@"mail.png"];
    UIImage *weiboImage = [UIImage imageNamed:@"weibo.png"];
    // add the image the tht menu item
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
    // init the menu
    self.menu = [[AwesomeMenu alloc] initWithFrame:self.view.bounds menus:menus];
    if (iPhone5) {
        self.menu.startPoint = CGPointMake(60.0, 375.0);
    }
    else
        self.menu.startPoint = CGPointMake(60.0, 315.0);
	self.menu.menuWholeAngle = M_PI * 0.8;
	self.menu.farRadius = 90.0f;
	self.menu.endRadius = 80.0f;
    self.menu.delegate = self;
    // add the menu to the view
    [self.view addSubview:self.menu];
    
    // add the upload button only if dream have not been upload
    if ([self.dream.uploadURL isEqualToString:@""]) {
        [self addUpladButton];
    }
}
#pragma mark - viem item part

// get the choes form the swith bar
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

// play the dream 
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

-(void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{
    self.zenPlayerButton.state = ZenPlayerButtonStateNormal;
}
#pragma mark - table view part
- (NSInteger)numberOfSectionsInTableView:(UITableView *)tableView {
    return 1;
}
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return [self.cells count];
}
- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	return [self.cells objectAtIndex:indexPath.row];
}

- (CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath{
	if(indexPath.row == 1)
		return 80.0;
	return 40.0;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // not allow select the cell 
    [tableView deselectRowAtIndexPath:[tableView indexPathForSelectedRow] animated:NO];
}
#pragma mark - upload part

-(void)addUpladButton{
    if ([self.dream.uploadURL isEqualToString:@""]) {
        self.uploadButton = [MAConfirmButton buttonWithTitle:@"Upload Dream" confirm:@"Are You Sure?"];
        [self.uploadButton addTarget:self action:@selector(confirmActionOfUpload:) forControlEvents:UIControlEventTouchUpInside];
        if (iPhone5) {
            [self.uploadButton setAnchor:CGPointMake(300, 265)];
        }
        else
            [self.uploadButton setAnchor:CGPointMake(300, 225)];
        [self.view addSubview:self.uploadButton];
    }
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

// get the chose form the by index form the menu
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

#pragma mark - Twitter part
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

#pragma mark - Facebook part
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

#pragma mark - Weibo part
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

#pragma mark - Mail part
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
