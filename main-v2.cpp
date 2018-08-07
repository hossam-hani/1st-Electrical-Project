//
//  main.cpp
//  HossamProject
//
//  Created by Hossam hani on 6/25/18.
//  Copyright © 2018 hossam hani. All rights reserved.
//

#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include<sstream>
#include<math.h>

using namespace std;

// function to get cofficents and varibales
int cutTerm(string term,string &var,float &coff){
    if(term[0] == '='){
        term = term.substr(1,term.size() - 1);
    }
    
    int firstCharPosition = 0;
    int findChar = 0;
    for(int i = 0;i < term.size();i++){
        if( !isdigit(term[i]) && term[i] != '.' && term[i] != '-' && term[i] != '+' ){
            firstCharPosition = i;
            findChar = 1;
            break;
        }
    }
    
    if(findChar){
        var = term.substr(firstCharPosition,term.size() - firstCharPosition);
        string coffStr;
        coffStr = term.substr(0,firstCharPosition);
    
        // special case if coff = 1
        if( (term[0] == '-' || term[0] == '+' ) && ( !isdigit(term[1]) && term[1] != '.' )  ){
            coff = (term[0] == '-') ? -1 : 1;
        }else if(firstCharPosition == 0){
            coff = 1;
        }else{
           coff = stof(coffStr);
        }
    }else{
        var = "Constant";
        if(!term.empty()){
            coff = stof(term);
        }else{
            coff = 0;
        }

    }
    

    //
    
    return 0;
}



int SimplificationEquation(vector<float> &cofficents,vector<string> &variables,vector<string> &positions){
    
    vector<string> oldVariables(variables);
    vector<string> oldPositions(positions);
    vector<float> oldCofficents(cofficents);
    
    
    
    sort(variables.begin(), variables.end());
    variables.erase( unique( variables.begin(), variables.end() ), variables.end() );
    
    cofficents.clear();


    float sumCoffs = 0;
    for(int i =0;i < variables.size();i++){
        
        for(int k = 0;k < oldVariables.size();k++){
            if(variables[i] == oldVariables[k]){
                if(oldPositions[k] == "R" && oldVariables[k] != "Constant"){
                    sumCoffs += oldCofficents[k] * -1;
                }else if(oldPositions[k] == "L" && oldVariables[k] == "Constant"){
                    sumCoffs += oldCofficents[k] * -1;
                }else{
                    sumCoffs += oldCofficents[k] ;
                }
                
            }
        }
        cofficents.push_back(sumCoffs);
        sumCoffs = 0;

    }
    
    positions.clear();
    for(int i = 0;i < variables.size();i++){
        if(variables[i] != "Constant"){
            positions.push_back("L");
        }else{
            positions.push_back("R");
        }
    }
    
    
    return 0;
}





/// function to cut equation
int cutEquation(string equation,vector<float> &cofficents,vector<string> &variables,vector<string> &position){
    
    // variable using inside loop
    vector<string> terms;
    float coff;
    string var;
    string term;
    int lastPosition = 0;
    unsigned long PositionOfEqual = equation.find('=');
    
    
    for(int i = 0;i < equation.size(); i++){
        if(equation[i] == '+' || equation[i] == '-' || equation[i] == '='){
            term = equation.substr(lastPosition,i - lastPosition);

            
            // get coff and var
    
            cutTerm(term,var,coff);
            cofficents.push_back(coff);
            variables.push_back(var);
            
            
            if(PositionOfEqual >= i){
                position.push_back("L");
            }else{
               position.push_back("R");
            }
            
            terms.push_back(term);
            lastPosition = i;
        }
    }
    
    string lastTerm = equation.substr(lastPosition,equation.size() - lastPosition);
    terms.push_back(lastTerm);
    position.push_back("R");
    
    /// get coff and var for last term
    cutTerm(lastTerm,var,coff);
    cofficents.push_back(coff);
    variables.push_back(var);
    
    

    
    SimplificationEquation(cofficents,variables,position);

    
    return 0;
}

string FloatToString(float num){
    string coffStr;
    ostringstream ss;
    ss << num;
    coffStr = ss.str();
    return coffStr;
}

