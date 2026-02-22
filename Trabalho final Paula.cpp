// ============================================
// SISTEMA DE CONTROLE DE ESTOQUE DE ROUPAS
// ============================================
// Membros da equipe:
// Amanda Gabriella Silva Borges do Nascimento - 202511140035
// Didimo Luan Neves da Silva - 202311140016
// Eduardo Araújo de Carvalho - 202411140046
// 
// Disciplina: Programação de Computadores I
// Professora: Dra. Paula Christina Figueira Cardoso
// Curso: Sistemas de Informação
// Universidade Federal do Pará - Campus Belém
//
// Data: 21 de fevereiro de 2026
// ============================================
// 
// Descrição: Sistema para gerenciamento de estoque de roupas
// com operações de cadastro, visualização, busca por período
// e faixa de preço, exclusão lógica e persistência em arquivo texto.
// ===================================================================

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
    bool excluido; // nosso flag (marcador) de exclusão, se "true" será excluído
};

struct DataFuncoes{
    // verifica se a data informada tem 10 caracteres e barras corretas
    bool formato_de_data_valido(string data){
        if (data.length() != 10){
            return false;
        }
        if ((data[2] != '/') or (data[5] != '/')){
            return false;
        }
        
        // verifica se essas posições são de fato dígitos
        int posicoes_validas[] = {0, 1, 3, 4, 6, 7, 8, 9};

        for (int posicao = 0; posicao < 8; posicao++){
            int valido = posicoes_validas[posicao];
            if (!isdigit(data[valido])){
                return false;
            }
        }

        return true;
    }

