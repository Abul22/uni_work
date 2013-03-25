//
//  RecorderViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 27/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "RecorderViewController.h"

@interface RecorderViewController ()
@end

@implementation RecorderViewController
@synthesize recordButton =_recordButton;
@synthesize saveButton;
@synthesize zenPlayerButton = _zenPlayerButton;
@synthesize dreamDatabase = _dreamDatabase;


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
    manager = [NSFileManager defaultManager];
    
    [super viewDidLoad];
    // add the blackgroud image 
    if (iPhone5) {
        UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 586)];
        self.view.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];
    }
    else{
        UIImage * backgroundImgae = [self scaleToSize:[UIImage imageNamed:@"wood.png"] size:CGSizeMake(340, 480)];
        self.view.backgroundColor =[UIColor colorWithPatternImage:backgroundImgae];
    }
    // set the play state HUD
    activityIndicator = [[HZActivityIndicatorView alloc] initWithFrame:CGRectMake(137, 70, 0, 0)];
    activityIndicator.backgroundColor =  self.view.backgroundColor ;
    activityIndicator.opaque = YES;
    activityIndicator.steps = 8;
    activityIndicator.finSize = CGSizeMake(17, 10);
    activityIndicator.indicatorRadius = 15;
    activityIndicator.stepDuration = 0.150;
    activityIndicator.color = [UIColor colorWithRed:127 green:127 blue:127 alpha:1];
    activityIndicator.cornerRadii = CGSizeMake(0, 0);
    [activityIndicator startAnimating];
    
    // the mac button vioce
    NSURL *buttonVioceURL = [[NSBundle mainBundle] URLForResource:@"buttonVioice.caf" withExtension:nil];
    AudioServicesCreateSystemSoundID((__bridge CFURLRef)buttonVioceURL, &soundID);
    
    tempFileURL = [NSURL fileURLWithPath:[NSTemporaryDirectory() stringByAppendingString:@"temp.caf"]];
    
    NSDictionary *recordSetting = [NSDictionary
                                   dictionaryWithObjectsAndKeys:
                                   [NSNumber numberWithInt:AVAudioQualityMin],
                                   AVEncoderAudioQualityKey,
                                   [NSNumber numberWithInt:16],
                                   AVEncoderBitRateKey,
                                   [NSNumber numberWithInt:2],
                                   AVNumberOfChannelsKey,
                                   [NSNumber numberWithFloat:44100.0],
                                   AVSampleRateKey, nil];
    
    
    NSError *error = nil;
    recorder = [[AVAudioRecorder alloc]initWithURL:tempFileURL settings:recordSetting error:&error];
    if (error) {
        NSLog(@"error: %@",[error localizedDescription]);
    }
    else
        [recorder prepareToRecord];
    
    
    // create new zen player button
    if (iPhone5) {
        self.zenPlayerButton = [[ZenPlayerButton alloc] initWithFrame:CGRectMake(108, 325, 104, 104)];

    }
    else
        
        self.zenPlayerButton = [[ZenPlayerButton alloc] initWithFrame:CGRectMake(108, 285, 104, 104)];
    // listening to tap event on the button
    [self.zenPlayerButton addTarget:self
                             action:@selector(zenPlayerButtonDidTouchUpInside:)
                   forControlEvents:UIControlEventTouchUpInside];
    
    [self.view addSubview:self.zenPlayerButton];
    
    self.zenPlayerButton.enabled = NO;
    self.saveButton.enabled = NO;
    
    
}

-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    // get database form the appdelegate
    AppDelegate *appDelegate = (AppDelegate*)[[UIApplication sharedApplication]delegate];
    self.dreamDatabase  = appDelegate.dreamDatabase;
    
}

- (void)viewDidUnload
{
    [self setRecordButton:nil];
    [self setSaveButton:nil];
    [super viewDidUnload];
    NSError *error = nil;
    // remove the tempfile
    [manager removeItemAtURL:tempFileURL error:&error];
    if (error) {
        NSLog(@"error2: %@",[error localizedDescription]);
    }   
}