/// print equation
string getEquationStr(vector<float> cofficents,vector<string> variables,vector<string> position){
    string equation;
    for(int i = 0;i < variables.size();i++){
        
        if(variables[i] != "Constant"){
            if(cofficents[i] >= 0 && i != 1){
                // if coff euqal zero
                if(cofficents[i] == 0){
                
                }
                else if(cofficents[i] != 1){
                    equation +=  "+" + FloatToString(cofficents[i]) + variables[i];
                }else{
                    equation +=  "+"  + variables[i];

                }
            }else{
                equation +=  FloatToString(cofficents[i]) + variables[i];
            }
        }
    }
    return equation + "=" + FloatToString(cofficents[0]) ;
}


// add equation
string addEquations(string equation1,string equation2){
    
    vector<string> totalPositions;
    vector<float> totalCoffs;
    vector<string> totalVars;
    
    vector<string> position1;
    vector<float> coffs1;
    vector<string> vars1;
    cutEquation(equation1,coffs1,vars1,position1);

    
    
    vector<string> position2;
    vector<float> coffs2;
    vector<string> vars2;
    cutEquation(equation2,coffs2,vars2,position2);
    
//    for(int i = 0;i < vars1.size();i++){
//        cout << vars1[i] << "," << coffs1[i] << endl;
//    }
    
    totalVars.insert(totalVars.end(), vars1.begin(), vars1.end());
    totalVars.insert(totalVars.end(), vars2.begin(), vars2.end());

    sort(totalVars.begin(), totalVars.end());
    totalVars.erase( unique( totalVars.begin(), totalVars.end() ), totalVars.end() );
    
    float sumCoffs = 0;
    for(int i = 0;i < totalVars.size();i++){
        
        
        // equation 1
        for(int k = 0;k < vars1.size();k++){
            
            if(vars1[k] == totalVars[i]){
                if(position1[k] == "R" && vars1[k] != "Constant"){
                    sumCoffs += coffs1[k] * -1;
                }else if(position1[k] == "L" && vars1[k] == "Constant"){
                    sumCoffs += coffs1[k] * -1;
                }else{
                    sumCoffs += coffs1[k] ;
                }
            }
        }
        

        
        // equation 2
        for(int k = 0;k < vars2.size();k++){
            if(vars2[k] == totalVars[i]){
                if(position2[k] == "R" && vars2[k] != "Constant"){
                    sumCoffs += coffs2[k] * -1;
                }else if(position2[k] == "L" && vars2[k] == "Constant"){
                    sumCoffs += coffs2[k] * -1;
                }else{
                    sumCoffs += coffs2[k] ;
                }
        }
        }

        totalCoffs.push_back(sumCoffs);
        sumCoffs = 0;
    }

    
    for(int i = 0;i < totalVars.size();i++){
        if(totalVars[i] != "Constant"){
            totalPositions.push_back("L");
        }else{
            totalPositions.push_back("R");
        }
    }
    
    
    
    return getEquationStr(totalCoffs,totalVars,totalPositions);
}


string inverseEquation(string equation){
    
    vector<string> positions;
    vector<float> coffs;
    vector<string> vars;
    cutEquation(equation,coffs,vars,positions);
    
    for(int i = 0;i < coffs.size();i++){
        coffs[i] *= -1;
    }
    
    return getEquationStr(coffs,vars,positions);
}





string subtract(string equation1, string equation2){
    string NewEquation2 = inverseEquation(equation2);
    string total = addEquations(equation1,NewEquation2);
    return total;
}

float GetCoffVar(string var,vector<float> coffs,vector<string> vars){
    
    int positionVar = -1;
    for(int i = 0;i < vars.size();i++){
        if(var == vars[i]){
            positionVar = i;
        }
    }
    
    
    if(positionVar != -1){
        return coffs[positionVar];
    }else{
        return 0;
    }
    
}

int multipleVector(float constant,vector<float> &coffs){
    
    for(int i = 0;i < coffs.size();i++){
        coffs[i] = coffs[i] * constant;
    }
    
    return 0;
    
}

