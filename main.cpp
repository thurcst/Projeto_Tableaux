#include "AnalyticTableaux.cpp"
#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

#define endl '\n'

using namespace std;

// Aplicar regras.
vector <Node*> applyRule(Node* node) {
    vector <Node*> insertedNodes;

    /*
     * pega operador principal;
     * pega o valor verdade;
     * aplica a regra
     */

    if(node->getExpression() == "((~Q) > (~P))" && node->getTruthValue() == 1) {
        insertedNodes = node->insertSides("(~Q)", 0, "(~P)", 1);
    }
    else if(node->getExpression() == "(~Q)" && node->getTruthValue() == 0) {
        insertedNodes = node->insertFront("Q", 1);
    }
    else if(node->getExpression() == "(~P)" && node->getTruthValue() == 1) {
        insertedNodes = node->insertFront("P", 0);
    }
    node->markApplied();
    return insertedNodes;
}

int achar_problema(string frase){
    /*  Problemas:
            1 = tautologia
            2 = refutavel
            3 = insatisfativel
            4 = satisfativel
            5 = consequencia logica
    */
    int problema;
    if(frase.find("tautologia")!= string::npos){
        //se achar "tautologia" na string, o problema é saber se é tautologia
        problema = 1
        }
    else if(frase.find("refutavel")!= string::npos){
        //se achar "refutavel" na string, o problema é saber se é refutável
        problema = 2
    }
    else if(frase.find("insatisfativel")!= string::npos){
        //se achar "insatisfativel" na string, o problema é saber se é insat.
        //procuramos insat antes de satisfativel pois satisfativel e uma substring de insatisfativel
        problema = 3
        }
    else if(frase.find("satisfativel")!= string::npos){
        //se achar "satisfativel" na string, o problema é saber se é sat.
        problema = 4
    }
    else if(frase.find("consequencia logica de")!= string::npos){ 
        problema = 5
        }
        return problema;
}
char achar_operador(string expressao){
    //enquanto o n de parentesis nao for 1, o operador principal continua sendo procurado
    //se o n de parentesis for 1 e achar um operador, esse é o principal, já que a operação negação também esté entre parentesis.
    char operador;
    for(int i = 0; i< expressao.length(); i++){
        int contador = 0;
        if(expressao[i] == '('){
            contador++;
        }
        else if(expressao[i] == ')'){
            contador--;
        }
        else if(expressao[i] == '&' || expressao[i] == 'v' || expressao[i] == '~' || expressao[i] == '>'){
            if(contador == 1){
                operador = expressao[i];
            }
        }
    }
    return operador;
}
void achar_subExpr(string expressao, string *substring1, string *substring2){
    int i=0,par_esq = 0,par_dir = 0;
    //caso atomico
    if(expressao[i+1] != '('){
        //se nao houver um ( apos o primeiro (, significa que e uma expressao atomica, ou seja, pegamos esse char para a substring1
        *substring1 = expressao[i+1];
        i+=5;
    }
    else{
        //caso haja, fazemos um for indo do parentesis inicial ate o ultimo parentesis antes do operador da string
        for(int j = 1;par_esq != par_dir && j < expressao.length(); j++){
            //enquanto o numero de parentesis forem diferentes, copiar a string para a substring1
            if(expressao[j]=='('){
                par_esq ++;
            }
            else if(expressao[j] == ')'){
                par_dir++;
            }
            *substring1.push_back(expressao[j]);
        }
        i+=4;
    }
    for(i;i<expressao[i].size()-1;i++){
        //copiamos o resto da string ate o ultimo parentesis* (ignoramos o ultimo parentesis por ser da expressao completa)
        *substring2.push_back(expressao[i]);
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // ---- Arquivos ----
    ifstream entrada;
    ofstram saida;


    // ---- variaveis ----
    string enunciados[1000], proposicao[1000];
    int problema[1000], n_enunciados, cursor, icursor;
    char operador_principal;
    Node arvore;
    
    cin >> n_enunciados;
    cin.ignore(); //ignorar o "\n" depois do n de enunciados
    saida.open("./inout/Saida.out");
    entrada.open("./inout/Entrada.in");
    entrada >> cursor;
    icursor = cursor;

    for(int i = 0;icursor > 0 && i < n_enunciados;i++){
        getline(cin,enunciados[i], '\n'); //recebe a linha ate o "\n"
        if(enunciados[i][0] == '('){
            //se o enunciado começa com (
            size_t ret = enunciados[i].rfind(")");//procuramos até a ultima ocorrencia do )
            for(int j = 0; j <= ret; j++){
                proposicao[i].push_back(enunciados[i][j]);
            }
        }
        else{
            proposicao[i] = enunciados[i][0];
        }
        problema[i] = achar_problema(enunciados[i]);
            /*  
                Problemas:
            1 = tautologia
            2 = refutavel
            3 = insatisfativel
            4 = satisfativel
            5 = consequencia logica

                Operadores:
            & = E
            v = Ou
            ~ = Nao
            > = Implicaçao

            */

            if(problema[i] == 1){
                //se for problema de tautologia
                Node tableaux = Node(proposicao[i],false);
                arvore = tableaux;
                }
            else if(problema[i] == 2){
                //se for problema de refutabilidade
                Node tableaux = Node(proposicao[i],false);
                arvore = tableaux;
                }

            else if(problema[i] == 3){
                //se for problema de insatisfatibilidade
                Node tableaux = Node(proposicao[i],true);
                arvore = tableaux;
                }
            else if(problema[i] == 4){
                //se for problema de satisfatibilidade
                Node tableaux = Node(proposicao[i],true);
                arvore = tableaux;
                }
            else{
                //se for pboelam de consequencia logica
                Node tableaux = Node(proposicao[i],false);
                arvore = tableaux;
                string conjunto_expr;
                vector <Node *> inserir;
                size_t posic = enunciados[i].find("{");
                size_t lastposc = enunciados[i].rfind("}");
                for(int ate_la = posic + 1; ate_la < lastposc;ate_la++){
                    conjunto_expr.push_back(enunciados[ate_la]);
                }
                
                while(conjunto_expr.length() > 0 && !arvore.isClosed()){

                }
            }

            saida << "Problema #" << i + 1 << endl;

            if(problema[i]==1) {
                if(arvore.isClosed()){
                   saida << "Nao, nao e satisfativel." << endl; 
                } 
                else{
                    saida << "Sim, e satisfativel." << endl;
                } 
            }
            else if(problema[i]==2) {
                if(arvore.isClosed()){
                    saida << "Nao, nao e refutavel." << endl;
                } 
                else{
                    saida << "Sim, e refutavel." << endl;
                }
            }
            else if(problema[i]==3) {
                if(arvore.isClosed()){
                    saida << "Sim, e tautologia." << endl;
                } 
                else{
                    saida << "Nao, nao e tautologia." << endl;
                } 
            }
            else if(problema[i]==4) {
                if(arvore.isClosed()){
                    saida << "Sim, e insatisfativel." << endl;
                } 
                else{
                    saida << "Nao, nao e insatisfativel." << endl;
                }
            }
            else {
                if(arvore.isClosed()){
                   saida << "Sim, e consequencia logica." << endl; 
                } 
                else{
                    saida << "Nao, nao e consequencia logica." << endl;
                }
            }

            icursor--;
            if(icursor > 0) saida << endl;

    }

/*    vector <Node*> appliableNodes = tableau.getAppliableNodes();
    while(!tableau.isClosed() && !appliableNodes.empty()) {
        //for(Node* node : appliableNodes) {
        //}
        for(int i = 0; i < appliableNodes.size(); i++) {
            vector <Node*> insertedNodes = applyRule(appliableNodes[i]);
            checkContradictions(insertedNodes);
            appliableNodes = tableau.getAppliableNodes();
        }
    }
*/
    saida.close();
    entrada.close();

    return 0;
}