## Execute operations on automata

1. Description: The user sets operations (minimisation, determinisation, intersect, difference, complement, union) that are implemented on an automaton.
2. Actor:
	+ The User 
3. Precondition: 
	+ The program is running and "Operations" tab is selected.
	+ At least one finite state automaton is created.
4. Postcondition: 
	+ A modified automata (automaton) is placed on the scene.
5. Event flow:
	+ The user selects on a canvas an automaton that the operations should be done on.
	+ The user selects which operation is to be done by checking checkboxes with operation names attached to them.
	+ The program verifies if the number of selected finite state automata is enough for an operation to be done.
		+ If only one finite state automaton is selected and the operation chosen is either union, intersection or difference, then the chosen operation could not be done and use case advances to next step.
		+ If there are no more finite state automata to be selected for that operation to be completed, use case advances to alternative flow A2.
		+ If the number of selected finite state automata is appropriate (1 for complement, determinisation and minimisation, 2 for union, interesection and difference), use case proceeds.
	+ Selected operations are done.
	+ The program draws a finite state automaton that represents the result of a selected operation.
6. Alternative flow:
	+ A1. Unexpected exit of the application - If the program is suddenly closed, all saved information are discarded and use case ends.
	+ A2. Not enough finite state automata are created - The user is warned that there are not enough finite state automata created for that operation to be completed and is prompted to return to "Creation" tab where a new finite state automaton should be created. After the creation, use case goes back to beginning.
		
##

![Diagram](../img/ExecuteOperationsSeqDiag.png)
