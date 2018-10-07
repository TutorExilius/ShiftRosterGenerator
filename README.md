# ShiftRosterGenerator

------

This App generates a shift plan with a fair distribution algorithm, where every slot will be filled by a SINLGE candidate.
App does not support a distribution per slot for more then one candidate! The result (output) will always contain for every data field one candidate only!

The goal is to gerenate a final shift plan, where every slot is assigned to a candidate who asked for that slot.
The distribution algorithm tries to keep a balance between all candidates.

------

Download -> Compile -> Execute

Use:

In Terminal:
EXECUTABLE "inputFile_yourShiftPlanWithCandidates.csv" "outputFile_FinalDistributedShiftPlan.csv"

-> Application will generate a shift plan based on input file.

-> Second argument is optional. If its not provided, result will be printed in standard output stream.

------

The Input CSV File (Table Sheet):

You will find an example input file 'example_shiftplan.csv' in data-dir. That file can be used as template csv input file.

Every Column must seperated by a (;) semicolon!

---

Example:

   ;  Monday        ; Thursday  ; Wednesday
   
10h;  Susan, Paul   ; Tom       ; Martin

11h;  Tim, Hermione ; Lisa      ; Bart 

12h;  Tom, Ben      ; Bart,Lisa ; Harry

---

Explanation of CSV File Structure:

-> First Row of Sheet (CSV File) must be a sheet of Column-Titles.

-> First Column of each row must contain the row-title.

-> the other fields are data fields. There are not allowed data outsite of data fields. Your table data need to be inside of a valid row and column index (valid means, there must exist a column and row title for each data field). Every data field can contain more than one entry (name of candidate, who is asking for that slot). If you want more than one candidate names (maybe there are more than one candidate, who asked for that slot), you have to seperate them by a comma! If no comma found, data will be handled as one name!

------

PS: sorry for my english :) I hope, you will understand this documentation.
