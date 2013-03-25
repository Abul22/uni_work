//
//  CanlendarTableViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 16/09/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "CanlendarTableViewController.h"

@interface CanlendarTableViewController ()
@end

@implementation CanlendarTableViewController
@synthesize dreamDatabase = _dreamDatabase;
@synthesize dreamArray = _dreamArray;
@synthesize dataArray =_dataArray;
@synthesize dataDictionary = _dataDictionary;


-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    // hide the navigation bar
    self.navigationController.navigationBar.hidden = YES;

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
    self.navigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;
}

- (void)viewDidUnload
{
    [super viewDidUnload];

}



- (NSArray*) calendarMonthView:(TKCalendarMonthView*)monthView marksFromDate:(NSDate*)startDate toDate:(NSDate*)lastDate{
    // get the database form the appDelegate
    AppDelegate *appDelegate = (AppDelegate*)[[UIApplication sharedApplication]delegate];
    self.dreamDatabase  = appDelegate.dreamDatabase;
    // get the data form the database 
    if ([appDelegate useDocumnet]) {
        NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Dream"];
        request.sortDescriptors = [NSArray arrayWithObject:[NSSortDescriptor sortDescriptorWithKey:@"time" ascending:NO]];
        NSError *error;
        self.dreamArray = [self.dreamDatabase.managedObjectContext
                           executeFetchRequest:request error:&error];
    }

    if (self.dreamArray) {
        // set the mark array
        [self generateDreamDataForStartDate:startDate endDate:lastDate];
    }
	return self.dataArray;
}

- (void) calendarMonthView:(TKCalendarMonthView*)monthViewos:(NSDate*)date{
	
	// CHANGE THE DATE TO YOUR TIMEZONE
	TKDateInformation info = [date dateInformationWithTimeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];
	NSDate *myTimeZoneDay = [NSDate dateFromDateInformation:info timeZone:[NSTimeZone systemTimeZone]];
	
	NSLog(@"Date Selected: %@",myTimeZoneDay);
	[self.tableView reloadData];
}

- (void) calendarMonthView:(TKCalendarMonthView*)mv monthDidChange:(NSDate*)d animated:(BOOL)animated{
	[super calendarMonthView:mv monthDidChange:d animated:animated];
	[self.tableView reloadData];
}

- (void) calendarMonthView:(TKCalendarMonthView*)monthView didSelectDate:(NSDate*)date{
    
    [self.tableView reloadData];
}

#pragma mark - Table view data source

- (NSInteger) tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	NSArray *ar = [self.dataDictionary objectForKey:[self.monthView dateSelected]];
	if(ar == nil) return 0;
    return [ar count];
}


- (NSInteger) numberOfSectionsInTableView:(UITableView *)tableView {
	return 1;
	
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Dream Info";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    // Configure the cell...
    if (cell == nil) {
        if (cell == nil) cell = [[UITableViewCell alloc] initWithStyle:UITableViewCellStyleDefault reuseIdentifier:CellIdentifier] ;
    }
    NSArray *array = [self.dataDictionary objectForKey:[self.monthView dateSelected]];
    Dream *dream  =  [array objectAtIndex:indexPath.row];

    cell.textLabel.text = dream.name;
        
    return cell;

}
// set the mark of the date 
- (void) generateDreamDataForStartDate:(NSDate*)start endDate:(NSDate*)end{
	// this function sets up dataArray & dataDictionary
	// dataArray: has boolean markers for each day to pass to the calendar view (via the delegate function)
	// dataDictionary: has items that are associated with date keys (for tableview)
	
	self.dataArray = [NSMutableArray array];
	self.dataDictionary = [NSMutableDictionary dictionary];
	NSDate *d = start;
	while([d compare:end]!=NSOrderedDescending){
        int flag = 0;
        
        NSMutableArray *darray = [[NSMutableArray alloc]init];
        for (NSInteger i = 0; i< [self.dreamArray count];i++ ) {
            Dream *dream = [self.dreamArray objectAtIndex:i];
            if ([[self getDayOfDate:d] isEqualToString:[self getDayOfDate:dream.time]]) {
                [darray addObject:dream];
                [self.dataDictionary setObject:darray forKey:d];
                flag = 1;
            }
        }
        if (flag == 1) {
            [self.dataArray addObject:[NSNumber numberWithBool:YES]];
        }
        else
            [self.dataArray addObject:[NSNumber numberWithBool:NO]];
        
		TKDateInformation info = [d dateInformationWithTimeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];
		info.day++;
		d = [NSDate dateFromDateInformation:info timeZone:[NSTimeZone timeZoneForSecondsFromGMT:0]];
        
	}
	
}

- (NSString *)getDayOfDate:(NSDate *)date{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc]init] ;
    [dateFormatter setDateFormat:@"YYYY-MM-dd"];
    NSString *formattedDateString = [dateFormatter stringFromDate:date];
    return formattedDateString;
}


- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    
    NSArray *ar = [self.dataDictionary objectForKey:[self.monthView dateSelected]];
    Dream *selectDream = [ar objectAtIndex:indexPath.row];
    NewDreamDetailViewController *dreamDetailer = (NewDreamDetailViewController *)[self.storyboard instantiateViewControllerWithIdentifier:@"NewDreamDetail"];
    dreamDetailer.dream = selectDream;
    dreamDetailer.indexPath = indexPath;
    dreamDetailer.delegate = self;
    [self.navigationController pushViewController:dreamDetailer animated:YES];
    self.navigationController.navigationBar.hidden = NO;
}

-(void)dreamDetailViewController:(NewDreamDetailViewController *)sender getNewDreamDetail:(Dream *)dream gotTheIndexOfTheDream:(NSIndexPath *)indexPath{
    NSArray *ar = [self.dataDictionary objectForKey:[self.monthView dateSelected]];
    Dream *dreamToDelete = [ar objectAtIndex:indexPath.row];
    [self.dreamDatabase.managedObjectContext deleteObject:dreamToDelete];
    [self.dreamDatabase.managedObjectContext save:nil];
    [self.dreamDatabase.managedObjectContext insertObject:dream];
    [self.dreamDatabase.managedObjectContext save:nil];
}

@end
