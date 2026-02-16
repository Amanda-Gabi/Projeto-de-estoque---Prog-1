#include <iostream>
#include <fstream>
#include <iomanip>
using namespace std;

string menu() {

    return "Digite o número da operação desejada abaixo: \n"
            "1. Adicionar roupa ao estoque\n"
            "2. Ver itens do estoque\n" 
            "3. Ver quantidade de roupas no estoque\n"
            "4. Pesquisar por... \n"
            "Digite aqui: ";
}

struct Info_roupa{

    string Tipo;
    string Tamanho;
    string descricao;
    string marca;
    float valor;
    string data;

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
        while((roupa.valor <= 0) or (cin.fail())) {
            cin.clear();  
            cin.ignore(10000, '\n');  
            cout << "Valor inválido! Informe um Valor válido: ";
            cin >> roupa.valor;
        }
        cin.ignore();

        cout << "Em que Data você está adicionando a roupa? (informe DD/MM/AAAA): ";
        getline(cin, roupa.data);
        while (roupa.data.empty()) {
            cout << "Campo Obrigatório! Infome a Data: ";
            getline(cin, roupa.data);
        }

        ofstream adc_roupa("Roupas_em_Estoque.txt", ios::app);  
        adc_roupa << roupa.Tipo << " | " << roupa.Tamanho << " | " << roupa.descricao
        << " | " << roupa.marca << " | " << "R$" << fixed << setprecision(2) << roupa.valor << endl;
        adc_roupa.close();
        }
};

struct DataFunções{
    bool formato_data(string data){
        if (data.length() != 10){
            return false;
        }
        if ((data[2] != /) or (data[5] != /)){
            return false;
        }
        
        int posicoes_validas[] = {0, 1, 3, 4, 6, 7, 8, 9,};

        for (int posicao = 0; posicao < 8; posicao++){
            int valido = posicoes_validas[posicao];
            if (!isdigit(data[i])){
                return false;
            }
        }

        return true;
    }

    bool valores_validos(string data){
        if (!formato_data(data)){
            return false;
        }

        int dia = stoi(data.substr(0, 2));
        int mes = stoi(data.substr(3, 2));
        int ano = stoi(data.substr(6, 4));

        if ((dia < 1) or (dia > 31)){
            return false;
        }

        if((mes < 1) or (mes > 12)){
            return false;
        }

        if (ano < 1){
            return false;
        }

        return true;

    }
};

struct Sistema_Estoque{
    void ver_estoque(){
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
        string dados;
        cout << "======================= ITENS EM ESTOQUE ========================" << endl 
            << "--- Roupa ---- Tamanho ---- Descrição ---- Marca ---- Preço ---" << endl << endl;
            while (getline(lendo_estoque, dados)) {
                cout << dados << endl;
                }
        cout << string(65, '=') << endl;
        lendo_estoque.close();   
        }

    void ver_quantidade(){
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
            if(!lendo_estoque){
                cout << setw(35) << string(50, '=') << endl;
                cout << setw(35) << "ARQUIVO NÃO ENCONTRADO!" << endl;
                cout << setw(35) << string(50, '=') << endl;
            }

            string linhas;
            if (!getline(lendo_estoque, linhas)){
                cout << string(65, '=') << endl;
                cout << setw(50) << "ESTOQUE VAZIO! ADICIONE UMA ROUPA!" << endl;
                cout << string(65, '=') << endl;
                return;
            }

            int conta_linha = 1;

            while(getline(lendo_estoque, linhas)) {
                conta_linha++;
            }

        lendo_estoque.close();

        cout << string(65, '=') << endl;
        cout << setw(40) << "ITENS CADASTRADOS" << endl;
        cout << string(65, '-') << endl;
        cout << setw(34) << "Total: " << conta_linha << endl;
        cout << string(65, '=') << endl;
    }
    
