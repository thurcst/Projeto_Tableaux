#include "AnalyticTableaux.cpp"
#include <bits/stdc++.h>
#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <cstring>

#define endl '\n'

using namespace std;
char achar_operador(string expressao);
string nega_prop(string expressao);
vector <Node*> byPreference(vector <Node *> arvore);
int achar_problema(string frase);
void achar_subExpr(string expressao, string *substring1, string *substring2);




int main() {
    cin.tie(nullptr);

    // ---- Arquivos ----
    ifstream entrada;
    ofstream saida;
    saida.open("Saida.out");
    entrada.open("testzada.in");
    
    // ---- variaveis ----
    string enunciados[(int)1e4], proposicao[(int)1e4]; //vetor enunciado recebe pergunta / vetor proposicao apenas as proposicoes (substrings da pergunta)
    string substring_1,substring_2, string_ngd;//ngd = negada
    int problema[1001], n_enunciados, restantes;
    char operador_principal;
    bool valor;
    Node arvore("",false);
    vector <Node *> appNodes, folhas;

    entrada >> n_enunciados;
    restantes = n_enunciados;
    entrada.ignore(); //ignorar o "\n" depois do n de enunciados

    for(int aux = 0; aux < n_enunciados; aux++){
        getline(entrada,enunciados[aux]);
    }
    entrada.close();
    for(int i = 0; i < n_enunciados;i++){
        if(enunciados[i][0] == '('){
            /*AQUI TA O PROBLEMAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA
            Em problemas de consequencia lógica ele corre até o FODENOD ULTIMO ')' e por isso ele copia uma expressao INTEIRA pra o vetor.*/
            //se o enunciado começa com (
            size_t ret = enunciados[i].rfind(')');//procuramos até a ultima ocorrencia do )
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
                Node tableaux(proposicao[i],false);
                arvore = tableaux;
                }
            else if(problema[i] == 2){
                //se for problema de refutabilidade
                Node tableaux(proposicao[i],false);
                arvore = tableaux;
                }

            else if(problema[i] == 3){
                //se for problema de insatisfatibilidade
                Node tableaux(proposicao[i],true);
                arvore = tableaux;
                }
            else if(problema[i] == 4){
                //se for problema de satisfatibilidade
                Node tableaux(proposicao[i],true);
                arvore = tableaux;
                }
            else{
                //se for problema de consequencia logica
                Node tableaux = Node(proposicao[i],false);
                arvore = tableaux;
                string conjunto_expr;
                string vet_auxiliar[1000];
                vector <Node *> auxiliar;
                int posic = enunciados[i].find('{');
                int lastposc = enunciados[i].rfind('}');
                int cnt_ajd = 0;
                int cnt_total = 0;
                for(int ate_la = posic + 1; ate_la < lastposc;ate_la++){
                    //recebe em conjunto_expr todas as expressoes dentro dos {}
                    conjunto_expr.push_back(enunciados[i][ate_la]);
                }
                cnt_total = conjunto_expr.length();
                int a = 0;
                for(int ajd = 0; ajd < conjunto_expr.length();ajd++){
                    cnt_ajd++;
                    /*vamos percorrer o vetor e sempre que acharmos uma , pulamos ela e passamos pra proxima posicao do vector auxiliar
                        Por exemplo, no conjunto {(Q v P), Q,(J & P)} o vector vai ficar
                        auxiliar[0] = (Q v P)
                        auxiliar[1] = Q
                        auxiliar[2] = (J & P)
                    */
                    if(conjunto_expr[ajd]==','){
                        a++;
                    }
                    else if(conjunto_expr[ajd] != ' '){
                        vet_auxiliar[a].push_back(conjunto_expr[ajd]);
                    }
                    if(ajd == conjunto_expr.size() - 1 || conjunto_expr[ajd + 1] == ','){
                        cout << vet_auxiliar[a] << endl;
                        auxiliar = arvore.insertFront(vet_auxiliar[a],true);
                    }
                }
            }

            while(!arvore.isClosed() && !arvore.getAppliableNodes().empty()) {
                appNodes.clear();
                folhas.clear();
                
                //De acordo com a preferencia, os nos que bifurcam tem menos preferencia, entao eles ficma por ultimo
                appNodes = byPreference(arvore.getAppliableNodes());

                for(int aux=0; aux < appNodes.size(); aux++) {
                    //atualizar o vetor de proposicao pra ordem correta
                    proposicao[i] = appNodes[aux]->getExpression();
                    valor = appNodes[aux]->getTruthValue();
                    operador_principal = achar_operador(proposicao[i]);
                    
                    if(operador_principal == '~'){
                        string_ngd="";
                        string_ngd = nega_prop(proposicao[i]); //negamos a prop caso o perador principal seja '~'
                        folhas = appNodes[aux]->insertFront(string_ngd, !valor);
                    }
                    else if(operador_principal == '&') {
                        substring_1="";
                        substring_2="";
                        achar_subExpr(proposicao[i], &substring_1, &substring_2);//dividimos a expressao em duas subexpressores
                        if(valor){
                            //caso seja true, ambos sao verdade, nao bifurca
                           folhas = appNodes[aux]->insertFront(substring_1, valor, substring_2, valor); 
                        } 
                        else{
                            //caso contrario, bifurca a arvore para caso sbst1 seja 0 ou sbst2 seja 0
                            folhas = appNodes[aux]->insertSides(substring_1, valor, substring_2, valor);
                        }
                    }
                    else if(operador_principal == 'v') {
                        substring_1="";
                        substring_2="";
                        achar_subExpr(proposicao[i], &substring_1, &substring_2);
                        
                        if(!valor){
                            //caso o ou seja falto, ambas sao falsas, nao bifurca
                            folhas = appNodes[aux]->insertFront(substring_1, valor, substring_2, valor);
                        } 
                        else{
                            //caso contrario, uma da sduas e verdadeira, bifurcaçao
                            folhas = appNodes[aux]->insertSides(substring_1, valor, substring_2, valor);
                        } 
                    }
                    else if(operador_principal == '>') {
                        substring_1="";
                        substring_2="";
                        achar_subExpr(proposicao[i], &substring_1, &substring_2);
                        if(!valor){
                            //caso seja falso, nao bifurca, substring 1 e verdadeira e substring2 e falsa;
                            folhas = appNodes[aux]->insertFront(substring_1, !valor, substring_2, valor);
                        } 
                        else{
                            //caso seja verdadeiro, bifurca em casos de substring 1 ser falso e substring 2 ser verdadeiro
                            folhas = appNodes[aux]->insertSides(substring_1, !valor, substring_2, valor);
                        } 
                    }
                    for(int aux2 = 0; aux2 < folhas.size(); aux2++) {
                        if(folhas[aux2]->checkContradiction()){
                            folhas[aux2]->markContradiction();
                        }    
                    }
                    appNodes[aux]->markApplied();
                }
            }
            saida << "Problema #" << i + 1 << endl;
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
           arvore.printTree();
            if(problema[i]==1) {
                if(!arvore.isClosed()){
                   saida << "Nao, nao e tautologia." << endl; 
                } 
                else{
                    saida << "Sim, e tautologia." << endl;
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
                    saida << "Sim, e insatisfativel." << endl;
                } 
                else{
                    saida << "Nao, nao e insatisfativel." << endl;
                } 
            }
            else if(problema[i]==4) {
                if(!arvore.isClosed()){
                    saida << "Sim, e satisfativel." << endl;
                } 
                else{
                    saida << "Nao, nao e satisfativel." << endl;
                }
            }
            else {
                if(!arvore.isClosed()){
                   saida << "Sim, e consequencia logica." << endl; 
                } 
                else{
                    saida << "Nao, nao e consequencia logica." << endl;
                }
            }
            restantes--;
            if(restantes > 0) saida << endl;

    }
    saida.close();
    return 0;
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

string nega_prop(string expressao){
    int aux = 2;
    size_t achei = expressao.find('~');
    int aux2 = achei;
    int par_esq = 0, par_dir = 0;
    string nova_expressao;
    for(int i=2; i<expressao.size()-1; i++){
        nova_expressao.push_back(expressao[i]);
    }
    return nova_expressao;
}


vector <Node*> byPreference(vector <Node *> arvore) {
    // Ordena o vetor por preferencia de nao bifurcaçao
    vector <Node*> no_organizado;
    char operador;
    bool valor;
    for (int aux =0; aux < arvore.size(); aux ++){
        operador = achar_operador(arvore[aux]->getExpression());
        valor = arvore[aux]->getTruthValue();

        if((operador == 'v' && valor == false) || (operador == '&' && valor == true) || (operador == '>' && valor == false) || operador == '~'){
            no_organizado.insert(no_organizado.begin(),arvore[aux]);
            //se for uma dessas regras, colocamos no começo
        }
        else{
            no_organizado.push_back(arvore[aux]);
            //caso nao seja, colocamos no final
        }
    }
    return no_organizado;
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
        problema = 1;
        }
    else if(frase.find("refutavel")!= string::npos){
        //se achar "refutavel" na string, o problema é saber se é refutável
        problema = 2;
    }
    else if(frase.find("insatisfativel")!= string::npos){
        //se achar "insatisfativel" na string, o problema é saber se é insat.
        //procuramos insat antes de satisfativel pois satisfativel e uma substring de insatisfativel
        problema = 3;
        }
    else if(frase.find("satisfativel")!= string::npos){
        //se achar "satisfativel" na string, o problema é saber se é sat.
        problema = 4;
    }
    else if(frase.find("consequencia logica de")!= string::npos){ 
        problema = 5;
        }
        return problema;
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
            *substring1+=expressao[j];
        }
        i+=4;
    }
    while(i<expressao.length()-1){
        //copiamos o resto da string ate o ultimo parentesis* (ignoramos o ultimo parentesis por ser da expressao completa)
        *substring2+= expressao[i];
        i++;
    }
}
