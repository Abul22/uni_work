//
//  ItemDetailTableViewController.m
//  MyshoppingList
//
//  Created by Chao Ruan on 5/08/12.
//  Copyright (c) 2012 Chao Ruan. All rights reserved.
//

#import "ItemDetailTableViewController.h"
#import "ItemManager.h"
@interface ItemDetailTableViewController ()<UITextFieldDelegate>
@property (nonatomic,strong) ItemManager *manager;
@end

@implementation ItemDetailTableViewController
@synthesize manager = _manager;
@synthesize nameTextField = _nameTextField;
@synthesize quantityLable = _quantityLable;
@synthesize descriptionTextField = _descriptionTextField;
@synthesize item = _item;
@synthesize name =_name;
@synthesize description =_description;
@synthesize quantity = _quantity;
@synthesize delegate = _delegate;
@synthesize indexPath = _indexPath;

// make sure the item and index path can be update
-(void)setItem:(NSMutableDictionary *)item{
    if (_item != item) {
        _item = item;
    }
    [self.tableView reloadData];
}

-(void)setIndexPath:(NSIndexPath *)indexPath{
    if (_indexPath != indexPath) {
        _indexPath = indexPath;
    }
}

-(void)setDescription:(NSString *)description{
    if (_description != description) {
        _description =description;
        self.descriptionTextField.text = description;
    }
}

// get the modle
-(ItemManager*)manager{
    if(!_manager) _manager = [[ItemManager alloc]init];
    return _manager;
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
    // set the test field and label
    self.navigationItem.title = self.name;
    self.nameTextField.text = self.name;
    self.descriptionTextField.text = self.description;
    self.quantityLable.text =[self.quantity stringValue];
    // delegate with shopping list table view
    self.nameTextField.delegate = self;
    self.descriptionTextField.delegate = self;
    
    // Uncomment the following line to preserve selection between presentations.
    // self.clearsSelectionOnViewWillAppear = NO;
 
    // Uncomment the following line to display an Edit button in the navigation bar for this view controller.
    // self.navigationItem.rightBarButtonItem = self.editButtonItem;
}
-(void)viewWillAppear:(BOOL)animated{
    [super viewWillAppear:animated];
    // set the name text field become the firest responder
    [self.nameTextField becomeFirstResponder];
}

- (void)viewWillDisappear:(BOOL)animated
{
    [super viewWillDisappear:animated];
    // save all the change when the view disappear
    self.description = self.descriptionTextField.text;
    self.name = self.nameTextField.text;
    self.quantity = [NSNumber numberWithInt:[self.quantityLable.text intValue]];
    self.item = [self.manager setItemDetail:self.name :self.description :self.quantity];
    // delegate the item detail and inder path to shopping list tableview
    [self.delegate itemDetailTableViewController:self gotTheNewItemDetail:self.item gotTheIndexOfTheItem:self.indexPath];
}

- (void)viewDidUnload
{
    [self setNameTextField:nil];
    [self setDescriptionTextField:nil];
    [self setQuantityLable:nil];
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}

- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation
{
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}

#pragma mark - Table view delegate

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // make sure the cell can not be select in itme detail table view
    [tableView deselectRowAtIndexPath:[tableView indexPathForSelectedRow] animated:NO];
}

- (IBAction)TexFieldHideKeybord:(id)sender {
    [sender resignFirstResponder];
}

- (IBAction)changeQuantity:(UIStepper*)sender {
    // change the label of quantity when the stepper have been pressed
    self.quantityLable.text = [NSString stringWithFormat:@"%d", [[NSNumber numberWithDouble:[(UIStepper *)sender value]] intValue]];
    
}

@end

