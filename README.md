# DPbasic
## Authors
2011:
Justin Lipton
Mike Stark

## About
The contents of the project are as follows:

DPbasic.cpp
  C++ code containing the Davis Putnam and Truth Tree methods.  These methods accept a list of disjunctive clauses and return true if the clauses are satisfiable, false if not satisfiable.  The code also contains basic I/O for reading from theorems.txt and writing to output.csv.
  
DPbasic.exe
  The executable file running our project.
  
Notable Cases.txt
  Some basic thoughts regarding patterns observed in the output.  These are speculative, and may require further research and testing.
  
output.csv
  Output file of DPbasic.exe, which contains the times for each algorithm on each theorem.  note that these times are measured in computer cycles, not seconds.
  
README.txt
  This explanation file.
  
Results.xlsx
  Compiled results of running the code on a T61 student laptop and a custom built computer with an Intel i5 quad core processor.
  
theorems.txt
  The input file containing a list of clauses in conjunctive normal form.  The input format is as follows.  Each line represents a clause, and is a list of literals (either a name, such as 'A', or the keyword 'NOT' followed by a name).  Each clause is separated by a 0, and a newline for ease of reading.  Each theorem is separated by the keyword 'end'.  The final keyword in the file is 'OVER', indicating that the end of the input has been reached.  For example, the CNF argument "(A or B) and (C or D) and (not A or not C)" would be represented by:
    A B 0
    C D 0
    NOT A NOT C
    end
