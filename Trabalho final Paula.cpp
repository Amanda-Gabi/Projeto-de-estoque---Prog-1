#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>
using namespace std;

struct Info_roupa{

    int id;
    string Tipo;
    string Tamanho;
    string descricao;
    string marca;
    float valor;
    string data;
    bool excluido;
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

    string convertendo_para_ISO(string data){            
        if (data.length() != 10){
            return "";
        }
        return data.substr(6, 4) + "-" +
               data.substr(3, 2) + "-" +
               data.substr(0,2);
    }

    bool intervalos(string data_na_linha, string inicio_periodo, string fim_periodo){

        string ItemISO = convertendo_para_ISO(data_na_linha);
        string InicioISO = convertendo_para_ISO(inicio_periodo);
        string FinalISO = convertendo_para_ISO(fim_periodo);

        return ((ItemISO >= InicioISO) and (ItemISO <= FinalISO));
    }    
};

struct Sistema_Estoque{
    Info_roupa *roupas;
    int capacidade;
    int total;

    void inicializar_estoque(){
        capacidade = 40;
        total = 0;
        roupas = new Info_roupa[capacidade];
    }

    void aumentar_estoque(){
        int capacidade_nova = capacidade + 10;
        Info_roupa *vetor_novo = new Info_roupa[capacidade_nova];

        for (int i = 0; i < total; i++) {
            vetor_novo[i] = roupas[i];
        }

        delete[] roupas;
        roupas = vetor_novo;
        capacidade = capacidade_nova;
    }

    void carregar_itens_do_arquivo(){
        ifstream lendo_arquivo("Roupas_em_Estoque.txt");
          if(!lendo_arquivo){
                cout << string(65, '=') << endl;
                cout << setw(35) << "ARQUIVO NÃO ENCONTRADO!" << endl;
                cout << string(65, '=') << endl;
                return;
            }

        string linha;
        while(getline(lendo_arquivo, linha)){
            if(!linha.empty()){
                if(total >= capacidade){
                    aumentar_estoque();
                }

                stringstream ss(linha);

                getline(ss, roupas[total].Tipo, '|');
                getline(ss, roupas[total].Tamanho, '|');
                getline(ss, roupas[total].descricao, '|');
                getline(ss, roupas[total].marca, '|');

                string valor_str;
                getline(ss, valor_str, '|');
                
                int posicao_R = -1;
                int cont = 0;

                while((cont < valor_str.length()) and (posicao_R == -1)){
                    if (valor_str[cont] == 'R'){
                        posicao_R = cont;
                    }
                    cont++;
                }

                if (posicao_R != -1){
                    string numero_str = valor_str.substr(posicao_R + 2);
                    roupas[total].valor = stof(numero_str);
                }

                getline(ss, roupas[total].data, '|');

                roupas[total].id = total + 1;
                roupas[total].excluido = false;
                total++;
 
            }      
            
        }

        lendo_arquivo.close();
        cout << "Carregados " << total << "itens para o vetor." << endl;
    }

    void salvar_no_arquivo(){
        ofstream salvando_roupa("Roupas_em_Estoque.txt");
        
        int salvos = 0;
        for (int i = 0; i < total; i++){
            if(!roupas[i].excluido){
                salvando_roupa << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | "
                << roupas[i].descricao << " | " << roupas[i].marca << " | " 
                << "R$" << fixed << setprecision(2) << roupas[i].valor << " | " << roupas[i].data << endl;
                salvos++;
            }
        }

        salvando_roupa.close();
        cout << "Foram salvas " << salvos << " roupas no estoque!" << endl;
    }

