#include <iostream>
#include <fstream>
using namespace std;
string menu() {

    return "Digite o numero da operacao desejada abaixo: \n"
            "1. Adicionar roupa ao estoque\n"
            "2. Ver itens do estoque\n" 
            "3. Ver quantidade de roupas no estoque\n"
            "Digite aqui: ";
}
   
int main(){
    int a;
    cout << menu();
    cin >> a;
    string Tipo_roupa, Tamanho_roupa;
    string adiciona_estoque(Tipo_roupa);
    if (a == 1){
    cout << "Diga que tipo de roupa e (ex: Camisa, calca, sapato, etc...): ";
    cin >> Tipo_roupa;
    cout << "Agora o tamanho da roupa (ex: M, G, P, etc..): ";
    cin >> Tamanho_roupa;

    ofstream adiciona_roupa("Roupas_em_Estoque.txt", ios::app);
    adiciona_roupa << Tipo_roupa << "/" << Tamanho_roupa << endl;
    adiciona_roupa.close();
        } 
    
    if (a == 2){
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
        string dados;
        getline(lendo_estoque, dados);
        cout << dados;
        lendo_estoque.close();
    }


    return 0;
}