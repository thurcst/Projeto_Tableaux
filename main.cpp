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

void checkContradictions(vector<Node*> insertedNodes) {
    for (int i = 0; i < insertedNodes.size(); i++) {
        if(insertedNodes[i]->checkContradiction())
            insertedNodes[i]->markContradiction();
    }
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

    for(int i = 0; i < n_enunciados;i++){
        getline(cin,enunciados[i], '\n'); //recebe a linha ate o "\n"
        if(enunciados[i][0] == '('){
            //se o enunciado começa com (
            size_t ret = enunciados[i].rfind(")");//procuramos até a ultima ocorrencia do )
            for(int j = 0; j <= ret; j++){
                proposicao[i].push_back(enunciados[i][j]);
            }
        }
        else{
            problema[i] = enunciados[i][0];
            size_t posic = enunciados[i].find("{");
            size_t lastposc = enunciados[i].rfind("}");
            //problema[i]+= enunciados[i].substr(posic + 1);
            proposicao[i].push_back(',');
            for(int j = posic + 1; j <= lastposc - 1 ; j++){
                proposicao[i].push_back(enunciados[i][j]);
            }
        }

        problema[i] = achar_problema(enunciados[i]);
            /*  Problemas:
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
                Node tableaux = Node(proposicao[i],false);
                operador_principal = achar_operador(tableaux.expression);
            }
            else if(problema[i] == 2){
                Node tableaux = Node(proposicao[i],false);
                operador_principal = achar_operador(tableaux.expression);
                }

            else if(problema[i] == 3){
                Node tableaux = Node(proposicao[i],true);
                operador_principal = achar_operador(tableaux.expression);
            }
            else if(problema[i] == 4){
                Node tableaux = Node(proposicao[i],true);
                operador_principal = achar_operador(tableaux.expression);
            }
            else{
                Node tableaux = Node(proposicao[i],true);
            }
    }
    //criar o primeiro nó.
    tableau.insertFront("P", 1);
    tableau.insertFront("Q", 0);

    vector <Node*> appliableNodes = tableau.getAppliableNodes();
    while(!tableau.isClosed() && !appliableNodes.empty()) {
        /*for(Node* node : appliableNodes) {
        }*/
        for(int i = 0; i < appliableNodes.size(); i++) {
            vector <Node*> insertedNodes = applyRule(appliableNodes[i]);
            checkContradictions(insertedNodes);
            appliableNodes = tableau.getAppliableNodes();
        }
    }

    if(tableau.isClosed())
        std::cout << "Tableau fechado." << std::endl;

    tableau.printTree();

    return 0;
}