    void adicionando_roupa_nova(){
        if(total >= capacidade){
            aumentar_estoque();
        }
         
        cout << "Diga que Tipo de roupa é (ex: Camisa, calca, sapato, etc...): ";
        getline(cin, roupas[total].Tipo);
        while (roupas[total].Tipo.empty()) {
            cout << "Campo Obrigatório! Infome o Tipo de roupa: ";
            getline(cin, roupas[total].Tipo);
        }

        cout << "Agora o Tamanho da roupa (ex: M, G, P, etc..): ";
        getline(cin, roupas[total].Tamanho);
        while (roupas[total].Tamanho.empty()) {
            cout << "Campo Obrigatório! Infome o Tamanho da roupa: ";
            getline(cin, roupas[total].Tamanho);
        }

        cout << "Agora dê uma breve Descrição sobre a roupa: ";
        getline(cin, roupas[total].descricao);
        while (roupas[total].descricao.empty()) {
            cout << "Campo Obrigatório! Infome a Descrição da roupa (se não souber ponha 'desconhecida'): ";
            getline(cin, roupas[total].descricao);
        }
        cout << "Agora diga a Marca da roupa: ";
        getline(cin, roupas[total].marca);
        while (roupas[total].marca.empty()) {
            cout << "Campo Obrigatório! Infome a Marca da roupa: ";
            getline(cin, roupas[total].marca);
        }
        cout << "Agora o valor da roupa: R$";
        cin >> roupas[total].valor;
        while((roupas[total].valor <= 0) or (cin.fail())) {
            cin.clear();  
            cin.ignore(10000, '\n');  
            cout << "Valor inválido! Informe um Valor válido: ";
            cin >> roupas[total].valor;
        }
        cin.ignore();

       DataFuncoes Fun_data;
       roupas[total].data = Fun_data.ler_data_valida("Informe a Data (DD/MM/AAAA): ");

       roupas[total].id = total + 1;
       roupas[total].excluido = false;

       total++;

       cout << endl << string(65, '=') << endl;
       cout << setw(45) << "Roupa adicionada com Sucesso ao estoque!!" << endl;
       cout << string(65, '=') << endl;
    }

    void ver_estoque(){
        if (total == 0){
            cout << string(65, '=') << endl;
            cout << setw(35) << "ESTOQUE VAZIO ADICIONE UMA ROUPA!!" << endl;
            cout << string(65, '=') << endl;
            return;
        }

        cout << "======================== ITENS EM ESTOQUE ========================" << endl 
        << "--- Roupa ---- Tamanho ---- Descrição ---- Marca ---- Preço ---" << endl << endl;
        for (int i = 0; i <  total; i++){
            if(!roupas[i].excluido){
                cout << roupas[i].id << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " 
                << roupas[i].descricao << " | " << roupas[i].marca << " | " << "R$" << fixed << setprecision(2) 
                << roupas[i].valor << " | " << "R$" << roupas[i].data << endl << endl;
            }
        }
        cout << string(65, '=') << endl;
    }
    
    void ver_quantidade(){
        int linhas_ativas = 0;
        for (int i = 0; i < total; i++){
            if(!roupas[i].excluido){
                linhas_ativas++;
            }
        }

        cout << string(65, '=') << endl;
        cout << setw(40) << "ITENS CADASTRADOS" << endl;
        cout << string(65, '-') << endl;
        cout << setw(34) << "Total: " << linhas_ativas << endl;
        cout << string(65, '=') << endl;
    }
    
    void ver_intervalo_data(){
        if (total == 0){
            cout << string(65, '=') << endl;
            cout << setw(35) << "ESTOQUE VAZIO ADICIONE UMA ROUPA!!" << endl;
            cout << string(65, '=') << endl;
            return;
        }
        
        DataFuncoes Fun_data;
        string inicio_data, fim_data;
        cout << string(65, '=') << endl;
        inicio_data = Fun_data.ler_data_valida("Ver itens de (DD/MM/AAAA): ");
        fim_data = Fun_data.ler_data_valida("Até: ");
        cout << string(65, '-') << endl;

        cout << "ITENS ADICIONADOS DE: " << inicio_data << " ATÉ " << fim_data << ":" << endl;
        
        int enco_periodo = 0;
        for(int i = 0; i < total; i++){
           if ((!roupas[i].excluido) and (Fun_data.intervalos(roupas[i].data, inicio_data, fim_data))){
            cout << roupas[i].id << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
            << roupas[i].marca << " | " << "R$" << roupas[i].valor << " | " << roupas[i].data << endl;
            enco_periodo++;
           }
        } 
        
        cout << endl << "Adicionados no período: " << enco_periodo << endl;
        cout << string(65, '=');
    }

