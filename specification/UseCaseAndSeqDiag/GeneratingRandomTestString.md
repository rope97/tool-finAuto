## Generating random test string
1. Description    
    + User can randomly generate or type a string used to test his automata  
2. Actors
    + User
3. Precondition
    + Application is running
    + "Testing" tab is open
    + Automata is created and selected
4. Postcondition
    + User has a string which is usable in testing automata
5. Standard process
    + User presses one of the string generating buttons
        + If "Generate random string" button was pressed application will generate completely random string comprised of letters in selected automata's alphabet 
        + If "Generate acceptable string" button was pressed application will generate random string comprised of letters in selected automata's alphabet that results in successful automata traversal
        + If "Generate unacceptable string" button was pressed application will generate random string comprised of letters in selected automata's alphabet that results in failed automata traversal
    + User proceeds to test given string (See 'Testing random string' diagram)
6. Alternative processes
    + User enters the test string manually
    + Application checks if entered string is valid (comprised only of selected automata's alphabet) and warns user if it's not

#

![GeneratingRndStringSeqDiag](../img/GeneratingRandomTestStringSeqDiag.png)