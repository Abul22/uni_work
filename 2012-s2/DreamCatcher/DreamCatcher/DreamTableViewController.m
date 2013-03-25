//
//  DreamTableViewController.m
//  DreamCatcher
//
//  Created by Chao Ruan on 22/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "DreamTableViewController.h"


@interface DreamTableViewController ()

@end

@implementation DreamTableViewController
@synthesize dreamDatabase = _dreamDatabase;

-(void)setupFetchedResultsController{
    //send the request, get the dream form the database 
    NSFetchRequest *request = [NSFetchRequest fetchRequestWithEntityName:@"Dream"];
    
    //sort the dram by time
    request.sortDescriptors = [NSArray arrayWithObject:[NSSortDescriptor sortDescriptorWithKey:@"time" ascending:NO]];
    
    //input the data to the fetchedResultsController
    self.fetchedResultsController = [[NSFetchedResultsController alloc]initWithFetchRequest:request managedObjectContext:self.dreamDatabase.managedObjectContext sectionNameKeyPath:nil cacheName:nil];
}

// check the database state before use the database
-(void)useDocument{
    if (![[NSFileManager defaultManager]fileExistsAtPath:[self.dreamDatabase.fileURL path]]) {
        [self.dreamDatabase saveToURL:self.dreamDatabase.fileURL forSaveOperation:UIDocumentSaveForCreating completionHandler:^(BOOL success){
            [self setupFetchedResultsController];        }];
        
    }else if(self.dreamDatabase.documentState == UIDocumentStateClosed){
        [self.dreamDatabase openWithCompletionHandler:^(BOOL success){
  [self setupFetchedResultsController];
        }];
    }else if (self.dreamDatabase.documentState == UIDocumentStateNormal){
  [self setupFetchedResultsController];
    }
    
}
-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    //get the database form the AppDelegate
    AppDelegate *appDelegate = (AppDelegate*)[[UIApplication sharedApplication]delegate];
    self.dreamDatabase  = appDelegate.dreamDatabase;
    // use the database
    [self useDocument];
    // reload the view
    [self.tableView reloadData];

}

- (id)initWithStyle:(UITableViewStyle)style
{
    self = [super initWithStyle:style];
    if (self) {
        // Custom initialization
    }
    return self;
}

- (void)viewDidLoad
{
    [super viewDidLoad];
    // set the Edit Button
    self.navigationItem.rightBarButtonItem = self.editButtonItem;

    //set the barStyle
    self.navigationController.navigationBar.barStyle = UIBarStyleBlackTranslucent;


}

#pragma mark - Table view data source

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Dream Cell";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    // set the dream in the cell
    Dream *dream = [self.fetchedResultsController objectAtIndexPath:indexPath];
    cell.textLabel.text = dream.name;
    
    return cell;
}

// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // allow editing
    return YES;
}

// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        //begin
        [self.tableView beginUpdates];
        Dream *dreamToDelete = [self.fetchedResultsController objectAtIndexPath:indexPath];
        // remove the audio file
        NSError *error = nil;
        [manager removeItemAtURL:[NSURL URLWithString:dreamToDelete.url] error:&error];
        [self.dreamDatabase.managedObjectContext deleteObject:dreamToDelete];
        // Delete the data from the coredata
        [self.dreamDatabase.managedObjectContext save:nil];
        //end
        [self.tableView endUpdates];
        
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if ([segue.identifier isEqualToString:@"Show Dream"]) {
        // use segue pass the data to the NewDreamDataView
        NSIndexPath *indexPath = [self.tableView indexPathForCell:sender];
        Dream *selectDream = [self.fetchedResultsController objectAtIndexPath:indexPath];
        NewDreamDetailViewController *dreamDetailer = (NewDreamDetailViewController *)segue.destinationViewController;
        dreamDetailer.dream = selectDream;
        dreamDetailer.indexPath = indexPath;
        dreamDetailer.delegate = self;
    }
}

-(void)dreamDetailViewController:(NewDreamDetailViewController *)sender getNewDreamDetail:(Dream *)dream gotTheIndexOfTheDream:(NSIndexPath *)indexPath{
    // get the data from the data NewDreamDetailViewController
    Dream *dreamToDelete = [self.fetchedResultsController objectAtIndexPath:indexPath];
    [self.dreamDatabase.managedObjectContext deleteObject:dreamToDelete];
    [self.dreamDatabase.managedObjectContext save:nil];
    [self.dreamDatabase.managedObjectContext insertObject:dream];
    [self.dreamDatabase.managedObjectContext save:nil];
}


@end