    void ver_por_preco(){
        if (total == 0){
            cout << string(65, '=') << endl;
            cout << setw(35) << "ESTOQUE VAZIO ADICIONE UMA ROUPA!!" << endl;
            cout << string(65, '=') << endl;
            return;
        }

        float valor_minimo, valor_maximo;
        cout << string(65, '=') << endl;
        cout << "Informe o preço minímo para procura: R$";
        cin >> valor_minimo;
        cout << string(65, '-') << endl;
        
        while((valor_minimo < 0) or (cin.fail())){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "O valor informado não é válido!" << endl;
            cout << "Informe um valor válido: R$";
            cin >> valor_minimo;
            cout << endl << string(65, '-') << endl;
        }

        cout << "Informe o preço máximo para procura: R$";
        cin >> valor_maximo;
        cout << string(65, '=') << endl;

        while((valor_maximo < 0) or (cin.fail())){
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "O valor informado não é válido!" << endl;
            cout << "Informe um valor válido: R$";
            cin >> valor_maximo;
        }

        cin.ignore();

        cout << string(65, '~') << endl;
        cout << "Itens encontrados com o valor de " << valor_minimo << "à " << valor_maximo << endl;

        int enco_periodo = 0;
        for(int i = 0; i < total; i++){
           if ((!roupas[i].excluido) and (roupas[i].valor >= valor_minimo) and ((roupas[i].valor <= valor_maximo))){
            cout << roupas[i].id << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
            << roupas[i].marca << " | " << "R$" << roupas[i].valor << " | " << roupas[i].data << endl;
            enco_periodo++;
           }
        }

        cout << endl << "Encontrados na faixa de preço: " << enco_periodo << endl;
        cout << string(65, '=') << endl;

    }
    void excluir_item() {
        if (total == 0){
            cout << string(65, '=') << endl;
            cout << setw(35) << "ESTOQUE VAZIO ADICIONE UMA ROUPA!!" << endl;
            cout << string(65, '=') << endl;
            return;
        }

        bool continuar = true;
        int escolha;
        while (continuar){
                cout << "========== Digite o número do item para marcar/desmarcar exclusão ==========" << endl;
                cout <<"==================== Digite 0 para finalizar a operação ====================" << endl << endl;

                for (int i = 0; i < total; i++){
                    cout << i + 1 << ". ";
                    if (roupas[i].excluido){
                        cout << "[ X ] ";
                    }

                    else{
                        cout << "[    ] ";
                    }

                    cout << roupas[i].id << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
                    << roupas[i].marca << " | "  << "R$" << roupas[i].valor << " | " << roupas[i].data << endl;

                }
                
                cout << string(65, '=') << endl;
                cout << "Informe o número do item que deseja excluir: ";
                cin >> escolha;
                cout << string(65, '=') << endl;

                while((escolha < -1) or (cin.fail())) {
                    cin.clear();
                    cin.ignore(10000, '\n');
                    cout << "Opção Inválida! Digite algo válido: ";
                    cin >> escolha;
                }

                if (escolha == 0){
                    continuar = false;
                }

                else if ((escolha > 0) and (escolha <= total)){
                    if (roupas[escolha - 1].excluido){
                        roupas[escolha - 1].excluido = false;
                        cout << "--- Item tirado da exclusão! ---" << endl;
                    }
                    else {
                        roupas[escolha - 1].excluido = true;
                        cout << "--- Item adicionado para exclusão ---" << endl;
                    }
                }

                else{
                    cout << "Opção inválida! Digite algo válido!" << endl;
                }  
            }

        int lixeira = 0;
        
        for (int i = 0; i < total; i++){
            if (roupas[i].excluido){
            lixeira++;
            }
        }

        if (lixeira > 0){
            char confirmar;
            cout << endl << "Deseja excluir " << lixeira << " itens? (S/N): ";
            cin >> confirmar;
            cin.ignore(10000, '\n');
            
            if ((confirmar == 'S') or (confirmar == 's')){
                cout << "Itens adicionados à lixeira" << endl;
            }

            else{
                cout << "Operação cancelada!" << endl;
                for (int i = 0; i < total; i++){
                    roupas[i].excluido = false;
                }
            }
        }
    }            
    
