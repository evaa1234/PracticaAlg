#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "Timer.h"
#include "Random.h"
#include <vector>
#include <iostream>
#include <string.h>
#include <fstream>
#include <vector>
#include <set>

// Data structures for the problem data
int n_of_nodes;
int n_of_arcs;
vector< set<int> > neighbors;
set<int> conjunt_D;
// string for keeping the name of the input file
string inputFile;

// dummy parameters as examples for creating command line parameters 
// see function read_parameters(...)
int dummy_integer_parameter = 0;
int dummy_double_parameter = 0.0;


inline int stoi(string &s) {

  return atoi(s.c_str());
}

inline double stof(string &s) {

  return atof(s.c_str());
}

void read_parameters(int argc, char **argv) {

    int iarg = 1;

    while (iarg < argc) {
        if (strcmp(argv[iarg],"-i")==0) inputFile = argv[++iarg];
        
        // example for creating a command line parameter param1 
        //-> integer value is stored in dummy_integer_parameter
        else if (strcmp(argv[iarg],"-param1")==0) 
            dummy_integer_parameter = atoi(argv[++iarg]); 
            
        // example for creating a command line parameter param2 
        //-> double value is stored in dummy_double_parameter
        else if (strcmp(argv[iarg],"-param2")==0) 
            dummy_double_parameter = atof(argv[++iarg]);  
            
        iarg++;
    }
}

/*****************
Funcions auxiliars
******************/

//Retorna el nº de veins de i que pertanyen al conjunt dominador d'influencia positiva D.
int calcula_veins_del_conjunt_D(int i, const set<int>& D){
    int contador = 0;
    for(auto a : neighbors[i]) {
        bool trobat = false;
        set<int>::iterator it;
        for(it = D.begin(); it!=D.end() and not trobat; ++it) {
            if(*it == a) {
                trobat = true;
                ++contador;
            }
        }
    }
    return contador;
}

//Retorna true si el conjunt D donat es un PIDS.
bool checkPIDS(const set<int>& D) {
    bool malament = false;
    int num_veins_del_conjunt_domindaor;
    int num_veins_totals;
    for(int i=1; i<= n_of_nodes and not malament; ++i) {
        num_veins_del_conjunt_domindaor = calcula_veins_del_conjunt_D(i, D);
        num_veins_totals = neighbors[i].size();
        if(num_veins_totals % 2 == 1) ++num_veins_totals;
        if(num_veins_del_conjunt_domindaor < num_veins_totals/2) malament = true;
    }
    if(malament) return false;
    else return true;
}



/************
Main function
*************/

int main( int argc, char **argv ) {

    read_parameters(argc,argv);

    // opening the corresponding input file and reading the problem data
    ifstream indata;
    indata.open(inputFile.c_str());
    if(not indata) { // file couldn't be opened
        cout << "Error: file could not be opened" << endl;
    }

    indata >> n_of_nodes;
    indata >> n_of_arcs;
    neighbors = vector< set<int> >(n_of_nodes+1);
    int u, v;
    int cont = 1;
    while(cont <= n_of_arcs and indata >> u >> v) {
        neighbors[u].insert(v);
        neighbors[v].insert(u);
        ++cont;
    }
    while(indata >> u) {
        conjunt_D.insert(u);
    }
    indata.close();
    
    cout << "Conjunt dominador d'influencia positiva proposat:" << endl;
    for(auto a : conjunt_D) {
        cout << a << " ";
    }
    cout << endl << endl;
    
    cout << "--------------------------------------------------" << endl << endl;
    
    
    //Ara que ja hem llegit totes les dades comprovem si cada vèrtex compleix l'equació.
    //Si tots els vèrtexs la compleixen, el conjunt D donat és un conjunt dominador d'influència positiva.
    bool PIDS;
    PIDS = checkPIDS(conjunt_D);
    if(not PIDS) cout << "El conjunt donat NO es un conjunt dominador d'influencia positiva." << endl;
    //Si es un PIDS, ara mirem si es minim.
    else {
        cout << "El conjunt donat ES un conjunt dominador d'influencia positiva" << endl << endl;
        cout << "--------------------------------------------------" << endl << endl;
        bool minimal = false;
        set<int> vMin;
        set<int>::iterator it;
        for(it = conjunt_D.begin(); it!=conjunt_D.end() and not minimal; ++it) {
            vMin = conjunt_D;
            vMin.erase(*it);
            bool found = checkPIDS(vMin);
            if(found) minimal = true;
        }
        if(minimal) {
            cout << "El conjunt donat NO es minimal ja que el subconjunt:" << endl;
            for(auto a : vMin) {
                cout << a << " ";
            }
            cout << endl << "tambe es un PIDS." << endl;
        }
        else cout << "El conjunt donat es MPIDS." << endl;
    }
    
}



