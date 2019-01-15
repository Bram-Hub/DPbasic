#include<iostream>
#include<vector>
#include<string>
#include<fstream>
#include<time.h>
using namespace std;

//The text file theorems.txt must be in the project folder

//conjunction of disjunction expressed as a vector of conjunctions
//conjunctions stored as vector of Lits        

class Lit{
public:
	string name; 
	bool NOT;//true if of the form NOT A
	Lit::Lit(string aname, bool aNOT){
		name=aname;
		NOT=aNOT;
	};
	Lit::Lit(){
		name="";
		NOT=false;
	};
	Lit::Lit(const Lit &a){
		name=a.name;
		NOT=a.NOT;
	};
};

class node{
public:
	string name;
	bool NOT;
	vector<node*> children;
	bool closed;
	node * parent;
	node::node(string aname, bool anot, node * aparent){
		name=aname;
		NOT=anot;
		parent=aparent;
		closed=false;
	};
};
bool contradiction(node * start, node * parent){
	if(parent->name=="ROOT"){
		return false;
	}
	if(start->name == parent->name && start->NOT != parent->NOT){
		return true;
	}
	else{
		return contradiction(start, parent->parent);
	}
}

//tree satisfiable
bool tree_sat(vector<vector<Lit> > clauses){
	node * root = new node("ROOT", false, NULL);
	vector<node *> current;
	vector<node *> current2;
	current.push_back(root);
	for(int i=0;i<clauses.size();i++){
		for(int j=0;j<clauses[i].size();j++){
			if(current.size()==0){
				return false;
			}
			for(int k=0;k<current.size();k++){
				node * newn= new node(clauses[i][j].name, clauses[i][j].NOT, current[k]);
				current[k]->children.push_back(newn);
				if(contradiction(newn, current[k])){
					newn->closed=true;
				}
				else{
					current2.push_back(newn);
				}
			}
		}
		current=current2;
		current2.clear();
	}
	if(current.size()==0){
		return false;
	}
	return true;
}

//tree satisfiable with some minor optimizations
bool tree_sat2(vector<vector<Lit> > clauses) {
	node * root = new node("ROOT", false, NULL);
	vector<node *> current;
	vector<node *> current2;
	current.push_back(root);
	int clauses_s = clauses.size();
	int clauses_i_s;
	int curr_s;
	for(int i = 0; i < clauses_s; ++i) {
		clauses_i_s = clauses[i].size();
		for(int j = 0; j < clauses_i_s; ++j) {
			curr_s = current.size();
			if(curr_s == 0){
				return false;
			}
			for(int k = 0; k < curr_s; ++k){
				node *newn = new node(clauses[i][j].name, clauses[i][j].NOT, current[k]);
				current[k]->children.push_back(newn);
				if(contradiction(newn, current[k])){
					newn->closed=true;
				} else{
					current2.push_back(newn);
				}
			}
		}
		current = current2;
		current2.clear();
	}
	return (current.size() != 0);
}

//DP satisfiable
bool is_satisfiable(vector<vector<Lit> > clauses){
	if(clauses.size()==0){
        return true;
    }
    Lit * a = new Lit();
    bool is_empty=true;
    for(unsigned int i=0;i<clauses.size();i++){
		if(clauses[i].size()!=0){
            is_empty=false;
            a = &clauses[i][0];
		}
    }
    if(is_empty){return false;}
    vector<vector<Lit> > tr;//if a is true
    vector<vector<Lit> > fa;//if a is false
    
    for(unsigned int i=0;i<clauses.size();i++){
        bool found=false;
        for(unsigned int j=0;j<clauses[i].size();j++){
            if(clauses[i][j].name==a->name ){
                found = true;
                if(clauses[i][j].NOT==a->NOT){
                    clauses[i].erase(clauses[i].begin()+j);
                    tr.push_back(clauses[i]);
                }
                else{
                    clauses[i].erase(clauses[i].begin()+j);
                    fa.push_back(clauses[i]);
                }
            }
        }
        if(!found){
            tr.push_back(clauses[i]);
            fa.push_back(clauses[i]);
        }
    }
    return is_satisfiable(tr)||is_satisfiable(fa);
}

//DP satisfiable 2.0
bool is_satisfiable2(vector<vector<Lit> > clauses){
	//if there are no clauses, or 1 clause which is just a Lit
  if(clauses.size()==0||(clauses.size()==1&&clauses[0].size()==1)){
    return true;
  }
  Lit * a = new Lit();
  bool is_empty=true;
  for(unsigned int i=0;i<clauses.size();i++) {
	if(clauses[i].size()!=0) {
      is_empty=false;
      a = &clauses[i][0];
	}
  }
  if(is_empty){return false;}
  vector<vector<Lit> > tr;//if a is true
  vector<vector<Lit> > fa;//if a is false
    
  for(unsigned int i=0;i<clauses.size();i++){
    bool found=false;
    for(unsigned int j=0;j<clauses[i].size();j++){
      if(clauses[i][j].name==a->name ){
        found = true;
        if(clauses[i][j].NOT==a->NOT){
          clauses[i].erase(clauses[i].begin()+j);
          tr.push_back(clauses[i]);
        } else {
          clauses[i].erase(clauses[i].begin()+j);
          fa.push_back(clauses[i]);
        }
      }
    }
    if(!found){
      tr.push_back(clauses[i]);
      fa.push_back(clauses[i]);
    }
  }
  return is_satisfiable2(tr)||is_satisfiable2(fa);
}

