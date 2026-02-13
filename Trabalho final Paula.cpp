#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;


struct Info_roupa{

    string Tipo;
    string Tamanho;
    string descricao;
    string marca;
    float valor;


    void adiciona_roupa() {

        Info_roupa roupa;

        cout << "Diga que Tipo de roupa é (ex: Camisa, calca, sapato, etc...): ";
        getline(cin, roupa.Tipo);
        while (roupa.Tipo.empty()) {
            cout << "Campo Obrigatório! Infome o Tipo de roupa: ";
            getline(cin, roupa.Tipo);
        }

        cout << "Agora o Tamanho da roupa (ex: M, G, P, etc..): ";
        getline(cin, roupa.Tamanho);
        while (roupa.Tamanho.empty()) {
            cout << "Campo Obrigatório! Infome o Tamanho da roupa: ";
            getline(cin, roupa.Tamanho);
        }

        cout << "Agora dê uma breve Descrição sobre a roupa: ";
        getline(cin, roupa.descricao);
        while (roupa.descricao.empty()) {
            cout << "Campo Obrigatório! Infome a Descrição da roupa (se não souber ponha 'desconhecida'): ";
            getline(cin, roupa.descricao);
        }
        cout << "Agora diga a Marca da roupa: ";
        getline(cin, roupa.marca);
        while (roupa.marca.empty()) {
            cout << "Campo Obrigatório! Infome a Marca da roupa: ";
            getline(cin, roupa.marca);
        }

        cout << "Agora o valor da roupa: ";
        cin >> roupa.valor;
        while((cin.fail()) or (roupa.valor <= 0)) {
            cin.clear(); // destrava o cin pois a informação errada do usuário fica no buffer, impedindo que uma nova informaçãos seja adicionada
            cin.ignore(10000, '\n'); // limpa o buffer para que o dado errado guardado não afete as próximas leituras
            cout << "Valor inválido! Informe um Valor válido: ";
            cin >> roupa.valor;
        }

        ofstream adc_roupa("Roupas_em_Estoque.txt", ios::app); // serve para adicionar itens sem apagar os anteriores
        adc_roupa << roupa.Tipo<< " | " << roupa.Tamanho << " | " << roupa.descricao
        << " | " << roupa.marca << " | " << fixed << setprecision(2) << roupa.valor << "R$" << endl;
        adc_roupa.close();
        }
};

string menu() {

    return "Digite o número da operação desejada abaixo: \n"
            "1. Adicionar roupa ao estoque\n"
            "2. Ver itens do estoque\n" 
            "3. Ver quantidade de roupas no estoque\n"
            "Digite aqui: ";
}

struct SistemaEstoque{

    void ver_estoque(){
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
        string dados;
        cout << "======================== ITENS EM ESTOQUE =========================" << endl 
            << "==== Roupa ==== Tamanho ==== Descrição ==== Marca ==== Preco ====" << endl << endl;
            while (getline(lendo_estoque, dados)) {
                cout << dados << endl;
                }
        lendo_estoque.close();   
        }

    void ver_quantidade(){
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
        if(!lendo_estoque){
            cout << setw(50) << string(50, '=') << endl;
            cout << setw(40) << "ARQUIVO NÃO ENCONTRADO!" << endl;
            cout << setw(50) << string(50, '=');
        }

        string linhas;
        if (!getline(lendo_estoque, linhas)){
            cout << setw(50) << string(50, '=') << endl;
            cout << setw(40) << "ESTOQUE VAZIO! ADICIONE UMA ROUPA!" << endl;
            cout << setw(50) << string(50, '=');
            return;
            }     
    }
};

   
int main(){
    int Sim_ou_Nao = 1;
    SistemaEstoque funcao;
    
    while (Sim_ou_Nao == 1) {
        int opcao;
        cout << menu();
        cin >> opcao;
        cin.ignore();

      switch (opcao) {
        case 1:{
            Info_roupa roupa;
            roupa.adiciona_roupa();
            cout << endl << setw(50) << string(40, '=') << endl;
            cout << setw(45) << "Roupa Adicionada com Sucesso!!!" << endl;
            cout << setw(50) << string(40, '=') << endl;
            break;
        }
        
        case 2:
            funcao.ver_estoque();
            break;
        
        case 3:
            funcao.ver_quantidade();
            break;
        
        default:
            cout << endl << "Opção Inválida!!!" << endl;   
      }

      cout << endl << setw(47) << "Deseja realizar outra operação?" << endl; 
      cout << setw(50) << "Digite 1 para 'SIM' ou 2 para 'NÃO': ";
      cin >> Sim_ou_Nao;
      cin.ignore();
    }

    cout << endl << "================= FIM DO PROGRAMA. OBRIGADO(A) =================" << endl;

    return 0;
}
