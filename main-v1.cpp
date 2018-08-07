

#include <iostream>
#include <string>
#include <vector>
#include <ctype.h>
#include <algorithm>
#include<sstream>
#include<math.h>

using  namespace std;



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




int cutEauation(string equation,vector<string> &termsBox){
    unsigned long firstPosition = 0;
    unsigned long secondPoistion = 0;
    unsigned long lengthOfTerm = 0;
    unsigned long NP,NN,NE;
    
    
    // cut equation if found + or - or =
    while(equation.find("+",firstPosition) != std::string::npos
          || equation.find("-",firstPosition) != std::string::npos
          || equation.find("=",firstPosition) != std::string::npos){
 
        
            // get posotion
            NP = equation.find("+",firstPosition);
            NN = equation.find("-",firstPosition);
            NE = equation.find("=",firstPosition);
            if(NP < NN){
                secondPoistion = NP;
            }else if(NP > NN){
                secondPoistion = NN;
            }else{
               secondPoistion = NE;
            }
        

        // cut equation
        // back one step if index != 0
        firstPosition = (firstPosition == 0) ? firstPosition : firstPosition - 1;
        
        lengthOfTerm = secondPoistion - firstPosition; // get length of term
        termsBox.push_back(equation.substr(firstPosition,lengthOfTerm));
     
        
        //cout << equation.substr(firstPosition,lengthOfTerm) << endl;
        
        firstPosition = secondPoistion + 1;
        
    }
    
    // get constatnt term
    NE = equation.find("=");
    termsBox.push_back( equation.substr(NE+1,equation.length()- NE) );

    return 0;
}


int getPositionOfChar(string term){
    int position = -1;
    for(int i = 0;i < term.length();i++){
        if(!isdigit(term[i]) && term[i] != '+'  && term[i] != '-' &&  term[i] != '.'){
            position = i;
            break;
        }
    }
    return position;
}

int seperateTerm(string term,float &num,string &var){
    
    int firstPosition = getPositionOfChar(term);

    
    if(firstPosition != -1){
        unsigned long lengthOfVar = term.length() - firstPosition;
        string varTerm = term.substr(firstPosition,lengthOfVar);
        string numTerm = term.substr(0,firstPosition);
        
        // convert from string to float
        num = strtof((numTerm).c_str(),0);
        
        if(num == 0 && numTerm != "0"){
            if(term[0] == '+'){
                num = 1;
            }else if(term[0] == '-'){
                num = -1;
            }
        }
        
        if(numTerm.length() == 0 && varTerm[0] != '-'){
            num = 1;
        }else if(numTerm.length() == 0 && varTerm[0] == '-'){
            num = -1;
        }
        
        var = varTerm;
        
    }else{
        // convert from string to float
        num = strtof((term).c_str(),0);
        var = "";
    }

    
    return 0;
}




int simEquation(vector<string> terms,vector<float> &coefficients,vector<string> &vars){
    
    float num,tempNum;
    string var,tempVar;
    int errorFix = 1;

    
    for(int i = 0;i < terms.size();i++){
        
        seperateTerm(terms[i],num,var);
        
        if(!(find(vars.begin(), vars.end(), var) != vars.end())){
            // put var in vars vector
            vars.push_back(var);
        
            for(int k =0;k < terms.size();k++){
                if(k != i){
                    seperateTerm(terms[k],tempNum,tempVar);
                    if(var == tempVar){
                        
                        // if found constant "always be last element in vector"
                        if(k == terms.size() -1){
                            //cout << "bla";
                            tempNum = tempNum * -1;
                            errorFix = -1;
                        }else{
                            errorFix = -1;
                        }
                        
                    
                        num = num + tempNum;
                       
                    }
                    
                }
            }
        
           
        
            // before end first loop
            if(var.empty()){
                // put var in vars vector
                coefficients.push_back(num  );
            }else{
                // put var in vars vector
                coefficients.push_back(num);
            }
            
        
    }
        
        
    }
    

    
    return 1;
}


int getAlphabetic(vector<float> &coefficients,vector<string> &vars){
    
    for(int i =0;i < vars.size();i++){
        for(int k =i;k < vars.size();k++){
            if(vars[k] < vars[i]){
                // swap vars
                string temp = vars[i];
                vars[i] = vars[k];
                vars[k] = temp;
                
                // swap coffs
                float tempF = coefficients[i];
                coefficients[i] = coefficients[k];
                coefficients[k] = tempF;
                
            }
        }
    }
    
    
    return 0;
}

