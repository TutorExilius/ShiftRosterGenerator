# Shift Roster Generator v1.1.2

---
This application has its own Wiki-Page:
https://github.com/TutorExilius/ShiftRosterGenerator/wiki

There you will find a documentation with screenshots.

---


This App generates a shift plan with a fair distribution algorithm, where every slot will be filled by a SINLGE candidate.
App does not support a distribution per slot for more then one candidate! The result (output) will always contain for every data field one candidate only!

The goal is to gerenate a final shift plan, where every slot is assigned to only one of the candidates who asked for that slot.
The distribution algorithm tries to keep a balance between all candidates.


Download -> Compile -> Execute

## Use:

In Terminal:
EXECUTABLE "inputFile_yourShiftPlanWithCandidates.csv" "outputFile_FinalDistributedShiftPlan.csv"

- Application will generate a shift plan based on input file.

- Second argument is optional. If its not provided, result will be printed in standard output stream (which can be also redirected in a file by using pipes)


## The Input CSV File (Table Sheet):

YOU WILL FIND AN EXAMPLE INPUT FILE 'example_shiftplan.csv' in data-dir. That file can be used as template csv input file.

Every Column must seperated by a (;) semicolon!


### Example:

   ;  Monday        ; Thursday  ; Wednesday
   
10h;  Susan, Paul   ; Tom       ; Martin

11h;  Tim, Hermione ; Lisa      ; Bart 

12h;  Tom, Ben      ; Bart,Lisa ; Harry


## Sheet-Structure

Your sheet need a fix defined structure:

- the first line of your sheet must contain the column-titles

- the first column of each tow must contain the row-title

- there aren't allowed data outside the range of your sheet. The column- and row-titles are spanning the table-range. Data are allowed in that spanned range only. Data outside of that range will lead to incorrect results! So make shure, that every data has a column- and a row-title.


#### All Tabulators, Single-Quotes, Double-Quotes, Newlines and SEPERATOR-Characters will be removed in all Data-Columns (Candidate-Values)!

---

PS: sorry for my english :) I hope, you will understand this documentation.

