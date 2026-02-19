#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

string menu() {

    return "Digite o número da operação desejada abaixo: \n"
            "1. Adicionar roupa ao estoque\n"
            "2. Ver itens do estoque\n" 
            "3. Ver quantidade de roupas no estoque\n"
            "4. Pesquisar por Data \n"
            "5. Excluir itens do estoque \n"
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
        << " | " << roupa.marca << " | " << "R$" << fixed << setprecision(2) << roupa.valor << endl
        << " | " << roupa.data << endl;
        adc_roupa.close();
        }
};

struct DataFuncoes{

    bool formato_de_data_valido(string data){
        if (data.length() != 10){
            return false;
        }
        if ((data[2] != '/') or (data[5] != '/')){
            return false;
        }
        
        int posicoes_validas[] = {0, 1, 3, 4, 6, 7, 8, 9};

        for (int posicao = 0; posicao < 8; posicao++){
            int valido = posicoes_validas[posicao];
            if (!isdigit(data[valido])){
                return false;
            }
        }

        return true;
    }

    bool valores_de_data_validos(string data){
        if (!formato_de_data_valido(data)){
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

    bool data_completa_valida(string data){
        return formato_de_data_valido(data) and valores_de_data_validos(data);
    }

    string ler_data_valida(string mensagem){
        string data;
        cout << mensagem;
        getline(cin, data);

        while(!data_completa_valida(data) or (data.empty())){
            cout << "A data inserida é inválida! Por favor use o formato DD/MM/AAAA, com dias e meses válidos: " << endl;
            getline(cin, data);
        }
        
        return data;
    }

    string data_na_linha(string l_atual) {
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
            return Data_do_Item;
        }
        return "";
    }

    string convertendo_para_ISO(string data){            
        if (data.length() != 10){
            return "";
        }
        return data.substr(6, 4) + "-" +
               data.substr(3, 2) + "-" +
               data.substr(0,2);
    }

    bool intervalos(string data_na_linha, string inicio_periodo, string fim_periodo){

        string ItemLinha = convertendo_para_ISO(data_na_linha);
        string InicioP = convertendo_para_ISO(inicio_periodo);
        string FinalP = convertendo_para_ISO(fim_periodo);

        return ((ItemLinha >= InicioP) and (ItemLinha <= FinalP));
    }    
};

struct Sistema_Estoque{
    void ver_estoque(){
        ifstream lendo_estoque("Roupas_em_Estoque.txt");
        string dados;
        cout << "======================== ITENS EM ESTOQUE ========================" << endl 
            << "--- Roupa ---- Tamanho ---- Descrição ---- Marca ---- Preço ---" << endl << endl;
            while (getline(lendo_estoque, dados)) {
                cout << dados << endl << endl;
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
        DataFuncoes funcaoData;
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
        inicio_data = funcaoData.ler_data_valida("Ver itens de (DD/MM/AAAA): ");
        fim_data = funcaoData.ler_data_valida("Até: ");
        cout << string(65, '-') << endl;

        cout << "ITENS ADICIONADOS DE: " << inicio_data << " ATÉ " << fim_data << ":" << endl;
        
        int enco_periodo = 0;
        for(int i = 0; i < tam_vetor; i++){
            string l_atual = roupas[i];

            string data_linha = funcaoData.data_na_linha(l_atual);

            if (!data_linha.empty()){
                if(funcaoData.intervalos(data_linha, inicio_data, fim_data)){
                    cout << l_atual << endl;
                    enco_periodo++;
                }
            }
        } 
        
        cout << endl << "Adicionados no período: " << enco_periodo << endl;
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

            lendo_estoque.close();
            lendo_estoque.open("Roupas_em_Estoque.txt");

            int tam_vetor = 0;
            string cont_aux;

            while(getline(lendo_estoque, cont_aux)){
                tam_vetor++;
            }
            lendo_estoque.close();

            string *roupas = new string[tam_vetor];
            bool *item_riscado = new bool[tam_vetor];

            lendo_estoque.open("Roupas_em_Estoque.txt");

            for (int i = 0; i < tam_vetor; i++){
                getline(lendo_estoque, roupas[i]);
                item_riscado[i] = false;
            }

            int escolha = -1;
            bool selecao = true;
            while (selecao){
                cout << "========== Digite o número do item para marcar/desmarcar exclusão ==========" << endl;
                cout <<"==================== Digite 0 para finalizar a operação ====================" << endl << endl;

                for (int i = 0; i < tam_vetor; i++){
                    string status_do_item;
                    
                    if (item_riscado[i] == true){
                        status_do_item = "[ X ] ";
                    }

                    else{
                        status_do_item = "[     ] ";
                    }

                    cout << i + 1 << ". " << status_do_item << roupas[i] << endl << endl;
                }
                
                cout << string(65, '=') << endl;
                cout << "Informe o número do item que deseja excluir: ";
                cin >> escolha;
                cout << string(65, '=') << endl;

                while((escolha <= -1) or (cin.fail())) {
                    cin.clear();  
                    cin.ignore(10000, '\n');  
                    cout << "Opção inválida, digite algo válido!!: ";
                    cin >> escolha;
                }

                if (escolha == 0){
                    selecao = false;
                }
                else if ((escolha > 0) and (escolha <= tam_vetor)){
                    if (item_riscado[escolha - 1] == true){
                        item_riscado[escolha - 1] = false;
                        cout << "--- Item tirado da exclusão! ---" << endl;
                    }
                    else {
                        item_riscado[escolha - 1] = true;
                        cout << "--- Item adicionado para exclusão ---" << endl;
                    }
                }

                else{
                    cout << string(50, '=') << endl;
                    cout << "Opção Inválida! Digite algo válido!" << endl;
                    cout << string(65, '=') << endl;
                }  
            }

        int lixeira = 0;
        
        for (int i = 0; i < tam_vetor; i++){
            if (item_riscado[i] == true){
            lixeira++;
            }
        }

        if (lixeira > 0){
            char confirmar;
            cout << endl << "Deseja excluir " << lixeira << " itens? (S/N): ";
            cin >> confirmar;
            cin.ignore(10000, '\n');
            
            if ((confirmar == 'S') or (confirmar == 's')){
                ofstream arquivo_atualizado("Roupas_em_estoque.txt");
                for (int i = 0; i < tam_vetor; i++){
                    if (item_riscado[i] == false){
                        arquivo_atualizado << roupas[i] << endl;
                    }
                }

                arquivo_atualizado.close();
                cout << "ALterações Salvas no Estoque!" << endl;
            }

            else{
                cout << "Operação cancelada!" << endl;
            }
        }
            
        delete[] roupas;
        delete[] item_riscado;
    }
};
 
int main(){
    int Sim_ou_Nao = 1;
    Sistema_Estoque sistema;
    
    while (Sim_ou_Nao == 1) {
        int opcao;
        string entrada;
        cout << menu();
        getline(cin, entrada);
        if (entrada.empty()){
            cout << string(65, '=') << endl;
            cout << setw(50) << "Insira algo válido!" << endl;
            cout << string(65, '=') << endl;
            cout << endl;
            continue;
        }

        stringstream ss(entrada);
        if(!(ss >> opcao)){
            cout << string(65, '=') << endl;
            cout << setw(45) << "Insira um NÚMERO!: " << endl;
            cout << string(65, '=') << endl;
            continue;
        }

        switch (opcao) {
            case 1:{
                Info_roupa roupa;
                roupa.adiciona_roupa();
                cout << endl << string(65, '=') << endl;
                cout << setw(45) << "Roupa Adicionada com Sucesso!!!" << endl;
                cout << string(65, '=') << endl;
                break;
            }
        
            case 2:
                sistema.ver_estoque();
                break;
            
            case 3:
                sistema.ver_quantidade();
                break;
            
            case 4: {
                cin.ignore();
                sistema.ver_intervalo_data();
                break;
            }

            case 5:
                sistema.excluir_num_item();
                break;
            
            default:
                cout << endl << "Opção Inválida!!!" << endl;
            
        }
        
        cout << endl << setw(47) << "Deseja realizar outra operação?" << endl; 
        cout << setw(50) << "Digite 1 para 'SIM' ou 2 para 'NÃO': ";
        
        string sim_ou_nao_entrada;
        bool se_valido = false;

        while(!se_valido){
            getline(cin, sim_ou_nao_entrada);

            if (sim_ou_nao_entrada.empty()){
                cout << string(65, '=') << endl;
                cout << setw(43) << "Valor inválido!" << endl;
                cout << string(65, '=') << endl;

                continue;
            }

           stringstream ss(sim_ou_nao_entrada);
            if (!(ss >> Sim_ou_Nao)){
                cout << string(65, '=') << endl;
                cout << setw(43) << "Digite apenas 1 ou 2! " << endl;
                cout << string(65, '=') << endl;
                cout << "Digite: ";
                continue;
            }

            if ((Sim_ou_Nao < 1) or (Sim_ou_Nao > 2)){
                cout << string(65, '=') << endl;
                cout << setw(43) << "Digite apenas 1 ou 2!" << endl;
                cout << string(65, '=') << endl;
                cout << "Digite: ";
                continue;
            }

            se_valido = true;
        }
    }

    cout << endl << "================= FIM DO PROGRAMA. OBRIGADO(A) ==================" << endl;

    return 0;
}