string FloatToString(float num){
    string coffStr;
    ostringstream ss;
    ss << num;
    coffStr = ss.str();
    return coffStr;
}


string properForm(string equation,vector<float> &coffs,vector<string> &vars){
    
    vector<string> terms;
    cutEauation(equation,terms);

    simEquation(terms,coffs,vars);
    getAlphabetic(coffs,vars);
    string equationStr;
    for(int i =1;i < coffs.size();i++){
        
        if(coffs[i] > 0 && i != 1){
            equationStr += "+"  + FloatToString(coffs[i]) + vars[i];
        }else{
            equationStr += FloatToString(coffs[i]) + vars[i];
        }
        
        
    }
    
     equationStr += "="  + FloatToString(coffs[0]);

    return equationStr;
}





string VectorToPro(vector<float> coffs,vector<string> vars){
    
    string equationStr;
    
    for(int i =1;i < coffs.size();i++){
        
        if(coffs[i] > 0 && i != 1){
            if(coffs[i] == 1){
                equationStr += "+"  + vars[i];
            }else{
                equationStr += "+"  + FloatToString(coffs[i]) + vars[i];
            }
        }else if(coffs[i] == 0){
            
        }else{
            equationStr += FloatToString(coffs[i]) + vars[i];
        }
        
        
    }
    
    equationStr += "="  + FloatToString(coffs[0]);
    
    return equationStr;
}





int addEqs(string eq1,string eq2,vector<float> &finalCoffs,vector<string> &finalVars,int consNum = 1){
    
    //add
    vector<string> vars1;
    vector<float> coffs1;
    properForm(eq1,coffs1,vars1);
   
    vector<string> vars2;
    vector<float> coffs2;
    properForm(eq2,coffs2,vars2);
    
    
    vector<string> varsSum;
    vector<float> coffsSum1;
    vector<float> coffsSum2;
    varsSum.insert(varsSum.end(), vars1.begin(), vars1.end());
    varsSum.insert(varsSum.end(), vars2.begin(), vars2.end());
    
    sort( varsSum.begin(), varsSum.end() );
    varsSum.erase( unique( varsSum.begin(), varsSum.end() ), varsSum.end() );
    
    for(int i =0;i < varsSum.size();i++){
        
        int foundInvar1 = 0;
        for(int k = 0;k < vars1.size();k++){
            if(varsSum[i] == vars1[k]){
                coffsSum1.push_back(coffs1[k]);
                foundInvar1 = 1;
            }
        }
        
        int foundInvar2 = 0;
        for(int k = 0;k < vars2.size();k++){
            if(varsSum[i] == vars2[k]){
                coffsSum2.push_back(coffs2[k]);
                foundInvar2 = 1;
            }
        }
        
        if(foundInvar1 == 0){
            coffsSum1.push_back(0);
        }
        
        if(foundInvar2 == 0){
            coffsSum2.push_back(0);
        }
        
        foundInvar1 = 0;
        foundInvar2 = 0;
        
    } // end for
    
    vector<float> coffTotal;
    
    
    float totalNum;
    for(int i =0;i < coffsSum1.size();i++){
        totalNum = coffsSum1[i] + coffsSum2[i] * consNum;
        coffTotal.push_back(totalNum);
    }
    
    finalCoffs.insert(finalCoffs.end(), coffTotal.begin(), coffTotal.end());
    finalVars.insert(finalVars.end(), varsSum.begin(), varsSum.end());

    
    return 0;
}



int getEqByVar(string equation,string var,vector<float> &finalCoffs,vector<string> &finalVars){
    
    vector<string> vars1;
    vector<float> coffs1;
    string eq1 = properForm(equation,coffs1,vars1);
    
    int positionVar = 0;
    for(int i =0;i < vars1.size();i++){
        if(vars1[i] == var){
            positionVar = i;
        }
    }
    cout << positionVar << endl;
    
    for(int i = 0;i < vars1.size();i++){
        if(i != positionVar && i != 0){
            coffs1[i] = (coffs1[i] * -1)/coffs1[positionVar];
        }else if(i == 0){
            coffs1[i] = (coffs1[i])/coffs1[positionVar];
        }
    }
    
    coffs1.erase(coffs1.begin() + positionVar);
    vars1.erase(vars1.begin() + positionVar);
    
    
    finalCoffs.insert(finalCoffs.end(), coffs1.begin(), coffs1.end());
    finalVars.insert(finalVars.end(), vars1.begin(), vars1.end());

    
    return 0;
}

