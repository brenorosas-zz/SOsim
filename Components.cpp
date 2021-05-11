#include "header.cpp"
#include "pages_algorithms.cpp"
struct Pagina {
    int idPagina;
    int idProcessoAssociado;
    Pagina() {}
    Pagina(int idPagina, int idProcesso) {
        this->idPagina = idPagina;
        this->idProcessoAssociado = idProcesso;
    }
    bool operator==(const Pagina p) const{
        return this->idPagina == p.idPagina and this->idProcessoAssociado == p.idProcessoAssociado;
    }
};
struct Processo {
    int tempoDeChegada;
    int tempoDeExecucao;
    int deadline;
    int prioridade;
    int quantidadeDePaginas;
    int idProcesso;
    int paginasProntas = 0;
    vector<Pagina> paginas;
    Processo(int tempoDeChegada, int tempoDeExecucao, int deadline,
             int prioridade, int quantidadeDePaginas, int idProcesso) {
        this->tempoDeChegada = tempoDeChegada;
        this->tempoDeExecucao = tempoDeExecucao;
        this->deadline = deadline;
        this->prioridade = prioridade;
        this->quantidadeDePaginas = quantidadeDePaginas;
        this->idProcesso = idProcesso;
        for (int i = 0; i < quantidadeDePaginas; i++) {
            paginas.push_back(Pagina(i, idProcesso));
        }
    }
    bool pronto() { return paginasProntas == quantidadeDePaginas; }
};
struct DISCO {
    vector<Pagina> disco;
    DISCO(){};
    bool existe(Pagina pagina) {
        for (auto &paginaD : disco)
            if (pagina == paginaD) return true;
        return false;
    }
    void add(Pagina pagina) { disco.push_back(pagina); }
    void remove(Pagina pagina) {
        for (int i = 0; i < disco.size(); i++) {
            if (pagina == disco[i]) {
                disco.erase(i + disco.begin());
                break;
            }
        }
    }
};
struct RAM {
    Pagina ram[50];
    int proximo = 0;
    vector<int> queueEDF;
    RAM() {
        for (int i = 0; i < 50; i++) {
            ram[i] = Pagina(i, -1);
            queueEDF.push_back(i);
        }
    }
    bool existe(Pagina pagina) {
        for (int i = 0; i < 50; i++)
            if (pagina == ram[i]) return true;
        return false;
    }
    Pagina addFIFO(Pagina pagina) {
        Pagina aux = ram[proximo];
        ram[proximo] = pagina;
        proximo = (proximo + 1) % 50;
        return aux;
    }
    Pagina addMRU(Pagina pagina) {
        Pagina aux = ram[queueEDF[0]];
        ram[queueEDF[0]] = pagina;
        queueEDF.push_back(queueEDF[0]);
        queueEDF.erase(queueEDF.begin());
        return aux;
    }
    void updateMRU(Pagina pagina){
        for(int i = 0; i < 50; i++){
            if(ram[queueEDF[i]] == pagina){
                queueEDF.push_back(queueEDF[i]);
                queueEDF.erase(queueEDF.begin() + i);
                break;
            }
        }
    }
};
struct MaquinaVirtual {
    DISCO disco;
    RAM ram;
    vector<Processo> processos;
    string escalonador, paginacao;
    MaquinaVirtual() {
        ram = RAM();
        disco = DISCO();
    }
    void addProcesso(Processo processo) { processos.push_back(processo); }
    void page(vector<Pagina> &paginas) {
        for (Pagina &pagina : paginas) {
            if (!ram.existe(pagina)) {
                Pagina aux;
                if(paginacao == "FIFO")
                    aux = ram.addFIFO(pagina);
                if(paginacao == "MRU")
                    aux = ram.addMRU(pagina);
                if (aux.idPagina != -1) {
                    disco.add(aux);
                }
                disco.remove(pagina);
            }
            else if(paginacao == "MRU"){
                ram.updateMRU(pagina);
            }
        }
    }
    void execute_processo(Processo &processo, int tempo) {
        page(processo.paginas);
        processo.paginasProntas = processo.paginas.size();
        sleep(tempo*1000);
    }
    void FIFO() {
        if (processos.size() == 0) return;
        for (Processo &processo : processos) {
            execute_processo(processo, processo.tempoDeExecucao);
        }
    }
    void rodar() {
        cin >> escalonador >> paginacao;
        if (escalonador == "FIFO") {
            FIFO();
        } 
        // else if (escalonador == "SJF") {
        //     SJF();
        // } else if (escalonador == "RoundRobin") {
        //     RoundRobin();
        // } else if (escalonador == "EDF") {
        //     EDF();
        // }
    }
};