    void ver_intervalo_data(){
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
            if(!lendo_estoque){
                cout << setw(35) << string(50, '=') << endl;
                cout << setw(35) << "ARQUIVO NÃO ENCONTRADO!" << endl;
                cout << setw(35) << string(50, '=') << endl;
                return;
            }

            string linhas;
            if (!getline(lendo_estoque, linhas)){
                cout << string(65, '=') << endl;
                cout << setw(50) << "ESTOQUE VAZIO! ADICIONE UMA ROUPA!" << endl;
                cout << string(65, '=') << endl;
                return;
            }

            int tam_vetor = 1;

            while(getline(lendo_estoque, linhas)) {
                tam_vetor++;
            }
        lendo_estoque.close();

        string *roupas = new string[tam_vetor];

        lendo_estoque.open("Roupas_em_Estoque.txt");

            for(int i = 0; i < tam_vetor; i++){
                getline(lendo_estoque, roupas[i]);
            }

        lendo_estoque.close();

        string inicio_data, fim_data;
        cout << string(65, '=') << endl;
        cout << "Ver itens de (DD/MM/AAAA): ";
        getline(cin, inicio_data);
            while (inicio_data.empty()) {
                cout << "Campo Obrigatório! Infome a Data de Inicio da busca: ";
                getline(cin, inicio_data);
            }

        cout << "Ate (DD/MM/AAAA): ";
        getline(cin, fim_data);
            while (inicio_data.empty()) {
                cout << "Campo Obrigatório! Infome a Data de Fim da busca: ";
                getline(cin, inicio_data);
            }
        cout << string(65, '-') << endl;

        cout << "ITENS ADICIONADOS DE: " << inicio_data << " ATÉ " << fim_data << ":" << endl;
        
        int enco_periodo = 0;
        for(int i = 0; i < tam_vetor; i++){
            string l_atual = roupas[i];

            
            int achar_traco = -1;
            int t = 0;

            for(t = 0; t < l_atual.length(); t++){
                if(l_atual[t] == '|'){
                    achar_traco = t;
                }
            }
        
            if(achar_traco != -1){
                string Data_do_Item = "";
                    
                for(int d = achar_traco + 2; d < l_atual.length(); d++){
                    Data_do_Item += l_atual[d];
                }
                
                string item_de_comp = Data_do_Item.substr(6, 4) + "-" +
                                     Data_do_Item.substr(3, 2) + "-" +
                                     Data_do_Item.substr(0,2);

                string comp_de_inicio = inicio_data.substr(6, 4) + "-" +
                                       inicio_data.substr(3, 2) + "-" +
                                       inicio_data.substr(0, 2);
                
                string comp_de_fim = fim_data.substr(6, 4) + "-" +
                                    fim_data.substr(3, 2) + "-" +
                                    fim_data.substr(0, 2);
               
                if ((item_de_comp >= comp_de_inicio) and (item_de_comp <= comp_de_fim)){
                    cout << l_atual << endl;
                    enco_periodo++;
               } 
            }      
        }
        
        cout << endl << "Adicionados no periodo: " << enco_periodo << endl;
        cout << string(65, '=');

        delete[] roupas;

    }

    void excluir_num_item() {
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
            if(!lendo_estoque){
                cout << setw(35) << string(50, '=') << endl;
                cout << setw(35) << "ARQUIVO NÃO ENCONTRADO!" << endl;
                cout << setw(35) << string(50, '=') << endl;
                return;
            }

            string linhas;
            if (!getline(lendo_estoque, linhas)){
                cout << string(65, '=') << endl;
                cout << setw(50) << "ESTOQUE VAZIO! ADICIONE UMA ROUPA!" << endl;
                cout << string(65, '=') << endl;
                return;
            }


    }
};
 
int main(){
    int Sim_ou_Nao = 1;
    Sistema_Estoque sistema;
    
    while (Sim_ou_Nao == 1) {
        int principal;
        cout << menu();
        cin >> principal;
        cin.ignore();
        cout << endl;


      switch (principal) {
        case 1:{
            Info_roupa roupa;
            roupa.adiciona_roupa();
            cout << endl << setw(50) << string(40, '=') << endl;
            cout << setw(45) << "Roupa Adicionada com Sucesso!!!" << endl;
            cout << setw(50) << string(40, '=') << endl;
            break;
        }
        
        case 2:
            sistema.ver_estoque();
            break;
        
        case 3:
            sistema.ver_quantidade();
            break;
        
        case 4: {
            int secundario;
            cout << "4. Pesquisar por..." << endl;
            cout << "1. Data" << endl;
            cout << "2. Identificador do Item" << endl;
            cout << "Digite aqui: ";
            cin >> secundario;

            while((cin.fail()) or (secundario < 1) or (secundario > 2)){
            cin.clear();  
            cin.ignore(10000, '\n');  
            cout << "Infome uma ação válida: ";
            cin >> secundario;
            }

            switch(secundario){
                case 1:
                    sistema.ver_intervalo_data();
                    break;  
                    
                default:
                    cout << "Digite uma opção válida!" << endl;
            }
            
            break;
        }
        
        default:
            cout << endl << "Opção Inválida!!!" << endl;
        
      }

      cout << endl << setw(47) << "Deseja realizar outra operação?" << endl; 
      cout << setw(50) << "Digite 1 para 'SIM' ou 2 para 'NÃO': ";
      cin >> Sim_ou_Nao;
      cout << endl;
      if((Sim_ou_Nao != 1) and (Sim_ou_Nao != 2)){
        cout << string(65, '=') << endl;
        cout << setw(43) << "Erro! Opção Inválida" << endl;
        cout << string(65, '=') << endl;
      }
    }

    cout << endl << "================= FIM DO PROGRAMA. OBRIGADO(A) ==================" << endl;

    return 0;
}
