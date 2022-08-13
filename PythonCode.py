from fileinput import filename
from typing import ItemsView
import re
import string
import os


FILENAME = "items.txt"

'''
    Gets the list of lines read in from the file in raw form.
    @param - the filename to read in (must be in same directory as the Source.cpp)
    @return - the list of lines
    '''
def readFile():
    lines = None
    
    with open(FILENAME) as file:
        lines = file.readlines()
    file.close()
    return lines


'''
    Get the inventory read in from the file using the readFile function in Python
    @parameters - none
    @return - dictionary of inventory in the form {item : frequency}
    '''
def getInventory():
    # Dictionary of {item : frequency} 
    inventory = {}
    
    items = readFile()

    # Loop through the list of items
    for item in items:
        item = item.strip() #remove newline character or space

        # Update existing frequency; otherwise add item to dictionary
        if item in inventory:
            inventory[item] += 1 # accumulate to the dictionary item
        else:
            inventory[item] = 1
            
    return inventory


'''
    Get the inventory and display each item with sales data
    @parameters - none
    @return - dictionary of inventory in the form {item : frequency}
    '''   
def determineInventory():
    # Get the inventory from Python function
    inventory = getInventory()
    
    # loop through the dictionary's keys and values
    for key, value in inventory.items():
        print(key + " " + str(value))


'''
    Search the inventory for specific item
    @parameters - user specified item
    @return - number of times item was sold from sales txt file
    '''
def determineFrequency(item):
    itemTotal = -1
   
    try:
        # Get the inventory
        inventory = getInventory()
        itemTotal = inventory[item]
    # catches exception when an item is not in the inventory
    except Exception as ex:
        pass
    return itemTotal 
    

'''
    Similar to determineInventory, but loops through dictionary and writes to frequency.dat file
    @parameters - none
    @return - none, dat file used by the C++ code that calls this function to make histogram of sales data
    '''
def output():
    inventory = getInventory()
    frequencyFile = open("frequency.dat", "w")
    for key in inventory:
        frequencyFile.write(f"{key}: {inventory[key]}")
    frequencyFile.close();
    print("")