string substitute(string var, string equation1, string equation2){
    
    cout << equation1 << endl;
    cout << equation2 << endl;
    
    vector<string> totalPositions;
    vector<float> totalCoffs;
    vector<string> totalVars;
    
    vector<string> position1;
    vector<float> coffs1;
    vector<string> vars1;
    cutEquation(equation1,coffs1,vars1,position1);
    
    
    
    vector<string> position2;
    vector<float> coffs2;
    vector<string> vars2;
    cutEquation(equation2,coffs2,vars2,position2);
    

    
    // equation 1 * coff var in equation 2
    float coffVarEqaution2 = GetCoffVar(var,coffs2,vars2);
    multipleVector(coffVarEqaution2,coffs1);
    
    // equation 2 * coff var in equation 1
    float coffVarEqaution1 = GetCoffVar(var,coffs1,vars1);
    multipleVector(coffVarEqaution1,coffs2);
    
    cout << coffVarEqaution2 << endl << coffVarEqaution1 << endl;
    
    // final String Before subtract
    string equation1Final = getEquationStr(coffs1,vars1,position1);
    string equation2Final = getEquationStr(coffs2,vars2,position2);
    

    
    return  subtract(equation1Final,equation2Final);
}


int getD(vector<string> equations, vector<vector<float>> &Rows,string var = ""){
    
    vector<string> totalVars;
    
    // get TotalVars
    for(int i = 0;i < equations.size();i++){
        
        vector<string> position;
        vector<float> coffs;
        vector<string> vars;
        cutEquation(equations[i],coffs,vars,position);
        
        // insert all vars in total var
        totalVars.insert(totalVars.end(), vars.begin(), vars.end());
    }
    
    // delete Duplicate and sort
    sort(totalVars.begin(), totalVars.end());
    totalVars.erase( unique( totalVars.begin(), totalVars.end() ), totalVars.end() );
    
    
    
    
    // Get Coffs
    for(int i = 0; i < equations.size() ;i++){
        
        vector<string> position;
        vector<float> coffs;
        vector<string> vars;
        cutEquation(equations[i],coffs,vars,position);
        
        vector<float> row;
        for(int v = 0;v < totalVars.size();v++){

            // position of Var
            int Position = -1;
            for(int k = 0; k < vars.size();k++){
                
                    if(vars[k] == totalVars[v] && totalVars[v] != var){
                        Position = k;
                    }else if(totalVars[v] == var && totalVars[v] == vars[k]){
                        Position = 0;
                    }
                
            }

          if(totalVars[v] != "Constant"){
                
                if(Position != -1){
                    row.push_back(coffs[Position]);
                }else{
                    row.push_back(0);
                }
            
           }

        }
    
        
        // insert Row
        Rows.push_back(row);
    }
    

    
    return 0;
}

int printMatrix(vector<vector<float>> Rows){
    for(int i = 0; i < Rows.size();i++){
        for(int k = 0;k < Rows[i].size();k++){
            cout << Rows[i][k] << "\t";
            
        }
        cout << endl;
    }
    
    return 0;
}

double det(int n, double mat[10][10])
{
    
    double d = 0;
    int c, subi, i, j, subj;
    double submat[10][10];
    if (n == 2)
    {
        return( (mat[0][0] * mat[1][1]) - (mat[1][0] * mat[0][1]));
    }
    else
    {
        for(c = 0; c < n; c++)
        {
            subi = 0;
            for(i = 1; i < n; i++)
            {
                subj = 0;
                for(j = 0; j < n; j++)
                {
                    if (j == c)
                    {
                        continue;
                    }
                    submat[subi][subj] = mat[i][j];
                    subj++;
                }
                subi++;
            }
            d = d + (pow(-1 ,c) * mat[0][c] * det(n - 1 ,submat));
        }
    }
    return d;
}

double determinant(vector<vector<float>> Rows,int numberOfEquation){
    int n = numberOfEquation;
    double mat[10][10];
    
    int i, j;
    for(i=0;i<n;i++)
    {
        for(j=0;j<n;j++)
        {
            mat[i][j] = Rows[i][j];
        }
    }

    return det(n,mat);
}


void dump(std::ostream &out, const std::vector<std::string> &v)
{
    for(size_t i = 0; i < v.size(); ++i) {
        out << '\'' << v[ i ] << '\'' << ' ';
    }
    
    out << std::endl;
}

