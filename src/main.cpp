#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include<bits/stdc++.h>
using namespace std;

#include "../include/KenoBet.hpp"
#include "../include/validation.hpp"

/*
Avaliação
Processamento do Arquivo de Entrada: 1
Execução correta: 1
Interfcace Textual: 1
Documentação: 1
Compilação automatizada: 1
Organização em src, include, build, data: 1

Observações:
Vocês poderiam ter oganizado a parte de validação em uma classe separada. O uso do
rand feito por vcs é muito propenso a repetir números... Ao invés de multiplicar
pelo tempo vcs poderiam apenas setar uma semente e usar o rand puro.
*/

int main(int argc, char *argv[]){

//Validação arquivo de apostas
    std::vector<string> erros = validate(argv[1]);
    if (erros.size() != 0){
        for(int i=0; i<erros.size(); i++)
            cout<<erros[i]<<endl;
        exit(1);
    }

//Abertura de arquivo  
    std::fstream file;
    file.open (argv[1]);
    cout << ">>> Lendo arquivo de apostas ["<<argv[1]<<"], por favor aguarde..."<< endl;
    cout << "-------------------------------------------------------------"<< endl;

//Passagem de valores
    KenoBet aposta;
    cash_type tempF;
    number_type tempI;
    string lineFromFile;
    stringstream ss;

    //Passagem de creditos
    getline( file, lineFromFile );
    ss.clear();
    ss << lineFromFile;  
    ss >> tempF; 
    aposta.set_wage(tempF);
    aposta.set_inicial_wage(tempF);

    //Passagem das rodadas
    getline( file, lineFromFile );
    ss.clear();
    ss << lineFromFile;  
    ss >> tempI; 
    aposta.set_rounds(tempI);

    //Passagem dos numeros apostados
    getline( file, lineFromFile );
    size_t start;
    size_t end = 0;
    while ((start = lineFromFile.find_first_not_of(" ", end)) != std::string::npos){
        end = lineFromFile.find(" ", start);
        ss.clear();
        ss << lineFromFile.substr(start, end - start);
        ss >> tempI; 
        aposta.add_number(tempI);
    }aposta.sort_spots();
    
//Play
    aposta.set_round_wage();
    //Dados aposta
    std::setprecision(2);
    cout << ">>> Aposta do arquivo lida com sucesso!"<< endl;
    cout << "\t Você apostará um total de $" <<  ceil(  aposta.get_wage()  *100)/100 <<".\n";
    cout << "\t Jogará um total de " << aposta.get_rounds()<< " rodadas, apostando $" << ceil(  aposta.get_wage()/aposta.get_rounds()  *100)/100 << " créditos por rodada." <<endl;
    cout << "\n\t Sua aposta tem " << aposta.get_spots().size() << " números, eles são: ";
    aposta.print_spots();
    cout << "\n\n\t\t----------------+---------------" << endl;
    cout << "\t\t| Hits\t\t| Retorno" << endl;

    for(int x = 0; x <= aposta.get_spots().size(); x++)
        cout << "\t\t| "<<x<<"\t\t\t| "<< aposta.get_retorno_espe(aposta.get_spots().size(), x) <<  endl;
    
    for(int q=1; q<=aposta.get_rounds(); q++){
        cout << "\t\t-------------------------------------------------------------"<< endl;  
        //Dados rodada
        cout << "\t\tEsta é a rodada #"<< q <<" de " << aposta.get_rounds() <<", sua aposta é $"<< fixed << setprecision(2) << aposta.get_round_wage() <<". Boa sorte!" << endl;
    
        //Numeros sorteados
        aposta.set_m_sorteados();
        cout<<"\n\t\tOs 20 numeros sorteados foram: ";
        aposta.print_sorteados();
        
        //Hits e Ganho
        aposta.set_hits();
        cout<<"\t\tVocê acertou os números: ";
        aposta.print_hits();
        cout<<", um total de " << aposta.size_hits() << " hits de " << aposta.get_spots().size()<<".\n";
        cout<<"\t\tSua taxa de retorno é " << aposta.get_retorno_round() << ", assim você sai com: $" <<aposta.get_after_round_wage()<<".\n";

        //Atualização dos creditos ao fim do round
        aposta.update_wage();
        cout<<"\t\tVocê possui um total de: $" << aposta.get_wage() <<" créditos."<<endl;          
    }
    cout << ">>> Fim das rodadas!" << endl;
    cout << "-------------------------------------------------------------"<< endl;  
    cout<<"\n======= Sumário ======="<<endl;
    
    aposta.print_reacao(aposta.get_inicial_wage(), aposta.get_wage());

    cout <<">>> Você está saindo do jogo com um total de "<<aposta.get_wage()<<" créditos.";    
    cout << "\n-------------------------------------------------------------"<< endl;
    aposta.reset();
        
    return 0;
}
