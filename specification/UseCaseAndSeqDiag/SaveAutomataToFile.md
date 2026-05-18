## Save finite automata to file

1. Description: 
    + The user saves the current state of the canvas to a file.
2.  Actor:
    + The User 
3. Precondition: 
    + The program is running.
    + At least one finite automaton is created.
4. Postcondition: 
    + A new .fat file containing the canvas is created which can be loaded into the program. 
5. Event flow:
    + The user presses the File tab.
    + The user clicks "Save".
    + The program verifies that the canvas isn't empty.
    + The user is asked to set a name for the save file.
    + A new .fat file is created and the current state of the canvas is written into it.
6. Alternative flow:
    + The canvas is empty 
    + The user is warned that a blank canvas cannot be saved and no files are created.

#

![SaveAutomataToFileSeqDiag](../img/SaveAutomataToFileSeqDiag.png)