    void ver_por_ID(){
        if (total == 0){
            cout << string(65, '=') << endl;
            cout << setw(35) << "ESTOQUE VAZIO ADICIONE UMA ROUPA!!" << endl;
            cout << string(65, '=') << endl;
            return;
        }

        int ID_inicial, ID_final;
        cout << "INFORME A BAIXO O ID DE ONDE VC DESEJA INICIAR A BUSCA";
        cout << "ID de início: ";
        cin >> ID_inicial;
        cout << "E agora informe o ID final da busca: ";
        cin >> ID_final;
        cin.ignore();

        if(ID_inicial < 1){
            ID_inicial = 1;
        }
        
        if(ID_final > total){
            ID_final = total;
        }

        if (ID_inicial > total){
            cout << "Intervalo Inválido!" << endl;
            return;
        }

        cout << string(65, '=') << endl;
        cout << setw(35) << "Itend do ID " << ID_inicial << "ao " << ID_final << endl;
        cout << string(65, '-') << endl;
        
        for (int i = ID_inicial - 1; i < ID_final; i++){
            cout << roupas[i].id;
            if (roupas[i].excluido){
                cout << "[EXCLUIDO]";
            }

            cout << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
                    << roupas[i].marca << " | "  << "R$" << roupas[i].valor << " | " << roupas[i].data << endl;
        }

        cout << string(65, '=') << endl; 
    }

    void liberar_memoria(){
        delete[] roupas;
    }

};

string menu() {

    return "Digite o número da operação desejada abaixo: \n"
            "1. Adicionar roupa ao estoque\n"
            "2. Ver itens do estoque\n" 
            "3. Ver quantidade de roupas no estoque\n"
            "4. Pesquisar por... \n"
            "5. Excluir itens do estoque \n"
            "6. Ver por faixa de ID\n"
            "0. Sair\n"
            "Digite aqui: ";
}
 
int main(){

    Sistema_Estoque sistema;
    sistema.inicializar_estoque();
    sistema.carregar_itens_do_arquivo();

    int opcao = -1;
    string entrada;

    while(opcao != 0){
        cout << menu();
        getline(cin, entrada);

        stringstream ss(entrada);

        if (entrada.empty()){
            cout << string(65, '=') << endl;
            cout << setw(35) << "INSIRA ALGO VÁLIDO!" << endl;
            cout << string(65, '=') << endl << endl;
        }
        
        else if (!(ss >> opcao)){
            cout << string(65, '=') << endl;
            cout << setw(35) << "INSIRA UM NÚMERO!" << endl;
            cout << string(65, '=') << endl << endl;
        }

        else{
            switch (opcao) {
                case 1:
                    sistema.adicionando_roupa_nova();
                    break;
                case 2:
                    sistema.ver_estoque();
                    break;
                
                case 3:
                    sistema.ver_quantidade();
                    break;
                
                case 4: {
                    int opcao_secundaria;
                    string entrada_da_secundaria;

                    cout << string(65, '=') << endl;
                    cout << "4. Pesquisar por: " << endl;
                    cout << "1. Período de Data." << endl;
                    cout << "2. Faixa de Preço." << endl;
                    cout << "Informe: ";
                    getline(cin, entrada_da_secundaria);

                    stringstream ss_pesquisa(entrada_da_secundaria);
                    if(!(ss_pesquisa >> opcao_secundaria)){
                        cout << string(65, '=') << endl;
                        cout << setw(50) << "Insira um número!" << endl;
                        cout << string(65, '=') << endl;
                    }

                    else{
                        switch(opcao_secundaria){
                            case 1:
                                sistema.ver_intervalo_data();
                                break;
                            
                            case 2:
                                sistema.ver_por_preco();
                                break;

                            default:
                                cout << "Opção Inválida!" << endl;
                        }
                    }
                    break;
                }

                case 5:
                    sistema.excluir_item();
                    break;
                
                case 6:
                    sistema.ver_por_ID();
                    break;

                case 0: {
                    int salvar;
                    cout << endl << setw(47) << "Deseja salvar sua atividade antes de sair?" << endl;
                    cout << setw(50) << "Digite 1 para 'SIM' ou 2 para 'NÂO': ";
                    cin >> salvar;
                    cin.ignore();

                    if (salvar == 1) {
                        sistema.salvar_no_arquivo();
                    }
                    cout << endl << "================= FIM DO PROGRAMA. OBRIGADO(A) ==================" << endl;
                    break;
                }

                default:
                    cout << endl << "Opção Inválida!" << endl;
                
            }
        }
    }

    return 0;
}
