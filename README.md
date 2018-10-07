# ShiftRosterGenerator

App generates a shift plan, with a fair distribution algorithm where every slot will be filled by a SINLGE candidate.
App does not support a distribution per slot for more then one candidate!

The goal is to gerenate a final shift plan, where every slot is assigned to a candidate who asked for that slot.
The distribution algorithm tries to keep a balance between all candidates.

Download -> Compile -> Execute

In Terminal:
EXECUTABLE "inputFile_yourShiftPlanWithCandidates.csv" "outputFile_FinalDistributedShiftPlan.csv"

Application will generate a shift plan based on input file.

You will find an example input file 'example_shiftplan.csv' in data-dir. That file can be used as template csv input file.

Second argument is optional. If its not provided, result will be printed in standard output stream.
