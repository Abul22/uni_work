//
//  ShoppingListTableViewController.m
//  MyshoppingList
//
//  Created by Chao Ruan on 5/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "ShoppingListTableViewController.h"
#import "ItemManager.h"
#import "ItemDetailTableViewController.h"

@interface ShoppingListTableViewController ()<ItemDetailTableViewControllerDelegate>
@property (nonatomic,strong) ItemManager *manager;
@end

@implementation ShoppingListTableViewController
@synthesize addButton = _addButton;
@synthesize manager = _manager;

// get the modle 
-(ItemManager*)manager
{
    if(!_manager) _manager = [[ItemManager alloc]init];
    return _manager;
}

// add a defalut itme when the add buttion have been pressed
- (IBAction)AddNewItem:(id)sender {
    NSMutableDictionary *item = [self.manager setItemDetail:nil :nil :nil];
   self.manager.itmesArray =  [self.manager addNewItemtoItemArray:item];
    //reload the view when any thing have been changed
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
    // set the button at the navigation bar
    self.navigationItem.leftBarButtonItem = self.editButtonItem;
    self.navigationItem.rightBarButtonItem = self.addButton;
}

- (void)viewDidUnload
{
    [self setAddButton:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - Table view data source


- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    // Return the number of rows in the section.
    return [self.manager.itmesArray count];
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    static NSString *CellIdentifier = @"Item Detail";
    UITableViewCell *cell = [tableView dequeueReusableCellWithIdentifier:CellIdentifier];
    
    // Configure the cell...
    NSMutableDictionary *item = [self.manager.itmesArray objectAtIndex:indexPath.row];
    NSString *title = [item objectForKey:ITEM_NAME];
    // set the defalut item's title "New Item"
    if (title == @"") {
        title = @"New Item";
    }
    // set the cell's title
    cell.textLabel.text  = title;
    return cell;
}


// Override to support conditional editing of the table view.
- (BOOL)tableView:(UITableView *)tableView canEditRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the specified item to be editable.
    return YES;
}



// Override to support editing the table view.
- (void)tableView:(UITableView *)tableView commitEditingStyle:(UITableViewCellEditingStyle)editingStyle forRowAtIndexPath:(NSIndexPath *)indexPath
{
    if (editingStyle == UITableViewCellEditingStyleDelete) {
        // Delete the row from the data source
        
        [self.manager.itmesArray removeObjectAtIndex:indexPath.row];
        [tableView deleteRowsAtIndexPaths:[NSArray arrayWithObject:indexPath]withRowAnimation:UITableViewRowAnimationFade];
      
    }   
    else if (editingStyle == UITableViewCellEditingStyleInsert) {
        // Create a new instance of the appropriate class, insert it into the array, and add a new row to the table view
    }   
}


// Override to support rearranging the table view.
- (void)tableView:(UITableView *)tableView moveRowAtIndexPath:(NSIndexPath *)fromIndexPath toIndexPath:(NSIndexPath *)toIndexPath
{
    NSString *moveItem = [self.manager.itmesArray objectAtIndex:fromIndexPath.row];
    // delete the old one 
    [self.manager.itmesArray removeObjectAtIndex:fromIndexPath.row];
    // add to the new path which arraived
    [self.manager.itmesArray insertObject:moveItem atIndex:toIndexPath.row];
    
}

// Override to support conditional rearranging of the table view.
- (BOOL)tableView:(UITableView *)tableView canMoveRowAtIndexPath:(NSIndexPath *)indexPath
{
    // Return NO if you do not want the item to be re-orderable.
    return YES;
}

-(void)prepareForSegue:(UIStoryboardSegue *)segue sender:(id)sender{
    if ([segue.identifier isEqualToString:@"ShowItem"]) {
        NSIndexPath *indexPath = [self.tableView indexPathForCell:sender];
        NSMutableDictionary *item = [self.manager.itmesArray objectAtIndex:indexPath.row];
        //pass all the item detail which need in  itme detail table view 
        ItemDetailTableViewController *itemer = (ItemDetailTableViewController *)segue.destinationViewController;
        itemer.name = [item objectForKey:ITEM_NAME];
        itemer.description = [item objectForKey:ITEM_DESCRIOTION];
        itemer.quantity = [item objectForKey:ITEM_QUANTITY];
        itemer.indexPath = indexPath;
        itemer.delegate = self;

    }
}
// item table view delegate
-(void)itemDetailTableViewController:(ItemDetailTableViewController *)sender gotTheNewItemDetail:(NSMutableDictionary *)item gotTheIndexOfTheItem:(NSIndexPath *)indexPath{
    //remove the old itme
    [self.manager.itmesArray removeObjectAtIndex:indexPath.row];
    // save the itme which have been changed
    [self.manager.itmesArray insertObject:item atIndex:indexPath.row];
    // reload the view
    [self.tableView reloadData];
}


- (void) setEditing:(BOOL)editing animated:(BOOL)animated{
    // hide the add buttion when start editing
    self.navigationItem.rightBarButtonItem = nil;
    if (editing != self.editing) {
        //over write the editing
        [super setEditing:editing animated:YES];
        [self.tableView setEditing:editing animated:YES];
        
    }
    if(!editing){
        //put add button back when editing end 
        [self.navigationItem setRightBarButtonItem:self.addButton animated:YES];
    }
}
@end