size_t split(const std::string &txt, std::vector<std::string> &strs, char ch)
{
    size_t pos = txt.find( ch );
    size_t initialPos = 0;
    strs.clear();
    
    // Decompose statement
    while( pos != std::string::npos ) {
        strs.push_back( txt.substr( initialPos, pos - initialPos ) );
        initialPos = pos + 1;
        
        pos = txt.find( ch, initialPos );
    }
    
    // Add the last one
    strs.push_back( txt.substr( initialPos, std::min( pos, txt.size() ) - initialPos + 1 ) );
    
    return strs.size();
}


int cutBySpace(string stm,vector<string> &newV){
    vector<string> v;

    split(stm, v, ' ' );
    for(int i = 0;i < v.size();i++){
        if(v[i] != " "){
            newV.push_back(v[i]);
        }
    }

    
    
    return 0;
}

int main(int argc, const char * argv[]) {
    
    
    

    int numberOfEquations;
    vector<string> equations;
    cin >> numberOfEquations;
    cin.ignore();
    

    
    for(int i = 0;i < numberOfEquations;i++){
        string equation;
        getline(cin,equation);
        equations.push_back(equation);
    }
    
    
    string operation;
    getline(cin,operation);
    
    while(operation != "stop"){
        
        if(operation == "num_vars"){
            
            vector<string> totalVars;
            
            for(int i = 0;i < equations.size();i++){
                vector<string> position;
                vector<float> coffs;
                vector<string> vars;
                cutEquation(equations[i],coffs,vars,position);
                
                for(int k = 0; k < vars.size(); k++){
                    totalVars.push_back(vars[k]);
                }
            }
            
            sort(totalVars.begin(), totalVars.end());
            totalVars.erase( unique( totalVars.begin(), totalVars.end() ), totalVars.end() );
            

            
            cout << totalVars.size() - 1;
        }else if(operation.find("equation") !=std::string::npos){
            vector<string> elements;
            cutBySpace(operation,elements);
            
            int numberEquation = atoi(elements[1].c_str());
            
            vector<string> position;
            vector<float> coffs;
            vector<string> vars;
            cutEquation(equations[numberEquation -1],coffs,vars,position);
            
            
            cout << getEquationStr(coffs,vars,position);
            

      }else if(operation.find("column") !=std::string::npos){
          vector<string> elements;
          cutBySpace(operation,elements);
          string var = elements[1];
          
          for(int i = 0; i < equations.size();i++){
              
              vector<string> position;
              vector<float> coffs;
              vector<string> vars;
              cutEquation(equations[i],coffs,vars,position);
              
              for(int k = 0; k < vars.size(); k++){
                  if(vars[k] == var){
                      cout << coffs[k] << endl;
                  }
              }
          }
          
     }else if(operation.find("add") !=std::string::npos){
         
         vector<string> elements;
         cutBySpace(operation,elements);
         int num1 = atoi(elements[1].c_str());
         int num2 = atoi(elements[2].c_str());
         
         
         string equation1 = equations[num1 -1];
         string equation2 = equations[num2 -1];
         

         vector<string> position1;
         vector<float> coffs1;
         vector<string> vars1;
         cutEquation(equation1,coffs1,vars1,position1);
         string finalEquation1 = getEquationStr(coffs1,vars1,position1);

         vector<string> position2;
         vector<float> coffs2;
         vector<string> vars2;
         cutEquation(equation2,coffs2,vars2,position2);
         string finalEquation2 = getEquationStr(coffs2,vars2,position2);


         cout << addEquations(finalEquation1, finalEquation2);
    
    
         
     }else if(operation.find("subtract") !=std::string::npos){

         vector<string> elements;
         cutBySpace(operation,elements);
         int num1 = atoi(elements[1].c_str());
         int num2 = atoi(elements[2].c_str());
         
         
         string equation1 = equations[num1 -1];
         string equation2 = equations[num2 -1];
         
         
         vector<string> position1;
         vector<float> coffs1;
         vector<string> vars1;
         cutEquation(equation1,coffs1,vars1,position1);
         string finalEquation1 = getEquationStr(coffs1,vars1,position1);
         
         vector<string> position2;
         vector<float> coffs2;
         vector<string> vars2;
         cutEquation(equation2,coffs2,vars2,position2);
         string finalEquation2 = getEquationStr(coffs2,vars2,position2);
         
         
         cout << subtract(finalEquation1, finalEquation2);
         
    
     }else if(operation.find("substitute") !=std::string::npos){
         
         
         vector<string> elements;
         cutBySpace(operation,elements);
         string var = elements[1];
         int num1 = atoi(elements[2].c_str());
         int num2 = atoi(elements[3].c_str());
         
         
         string equation1 = equations[num1 -1];
         string equation2 = equations[num2 -1];
         
         
         vector<string> position1;
         vector<float> coffs1;
         vector<string> vars1;
         cutEquation(equation1,coffs1,vars1,position1);
         string finalEquation1 = getEquationStr(coffs1,vars1,position1);
         
         vector<string> position2;
         vector<float> coffs2;
         vector<string> vars2;
         cutEquation(equation2,coffs2,vars2,position2);
         string finalEquation2 = getEquationStr(coffs2,vars2,position2);
         
         
         cout << substitute(var,finalEquation1, finalEquation2);
         
         
     }
     else if(operation == "D"){
         vector<vector<float>> Rows;
         getD(equations,Rows);
         printMatrix(Rows);
         
     }else if(operation == "D_value"){
         
         vector<vector<float>> Rows;
         getD(equations,Rows);
         cout <<  determinant(Rows,numberOfEquations);
     }else if(operation == "solve"){
         
         vector<vector<float>> Rows;
         getD(equations,Rows);
         double D =  determinant(Rows,numberOfEquations);
         
         
         // all variables in Vector
         vector<string> finalVars;
         
         for(int i = 0; i < equations.size();i++){
             for(int k = 0;k < equations[i].size();k++){
                 // first Equation
                 vector<string> vars;
                 vector<float> coffs;
                 vector<string> positions;
//                 cutEquation(equations[i],coffs,vars,position);

                 cutEquation(equations[i],coffs,vars,positions);
                 finalVars.insert(finalVars.end(), vars.begin(), vars.end());
             }
         }
         
         sort( finalVars.begin(), finalVars.end() );
         finalVars.erase( unique( finalVars.begin(), finalVars.end() ), finalVars.end() );
         
         
         for(int i = 0; i < finalVars.size();i++){
             if(finalVars[i] == "Constant"){
                 continue;
             }
             
             vector<vector<float>> Rows;
             getD(equations,Rows, finalVars[i]);
             double DCurrent =  determinant(Rows,numberOfEquations);
             
             cout << finalVars[i] << "=" <<  (DCurrent/D) << endl;
         }
         
         
     }
     else if(operation.find("D") !=std::string::npos){
         vector<string> elements;
         cutBySpace(operation,elements);
         string var = elements[1];
         
         vector<vector<float>> Rows;
         getD(equations,Rows,var);
         printMatrix(Rows);
         
     }
        
        cout << endl;
        getline(cin,operation);
        
    }
    
    
    // insert code here...
//    string equation = "4x1+5x1+2+5a1+1a1+3b1=12-2x1+2x1+7x1+20x1+12-20b1";
//
//    vector<string> position;
//    vector<float> coffs;
//    vector<string> vars;
//    cutEquation(equation,coffs,vars,position);
    
//    vector<string> equations;
//  string eq1 = "2x1+3x2+4x3=16";
//  string eq2 = "1x1+2x2+1x3=8";
//  string eq3 = "3x1+1x2+2x3=13";
//  string var = "x2";
//    equations.push_back(eq1);
//    equations.push_back(eq2);
//    equations.push_back(eq3);
    
//  cout << substitute(var,eq1,eq3);
    
    // getD(equations,"x2");
    vector<vector<float>> Rows;
    getD(equations,Rows);
    printMatrix(Rows);
    int numberOfEquation = 3;
    
    cout << determinant(Rows,numberOfEquation);

    
    
    

    
//    string eq3 = inverseEquation(eq2);
//    string total = addEquations(eq1,eq3);
//    cout << total << endl;
    
    
//    cout << getEquationStr(coffs,vars,position) << endl;
    return 0;
}