int main(int argc, const char * argv[]) {


    
    int numberOfEquations;
    
    vector<string> eqs;

    
    cin >> numberOfEquations;
    cin.ignore();
    for(int i = 0;i < numberOfEquations;i++){
        string equation;
        getline(cin,equation);
        eqs.push_back(equation);
    }

    string operation;
    getline(cin,operation);
    
    while(operation != "stop"){
        
        if(operation == "num_vars"){
            vector<vector<float>> coffsEq;
            vector<vector<string>> varsEq;
            vector<string> varsAllInOne;
            for(int i = 0;i < eqs.size();i++){
                vector<string> vars;
                vector<float> coffs;
                string eq = properForm(eqs[i],coffs,vars);
                
                coffsEq.push_back(coffs);
                varsEq.push_back(vars);
                
                    for(int k =0;k < coffs.size(); k++){
                        if(!vars[k].empty()){
                            varsAllInOne.push_back(vars[k]);
                        }
                    }
            }
            
            sort( varsAllInOne.begin(), varsAllInOne.end() );
            varsAllInOne.erase( unique( varsAllInOne.begin(), varsAllInOne.end() ), varsAllInOne.end() );

            cout << varsAllInOne.size() -1;
            operation = "";
        }else if(operation.find("equation") !=std::string::npos){
            unsigned long firstPosition = operation.find(" ");
            string numEqStr = operation.substr(firstPosition,operation.length()-firstPosition);
            int numEq = atoi(numEqStr.c_str());
            
            vector<string> vars;
            vector<float> coffs;
            cout << properForm(eqs[numEq-1],coffs,vars);
        }else if(operation.find("column") !=std::string::npos){
            
            unsigned long firstPosition = operation.find(" ");
            string ColStr = operation.substr(firstPosition+1,operation.length()-firstPosition);
            
            vector<vector<float>> coffsEq;
            vector<vector<string>> varsEq;
            for(int i = 0;i < eqs.size();i++){
                vector<string> vars;
                vector<float> coffs;
                string eq = properForm(eqs[i],coffs,vars);
                
                for(int k=0;k < vars.size();k++){
                    if(vars[k] == ColStr){
                        cout << coffs[k] << endl;
                    }
                }
        
//                coffsEq.push_back(coffs);
//                varsEq.push_back(vars);
            }
            
            
            
        }else if(operation.find("add") !=std::string::npos){
            
            /// get number of equations
            unsigned long firstPosition = operation.find(" ");
            string numEqs = operation.substr(firstPosition+1,operation.length()-firstPosition);
            firstPosition = numEqs.find(" ");
            string numEqs1Str = numEqs.substr(0,firstPosition);
            string numEqs2Str = numEqs.substr(firstPosition+1,numEqs.length()-firstPosition);
            int numEqs1 = atoi(numEqs1Str.c_str());
            int numEqs2 = atoi(numEqs2Str.c_str());
            
            //  add
            string equation1 = eqs[numEqs1-1];
            string equation2 = eqs[numEqs2-1];
            vector<float> finalCoffs;
            vector<string> finalVars;
            
            addEqs(equation1,equation2,finalCoffs,finalVars);
            
            cout << VectorToPro(finalCoffs,finalVars);

        }else if(operation.find("subtract ") !=std::string::npos){
            
            /// get number of equations
            unsigned long firstPosition = operation.find(" ");
            string numEqs = operation.substr(firstPosition+1,operation.length()-firstPosition);
            firstPosition = numEqs.find(" ");
            string numEqs1Str = numEqs.substr(0,firstPosition);
            string numEqs2Str = numEqs.substr(firstPosition+1,numEqs.length()-firstPosition);
            int numEqs1 = atoi(numEqs1Str.c_str());
            int numEqs2 = atoi(numEqs2Str.c_str());
            
            
            
            
            //subtract
            string equation1 = eqs[numEqs1-1];
            string equation2 = eqs[numEqs2-1];
            vector<float> finalCoffs;
            vector<string> finalVars;
            
            addEqs(equation1,equation2,finalCoffs,finalVars,-1);
            
            cout << VectorToPro(finalCoffs,finalVars);
           
            
            
            
        }else if(operation.find("substitute") !=std::string::npos){
            
            /// get number of equations
            unsigned long firstPosition = operation.find(" ");
            string numEqs = operation.substr(firstPosition+1,operation.length()-firstPosition);
            firstPosition = numEqs.find(" ");
            
            string var = numEqs.substr(0,firstPosition);
            string numbersEqs = numEqs.substr(firstPosition+1,numEqs.length()-firstPosition);
            firstPosition = numbersEqs.find(" ");
            string Eqs1 = numbersEqs.substr(0,firstPosition);
            string Eqs2 = numbersEqs.substr(firstPosition+1,numbersEqs.length());
            int numEqs1 = atoi(Eqs1.c_str());
            int numEqs2 = atoi(Eqs2.c_str());


     
            
            // get equations
            string firstEquation = eqs[numEqs1-1];
            string scondeEquation = eqs[numEqs2-1];
            
            // first Equation
            vector<string> vars1;
            vector<float> coffs1;
            properForm(firstEquation,coffs1,vars1);
            
            // Second Equation
            vector<string> vars2;
            vector<float> coffs2;
            properForm(scondeEquation,coffs2,vars2);

            // get Cofficent of Equation 1
            float CoffEquation1 = 0;
            for(int i = 0;i < vars1.size();i++){
                if(vars1[i] == var){
                    CoffEquation1 = coffs1[i];
                }
            }
            
            // get Cofficent of Equation 2
            float CoffEquation2 = 0;
            for(int i = 0;i < vars2.size();i++){
                if(vars2[i] == var){
                    CoffEquation2 = coffs2[i];
                }
                
            }
            
            //  equation1 * Cofficent of Equation 2
            for(int i = 0;i <coffs1.size();i++){
                coffs1[i] = coffs1[i] * CoffEquation2;
            }
            
            //  equation1 * Cofficent of Equation 2
            for(int i = 0;i <coffs2.size();i++){
                coffs2[i] = coffs2[i] * CoffEquation1 ;
            }
            
            //

            
            //subtract eq1 - eq2
            string eq1 = VectorToPro(coffs1,vars1);
            string eq2 = VectorToPro(coffs2,vars2);

            vector<float> finalCoffs;
            vector<string> finalVars;
            addEqs(eq1,eq2,finalCoffs,finalVars,-1);
            
            // print final equation
            cout << VectorToPro(finalCoffs,finalVars);
            
            
        }else if(operation == "D" || operation == "D_value" || operation == "solve"){
            
            // Rows (Given To Function) (outPut)
            vector<vector<float>> Rows;

            ///////// start Get D /////////////
            
            // all variables in Vector
            vector<string> finalVars;
            
            for(int i = 0; i < eqs.size();i++){
                for(int k = 0;k < eqs[i].size();k++){
                    // first Equation
                    vector<string> vars;
                    vector<float> coffs;
                    properForm(eqs[i],coffs,vars);
                    finalVars.insert(finalVars.end(), vars.begin(), vars.end());
                }
            }
            
            sort( finalVars.begin(), finalVars.end() );
            finalVars.erase( unique( finalVars.begin(), finalVars.end() ), finalVars.end() );
            
            for(int s = 0; s < eqs.size();s++){
                // first Equation
                vector<string> vars;
                vector<float> coffs;
                vector<float> finalCoffs;
                properForm(eqs[s],coffs,vars);
                
                // insert equation in vector
                for(int i = 0;i < finalVars.size();i++){
                    int PositionOfVar = -1;
                    for(int k = 0;k < vars.size();k++){
                        if(finalVars[i] == vars[k]){
                            PositionOfVar = k;
                        }
                    }
                    
                    // add number if not Constant
                    if((finalVars[i] != " ") && (!finalVars[i].empty())){
                        // add number
                        if(PositionOfVar != -1){
                            finalCoffs.push_back(coffs[PositionOfVar]);
                        }else{
                            finalCoffs.push_back(0);
                        }
                    }
                    
                    
                }
                
                Rows.push_back(finalCoffs);
            }
            
            /// end function
            if(operation == "solve"){
                
                int n = numberOfEquations;
                double mat[10][10];
                int i, j;
                for(i=0;i<n;i++)
                {
                    for(j=0;j<n;j++)
                    {
                            mat[i][j] = Rows[i][j];

                    }
                }
                
                double D_value = det(n,mat);
                
                
                
                for(int i = 0;i < finalVars.size();i++){
                
                
                    if((finalVars[i] != " ") && (!finalVars[i].empty())){
                        string var = finalVars[i];
                        
                        
                        
                        // Rows (Given To Function) (outPut)
                        vector<vector<float>> Rows;
                        
                        
                        ///////// start Get D /////////////
                        
                        // all variables in Vector
                        vector<string> finalVars;
                        
                        for(int i = 0; i < eqs.size();i++){
                            for(int k = 0;k < eqs[i].size();k++){
                                // first Equation
                                vector<string> vars;
                                vector<float> coffs;
                                properForm(eqs[i],coffs,vars);
                                finalVars.insert(finalVars.end(), vars.begin(), vars.end());
                            }
                        }
                        
                        sort( finalVars.begin(), finalVars.end() );
                        finalVars.erase( unique( finalVars.begin(), finalVars.end() ), finalVars.end() );
                        
                        for(int s = 0; s < eqs.size();s++){
                            // first Equation
                            vector<string> vars;
                            vector<float> coffs;
                            vector<float> finalCoffs;
                            properForm(eqs[s],coffs,vars);
                            
                            // insert equation in vector
                            for(int i = 0;i < finalVars.size();i++){
                                int PositionOfVar = -1;
                                for(int k = 0;k < vars.size();k++){
                                    if(finalVars[i] == vars[k]){
                                        PositionOfVar = k;
                                    }
                                    if(vars[k] == var && finalVars[i] == vars[k]){
                                        PositionOfVar = 0;
                                    }
                                }
                                
                                // add number if not Constant
                                if((finalVars[i] != " ") && (!finalVars[i].empty())){
                                    // add number
                                    if(PositionOfVar != -1){
                                        finalCoffs.push_back(coffs[PositionOfVar]);
                                    }else{
                                        finalCoffs.push_back(0);
                                    }
                                }
                                
                                
                            }
                            
                            Rows.push_back(finalCoffs);
                        }
                        
                        /// end function
                        

                        
                        int numEqs = numberOfEquations;
                        double matCurr[10][10];
                        int ia, ja;
                        for(ia=0;ia<numEqs;ia++)
                        {
                            for(ja=0;ja<numEqs;ja++)
                            {
                                matCurr[ia][ja] = Rows[ia][ja];
                                
                            }
                        }
                        
                        cout << var << "=" << det(numEqs,matCurr) / D_value << endl;


                        
                        
                    }
                
                
                } // end for
                
                }
            else if(operation == "D_value"){
                int n = numberOfEquations;
                double mat[10][10];
                int i, j;
                for(i=0;i<n;i++)
                {
                    for(j=0;j<n;j++)
                    {
                            mat[i][j] = Rows[i][j];
                    }
                }
                cout <<det(n,mat);


            }else{
                for(int i = 0; i < Rows.size();i++){
                    for(int k = 0;k < Rows[i].size();k++){
                            cout << Rows[i][k] << "\t";
           
                    }
                    cout << endl;
                }
            }

            
            
        }else if(operation.find("D") !=std::string::npos){
            
            unsigned long firstPosition = operation.find(" ");
            string var = operation.substr(firstPosition+1,operation.length()-firstPosition);
            
            // Rows (Given To Function) (outPut)
            vector<vector<float>> Rows;

            
            ///////// start Get D /////////////
            
            // all variables in Vector
            vector<string> finalVars;
            
            for(int i = 0; i < eqs.size();i++){
                for(int k = 0;k < eqs[i].size();k++){
                    // first Equation
                    vector<string> vars;
                    vector<float> coffs;
                    properForm(eqs[i],coffs,vars);
                    finalVars.insert(finalVars.end(), vars.begin(), vars.end());
                }
            }
            
            sort( finalVars.begin(), finalVars.end() );
            finalVars.erase( unique( finalVars.begin(), finalVars.end() ), finalVars.end() );
            
            for(int s = 0; s < eqs.size();s++){
                // first Equation
                vector<string> vars;
                vector<float> coffs;
                vector<float> finalCoffs;
                properForm(eqs[s],coffs,vars);
                
                // insert equation in vector
                for(int i = 0;i < finalVars.size();i++){
                    int PositionOfVar = -1;
                    for(int k = 0;k < vars.size();k++){
                        if(finalVars[i] == vars[k]){
                            PositionOfVar = k;
                        }
                        if(vars[k] == var && finalVars[i] == vars[k]){
                            PositionOfVar = 0;
                        }
                    }
                    
                    // add number if not Constant
                    if((finalVars[i] != " ") && (!finalVars[i].empty())){
                        // add number
                        if(PositionOfVar != -1){
                            finalCoffs.push_back(coffs[PositionOfVar]);
                        }else{
                            finalCoffs.push_back(0);
                        }
                    }
                    
                    
                }
                
                Rows.push_back(finalCoffs);
            }
            
            /// end function
            
            
            for(int i = 0; i < Rows.size();i++){
                for(int k = 0;k < Rows[i].size();k++){
                    cout << Rows[i][k] << "\t";
                }
                cout << endl;
            }
            
            
        }
    
        cout << endl;
    getline(cin,operation);
        
    }
 
    

    
    
    cout << endl;
    


    return 0;
}