//DP satisfiable 3.0
bool is_satisfiable3(vector<vector<Lit> > clauses){
  unsigned int clauses_s = clauses.size();
	//if there are no clauses, or 1 clause which is just a Lit
  if(clauses_s==0||(clauses_s==1&&clauses[0].size()==1)){
    return true;
  }

  //if there is an empty clause, this constitutes a contradiction.
  for (unsigned int i = 0; i < clauses_s; ++i) {
    if (clauses[i].size() == 0) {
      return false;
    }
  }

  Lit * a = new Lit();
  bool is_empty=true;
  for(unsigned int i=0;i<clauses_s;i++) {
    if(clauses[i].size()!=0) {
      is_empty=false;
      a = &clauses[i][0];
    }
  }
  if(is_empty){return false;}
  vector<vector<Lit> > tr;//if a is true
  vector<vector<Lit> > fa;//if a is false

  for(unsigned int i=0;i<clauses_s;i++){
    bool found=false;
    for(unsigned int j=0;j<clauses[i].size();j++){
      if(clauses[i][j].name==a->name ){
        found = true;
        if(clauses[i][j].NOT==a->NOT){
          clauses[i].erase(clauses[i].begin()+j);
          tr.push_back(clauses[i]);
        } else {
          clauses[i].erase(clauses[i].begin()+j);
          fa.push_back(clauses[i]);
        }
      }
    }
    if(!found){
      tr.push_back(clauses[i]);
      fa.push_back(clauses[i]);
    }
  }
  return is_satisfiable3(tr)||is_satisfiable3(fa);
}

int main(){
	int count=1;//holds the current theorem
	bool complete=false;

	ifstream in;
	in.open("theorems.txt");
	
	ofstream out;
	out.open("output.csv");
	if (!out) {
      cerr << "Error, cannot open output file." << endl;
      return 1;
    }
	
	out<<"Test Num,DP,DP 2.0,DP 3.0,TT,TT 2.0"<<endl;


	//cout<<"Enter your conjunction, \"NOT \" for negation, 0 to go to next conjunction, end to start sat"<<endl;
	while(!complete){
		vector<vector<Lit> > clauses;//consider sorting the clauses beforehand
		//read clauses
		bool done=false;
		string temps;
		vector<Lit> temp;
		
		while(!done){
			in>>temps;
			if(temps == "OVER"){
				complete=true;
				break;
			}
			if(temps == "0"){
				clauses.push_back(temp);
				temp.clear();
			}
			else if(temps == "end"){
				clauses.push_back(temp);
				break;
			}
			else if(temps == "NOT"){
				in>>temps;
				temp.push_back(Lit(temps, true));
			}
			else{
				temp.push_back(Lit(temps, false));
			}
		}
		
		if (!complete) {
            out << count << ",";
                      
			////////////////////////////////////////////////////////////////////
			// Davis-Putnam, without optimizations                            //
			////////////////////////////////////////////////////////////////////
			bool sat;
			clock_t before = clock ();
			for(int i=0;i<1000;i++){
				sat=is_satisfiable(clauses);//C++ is not good enough to measure this small amount of time, so run it 1000 times
			}
			clock_t after = clock ();
			float taken=(float)after-(float)before;
			out<<taken<<",";

			if(sat){
				cout<<count<<" satisfiable"<<endl;
			}
			else{
				cout<<count<<" not satisfiable"<<endl;
			
            }
			
			////////////////////////////////////////////////////////////////////
			// Davis-Putnam, with optimizations                               //
			////////////////////////////////////////////////////////////////////
			bool sat2;
			before = clock ();
			for(int i=0;i<1000;i++){
				sat2=is_satisfiable2(clauses);//C++ is not good enough to measure this small amount of time, so run it 1000 times
			}
			after = clock ();
			taken=(float)after-(float)before;
			out<<taken<<",";
			if(sat2){
				cout<<count<<" satisfiable"<<endl;
			}
			else{
				cout<<count<<" not satisfiable"<<endl;
			}

			////////////////////////////////////////////////////////////////////
			// Davis-Putnam, with optimizations                               //
			////////////////////////////////////////////////////////////////////
			bool sat_dp3;
			before = clock ();
			for(int i=0;i<1000;i++){
				sat_dp3=is_satisfiable3(clauses);//C++ is not good enough to measure this small amount of time, so run it 1000 times
			}
			after = clock ();
			taken=(float)after-(float)before;
			out<<taken<<",";
			if(sat_dp3){
				cout<<count<<" satisfiable"<<endl;
			}
			else{
				cout<<count<<" not satisfiable"<<endl;
			}
			
			////////////////////////////////////////////////////////////////////
			// Truth-Tree method, without optimization                        //
			////////////////////////////////////////////////////////////////////
			bool sat3;
			before = clock ();
			for(int i=0;i<1000;i++){
				sat3=tree_sat(clauses);//C++ is not good enough to measure this small amount of time, so run it 1000 times
			}
			after = clock ();
			taken=(float)after-(float)before;
			out<<taken<<",";
			if(sat3){
				cout<<count<<" satisfiable"<<endl;
			}
			else{
				cout<<count<<" not satisfiable"<<endl;
			}
			
			////////////////////////////////////////////////////////////////////
			// Truth-Tree method, with optimization                           //
			////////////////////////////////////////////////////////////////////
			bool sat4;
			before = clock ();
			for(int i=0;i<1000;i++){
				sat4=tree_sat2(clauses);//C++ is not good enough to measure this small amount of time, so run it 1000 times
			}
			after = clock ();
			taken=(float)after-(float)before;
			out<<taken<<",";
			if(sat4){
				cout<<count<<" satisfiable"<<endl;
			}
			else{
				cout<<count<<" not satisfiable"<<endl;
			}
			
			count++;
			
			out << endl;
		}
	}
	in.close();
	out.close();

  system("PAUSE");

	return 0;
}