    // vai converter nossa data para inteiros e também valida os valores
    bool valores_de_data_validos(string data){
        if (!formato_de_data_valido(data)){
            return false;
        }

        // stoi vai pegar a posição e a quantidade de caracteres e trasformar a string em inteiros
        int dia = stoi(data.substr(0, 2)); //substr pega na posição 0 os 2 primeiros caracteres
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

    // converte a data padrão (DD/MM/AAAA) para o formato ISO (AAAA/MM/DD) para comparar
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
    Info_roupa *roupas; // ponteiro para o vetor dinâmico
    int capacidade;
    int total;

    void inicializar_estoque(){
        capacidade = 40;
        total = 0;
        roupas = new Info_roupa[capacidade]; //alocação dinâmica
    }

    // vai aumentar o tamanho do vetor forme a necessidade
    void aumentar_estoque(){
        int capacidade_nova = capacidade + 10;
        Info_roupa *vetor_novo = new Info_roupa[capacidade_nova];

        // passa os dados do vetor antigo pro novo
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

                // extraímos o número depois do "R$" o substr pega um pedaço da string 
                // o stof pega a string e converte em float
                // necessário pois como no arquivo o valor está como string, precisamos dele
                // como float para comparar valores na busca de preço
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
    }

    void salvar_no_arquivo(){
        ofstream salvando_roupa("Roupas_em_Estoque.txt");
        
        int salvos = 0;
        for (int i = 0; i < total; i++){
            if(!roupas[i].excluido){ // só são salvos itens não excluídos
                salvando_roupa << roupas[i].Tipo << "|" << roupas[i].Tamanho << "|"
                << roupas[i].descricao << "|" << roupas[i].marca << "|" 
                << "R$" << fixed << setprecision(2) << roupas[i].valor << "|" << roupas[i].data << endl;
                salvos++;
            }
        }

        salvando_roupa.close();
        cout << endl << "O estoque conta com " << salvos << " roupas!" << endl;
    }

    void adicionando_roupa_nova(){
        if(total >= capacidade){
            aumentar_estoque();
        }
        
        cout << string(65, '=') << endl;
        cout << endl << "Diga que Tipo de roupa é (ex: Camisa, calca, sapato, etc...): ";
        getline(cin, roupas[total].Tipo);
        while (roupas[total].Tipo.empty()) { // o empty verifica se a string está vazia
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
            cout << "Campo Obrigatório! Infome a Descrição da roupa: ";
            getline(cin, roupas[total].descricao);
        }
        cout << "Agora diga a Marca da roupa: ";
        getline(cin, roupas[total].marca);
        while (roupas[total].marca.empty()) {
            cout << "Campo Obrigatório! Infome a Marca da roupa (se não souber ponha 'desconhecida'): ";
            getline(cin, roupas[total].marca);
        }

        cout << "Agora o valor da roupa: R$";
        string valor_str;
        bool valor_valido = false;

        while(!valor_valido) {
            getline(cin, valor_str);
            
            if(valor_str.empty()) {
                cout << "Valor não pode ser vazio! Informe um valor válido: R$";
                continue;
            }
        
            stringstream ss(valor_str);
            if(ss >> roupas[total].valor) {
 
                if(roupas[total].valor > 0) {
                    valor_valido = true;
                } else {
                    cout << "Valor deve ser positivo! Informe um valor válido: R$";
                }
            } else {
                cout << "Valor inválido! Informe um número: R$";
            }
        }
       DataFuncoes Fun_data;
       roupas[total].data = Fun_data.ler_data_valida("Informe a Data (DD/MM/AAAA): ");

       roupas[total].id = total + 1;
       roupas[total].excluido = false;

       total++;

       cout << endl << string(65, '=') << endl;
       cout << setw(50) << "Roupa adicionada com Sucesso ao estoque!!" << endl;
       cout << string(65, '=') << endl;
    }

    void ver_estoque(){
        if (total == 0){
            cout << string(65, '=') << endl;
            cout << setw(50) << "ESTOQUE VAZIO ADICIONE UMA ROUPA!!" << endl;
            cout << string(65, '=') << endl;
            return;
        }

        cout << "======================== ITENS EM ESTOQUE ========================" << endl 
        << "--- Roupa ---- Tamanho ---- Descrição ---- Marca ---- Preço ---" << endl << endl;
        for (int i = 0; i <  total; i++){
            if(!roupas[i].excluido){
                cout << roupas[i].id << ". " << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " 
                << roupas[i].descricao << " | " << roupas[i].marca << " | " << "R$" << fixed << setprecision(2) 
                << roupas[i].valor << " | " << roupas[i].data << endl;
            }
            else{
                cout << roupas[i].id << ". [EXCLUIDO] - ";
                cout << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " 
                << roupas[i].descricao << " | " << roupas[i].marca << " | " << "R$" << fixed << setprecision(2) 
                << roupas[i].valor << " | " << roupas[i].data << endl;
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
            // usa a função intervalos() da struct DataFunções para fazer a comparação de datas
            if ((Fun_data.intervalos(roupas[i].data, inicio_data, fim_data))){
                cout << roupas[i].id;
                if(roupas[i].excluido){
                    cout << "[EXCLUIDO] - ";
                }
                cout << roupas[i].Tipo << ". " << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
                << roupas[i].marca << " | " << "R$" << fixed << setprecision(2) <<  roupas[i].valor << " | " << roupas[i].data << endl;
                enco_periodo++;
            }
        } 
        
        cout << endl << "Adicionados no período: " << enco_periodo << endl;
    }

    void ver_por_preco(){
        if (total == 0){
            cout << string(65, '=') << endl;
            cout << setw(50) << "ESTOQUE VAZIO ADICIONE UMA ROUPA!!" << endl;
            cout << string(65, '=') << endl;
            return;
        }

        float valor_minimo, valor_maximo;
        string entrada;
        bool valida = false;

        cout << string(65, '=') << endl;
        while(!valida){
            cout << "Informe o preço minímo para procura: R$";
            getline(cin, entrada);

            if (entrada.empty()){
                cout << endl << "Você precisa adicionar um valor!" << endl;
            }

            else{
                stringstream ss(entrada);
                if (!(ss >> valor_minimo)){
                    cout << "Valor inválido! Digite um número!" << endl;
                }

                else if(valor_minimo < 0){
                    cout << "Valor inválido! O valor do item não pode ser negativo!" << endl;
                }

                else{
                    valida = true;
                }
            }
        }
        
        cout << string(65, '-') << endl;

        valida = false;
        while(!valida){
            cout << "Informe o preço máximo para procura: R$";
            getline(cin, entrada);

            if (entrada.empty()){
                cout << endl << "Você precisa adicionar um valor!" << endl;
            }

            else{
                stringstream ss(entrada);
                if (!(ss >> valor_maximo)){
                    cout << "Valor inválido! Digite um número!" << endl;
                }

                else if(valor_maximo< 0){
                    cout << "Valor inválido! O valor do item não pode ser negativo!" << endl;
                }

                else if(valor_maximo < valor_minimo){
                    cout << "O valor máximo deve ser igual ou maior que o valor minímo!" << endl;
                }

                else{
                    valida = true;
                }
            }

            cout << string(65, '~') << endl;
            cout << "Itens encontrados com o valor de R$" << valor_minimo << " à " << valor_maximo << endl;

            int enco_periodo = 0;
            // verifica se o preço está dentro da faixa de busca
            for(int i = 0; i < total; i++){
                if ((!roupas[i].excluido) and (roupas[i].valor >= valor_minimo) and ((roupas[i].valor <= valor_maximo))){
                    cout << roupas[i].id << ". ";
                    if(roupas[i].excluido){
                        cout << "[EXCLUÍDO] - ";
                    }
                    cout << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
                    << roupas[i].marca << " | " << "R$" << fixed << setprecision(2) << roupas[i].valor << " | " << roupas[i].data << endl;
                    enco_periodo++;
                }
            }

            cout << endl << "Encontrados na faixa de preço: " << enco_periodo << endl;
            cout << string(65, '=') << endl;
        }
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
            cout << endl << string(65, '=') << endl;
            cout << "========== Digite o número do item para marcar/desmarcar exclusão ==========" << endl;
            cout <<"==================== Digite 0 para finalizar a operação ====================" << endl << endl;

            // mostra se o item está ou não excluído
            for (int i = 0; i < total; i++){
                cout << i + 1 << ". ";
                if (roupas[i].excluido){
                        cout << "[ X ] ";
                }

                else{
                    cout << "[   ] ";
                }

                cout << " " << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
                << roupas[i].marca << " | "  << "R$" << fixed << setprecision(2) << roupas[i].valor << " | " << roupas[i].data << endl;

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
                    cout << endl << "--- Item tirado da exclusão! ---" << endl;
                }
                else {
                    roupas[escolha - 1].excluido = true;
                    cout << endl << "--- Item adicionado para exclusão ---" << endl;
                }
            }

            else{
                cout << "Opção inválida! Digite algo válido!" << endl;
            }  
        }

        cin.ignore(10000, '\n');

        int lixeira = 0;
        
        for (int i = 0; i < total; i++){
            if (roupas[i].excluido){
            lixeira++;
            }
        }

        if (lixeira > 0){
            char confirmar;
            bool R_valida = false;
            while(!R_valida){
                cout << endl << "Deseja excluir " << lixeira << " itens? (S/N): ";
                cin >> confirmar;
                cin.ignore(10000, '\n');

                if((confirmar == 'S') or (confirmar == 's') or (confirmar == 'N') or (confirmar == 'n')){
                    R_valida = true;

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
                
                else{
                    cout << "Opção Inválida! Digite apenas S ou N." << endl;
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
        cout << "INFORME A BAIXO O ID DE ONDE VC DESEJA INICIAR A BUSCA" << endl;
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
            cout << roupas[i].id << ". ";
            if (roupas[i].excluido){
                cout << "[EXCLUIDO] - ";
            }

            cout << roupas[i].Tipo << " | " << roupas[i].Tamanho << " | " << roupas[i].descricao << " | "
                    << roupas[i].marca << " | "  << "R$" << fixed << setprecision(2) << roupas[i].valor << " | " << roupas[i].data << endl;
        }

        cout << string(65, '=') << endl; 
    }

    // libera a memoria que alocamos dinamicamente
    void liberar_memoria(){
        delete[] roupas;
    }

};

string menu() {
    stringstream ss;
    ss << string(65, '=') << endl;
    ss << setw(47) << "SISTEMA DE CONTROLE DE ESTOQUE" << endl;
    ss << string(65, '=') << endl;
    ss << "1. Adicionar Roupas ao Estoque." << endl;
    ss << "2. Ver Roupas no Estoque." << endl;
    ss << "3. Ver Quantidade de Roupas no Estoque." << endl;
    ss << "4. Pesquisar Roupas por..." << endl;
    ss << "5. Excluír Roupas do Estoque." << endl;
    ss << "6. Ver Roupas por ID (posições) no Estoque." << endl;
    ss << "0. Sair/Salvar." << endl;
    ss << string(65, '=') << endl;
    ss << "Digite aqui: ";
    return ss.str();
}
 
int main(){

    Sistema_Estoque sistema;    

    //inicializamos nosso vetor e carregamos os dados do arquivo pra ele         
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
                    cout << endl << setw(50) << "Deseja salvar sua atividade antes de sair?" << endl;
                    cout << setw(47) << "Digite 1 para 'SIM' ou 2 para 'NÂO': ";
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

    sistema.liberar_memoria();
    return 0;
}