// start recording
- (IBAction)recording:(id)sender {
    if (!recorder.recording) {
        AudioServicesPlayAlertSound(soundID);
        
        self.zenPlayerButton.enabled = NO;
        self.saveButton.enabled = NO;
        [self.view addSubview:activityIndicator];
        [recorder performSelector:@selector(record) withObject:nil afterDelay:1.];
    }
    else{
        self.zenPlayerButton.enabled = YES;
        self.saveButton.enabled = YES;
        [recorder stop];
        [activityIndicator removeFromSuperview];
    }
}

// save the dream
- (IBAction)saveDream:(id)sender {
    
    NSString *name = [self timeString];
    NSArray *dirPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
    NSString *dreamDir = [dirPaths lastObject];
    NSString *dreamFilePath = [dreamDir stringByAppendingPathComponent:[NSString stringWithFormat:@"%@.caf",name]];
    dreamFileURL = [NSURL fileURLWithPath:dreamFilePath];
    
    if ([[NSFileManager defaultManager]fileExistsAtPath:tempFileURL.path]) {
        NSError *err = nil;
        [manager moveItemAtURL:tempFileURL toURL:dreamFileURL error:&err];
        if (err) {
            [SVProgressHUD showErrorWithStatus:@"temp not exist"];
            NSLog(@"error1: %@",[err localizedDescription]);
        }
        else{
            NSMutableDictionary *dream = [[NSMutableDictionary alloc]init];
            NSDate *date = [NSDate date];
            NSLog(@"the date is %@",date );

            TKDateInformation info = [date dateInformationWithTimeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];

            date = [NSDate dateFromDateInformation:info timeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];
            NSLog(@"the date is %@",date );

            [dream setObject:name forKey:DREAM_NAME];
            [dream setObject:@"" forKey:DREAM_TYPE];
            [dream setObject:@"" forKey:DREAM_DESCRIOTION];
            [dream setObject:date forKey:DREAM_TIME];
            [dream setObject:@"" forKey:DREAM_UPLOADURL];
            [dream setObject:[dreamFileURL absoluteString] forKey:DREAM_URL];
            [Dream dreamWithInfo:dream inManagedObjectContext:self.dreamDatabase.managedObjectContext];
            [SVProgressHUD showSuccessWithStatus:@"Dream Saved!"];
            self.saveButton.enabled = NO;

        }

    }else{
        [SVProgressHUD showErrorWithStatus:@"No Dream!"];
    }
    
    
}

// set the time formate
-(NSString*)timeString{
    NSDateFormatter *df = [[NSDateFormatter alloc]init];
    [df setDateFormat:@"MM-dd-h:mm:ss a"];
    NSString *timeString = [df stringFromDate:[NSDate date]];
    return timeString;
}

-(void)audioRecorderDidFinishRecording:(AVAudioRecorder *)recorder successfully:(BOOL)flag{
    AudioServicesPlayAlertSound(soundID);
}

-(void)audioPlayerDidFinishPlaying:(AVAudioPlayer *)player successfully:(BOOL)flag{
    self.recordButton.enabled = YES;
    self.saveButton.enabled = YES;
    self.zenPlayerButton.state = ZenPlayerButtonStateNormal;
}

// play the dream
- (void) zenPlayerButtonDidTouchUpInside:(id)sender;
{
    if (!player.playing) {
        self.recordButton.enabled = NO;
        self.saveButton.enabled = NO;
        NSError *error = nil;
        if ([[NSFileManager defaultManager]fileExistsAtPath:tempFileURL.path]) {
            player = [[AVAudioPlayer alloc]initWithContentsOfURL:recorder.url error:&error];
        }
        else
            player = [[AVAudioPlayer alloc]initWithContentsOfURL:dreamFileURL error:&error];

            
        player.delegate = self;
        
        if (error) {
            NSLog(@"error: %@",[error localizedDescription]);
        }
        else
            [player play];
        
    }
    else {
        [player stop];
    }
    
}

@end
