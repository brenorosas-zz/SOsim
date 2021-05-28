#include "header.cpp"
struct Pagina {
    int idPagina;
    int idProcessoAssociado;
    Pagina() {}
    Pagina(int idPagina, int idProcesso) {
        this->idPagina = idPagina;
        this->idProcessoAssociado = idProcesso;
    }
    bool operator==(const Pagina p) const {
        return this->idPagina == p.idPagina and
               this->idProcessoAssociado == p.idProcessoAssociado;
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
    bool estourou = false;
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
    DISCO() {
        disco.resize(100);
        for (int i = 0; i < 100; i++) {
            disco[i] = Pagina(i, -1);
        }
    };
    bool existe(Pagina pagina) {
        for (auto &paginaD : disco)
            if (pagina == paginaD) return true;
        return false;
    }
    void add(Pagina pagina) { disco.push_back(pagina); }
    void remove(Pagina pagina) {
        for (int i = 0; i < disco.size(); i++) {
            if (pagina == disco[i]) {
                cout << "Removendo do disco a pagina " << pagina.idPagina << ' '
                     << pagina.idProcessoAssociado << endl;
                disco.erase(i + disco.begin());
                break;
            }
        }
        sleep(1);
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
    void updateMRU(Pagina pagina) {
        for (int i = 0; i < 50; i++) {
            if (ram[queueEDF[i]] == pagina) {
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
    int quantum;
    int sobrecarga;
    vector<Processo> processos;
    string escalonador, paginacao;
    int tempo;
    MaquinaVirtual() {
        ram = RAM();
        disco = DISCO();
        tempo = 0;
    }
    void addProcesso(Processo processo) { processos.push_back(processo); }
    void page(vector<Pagina> &paginas) {
        for (Pagina &pagina : paginas) {
            cout << "paginando pagina " << pagina.idPagina << ' '
                 << pagina.idProcessoAssociado << endl;
            if (!ram.existe(pagina)) {
                cout << "Buscando pagina " << pagina.idPagina << ' '
                     << pagina.idProcessoAssociado << " no disco" << endl;
                Pagina aux;
                if (paginacao == "FIFO") aux = ram.addFIFO(pagina);
                if (paginacao == "MRU") aux = ram.addMRU(pagina);
                if (aux.idProcessoAssociado != -1) {
                    cout << "Removeu a pagina " << aux.idPagina << ' '
                         << aux.idProcessoAssociado << " da Ram" << endl;
                    disco.add(aux);
                }
                tempo++;
                disco.remove(pagina);
            } else if (paginacao == "MRU") {
                ram.updateMRU(pagina);
            }
        }
    }
    void execute_processo(Processo &processo, int tempoDeExecucao) {
        cout << "Preparando processo " << processo.idProcesso
             << " para execução" << endl;
        page(processo.paginas);
        processo.paginasProntas = processo.paginas.size();
        cout << "Processo pronto para executar" << endl;
        sleep(tempoDeExecucao);
        cout << "Processo " << processo.idProcesso << " executado" << endl;
        processo.tempoDeExecucao -= tempoDeExecucao;
        tempo += tempoDeExecucao;
    }
    static bool cmpFIFO(Processo a, Processo b) {
        return a.tempoDeChegada < b.tempoDeChegada;
    }
    double FIFO() {
        double somaDeTurnaround = 0;
        for (Processo &processo : processos) {
            if (processo.tempoDeChegada > tempo) {
                cout << "Aguardando próximo processo" << endl;
                sleep(processo.tempoDeChegada - tempo);
                tempo = processo.tempoDeChegada;
            }
            execute_processo(processo, processo.tempoDeExecucao);
            if (tempo > processo.deadline) {
                processo.estourou = true;
            }
            somaDeTurnaround += (tempo - processo.tempoDeChegada);
        }
        return somaDeTurnaround / (int)processos.size();
    }
    struct cmpSJF : binary_function<Processo, Processo, bool> {
        bool operator()(const Processo &a, const Processo &b) const {
            return a.tempoDeExecucao > b.tempoDeExecucao;
        }
    };
    double SJF() {
        double somaDeTurnaround = 0;
        int at = 0;
        priority_queue<Processo, vector<Processo>, cmpSJF> queue;
        while (!queue.empty() or at < processos.size() - 1) {
            if (queue.empty() and tempo < processos[at].tempoDeChegada) {
                cout << "Aguardando próximo processo" << endl;
                sleep(processos[at].tempoDeChegada - tempo);
                tempo = processos[at].tempoDeChegada;
            }
            while (at < processos.size() and
                   processos[at].tempoDeChegada <= tempo) {
                queue.push(processos[at]);
                at++;
            }
            Processo aux = queue.top();
            execute_processo(aux, aux.tempoDeExecucao);
            somaDeTurnaround += tempo - aux.tempoDeChegada;
            queue.pop();
        }
        return somaDeTurnaround / (int)processos.size();
    }
    double RoundRobin() {
        double somaDeTurnaround = 0;
        queue<Processo> fila;
        for (Processo &processo : processos) fila.push(processo);
        while (!fila.empty()) {
            Processo processo = fila.front();
            fila.pop();
            if (processo.tempoDeChegada > tempo) {
                cout << "Aguardando próximo processo" << endl;
                sleep(processo.tempoDeChegada - tempo);
                tempo = processo.tempoDeChegada;
            }
            execute_processo(processo, min(quantum, processo.tempoDeExecucao));
            if (tempo > processo.deadline) {
                processo.estourou = true;
            }
            if (processo.tempoDeExecucao > 0) {
                fila.push(processo);
                tempo += sobrecarga;
                sleep(sobrecarga);
            } else {
                somaDeTurnaround += tempo - processo.tempoDeChegada;
            }
        }

        return somaDeTurnaround / (int)processos.size();
    }
    struct cmpEDF : binary_function<Processo, Processo, bool> {
        bool operator()(const Processo &a, const Processo &b) const {
            return a.deadline > b.deadline;
        }
    };
    double EDF() {
        double somaDeTurnaround = 0;
        int at = 0;
        priority_queue<Processo, vector<Processo>, cmpEDF> queue;
        while (!queue.empty() or at < processos.size() - 1) {
            if (queue.empty() and tempo < processos[at].tempoDeChegada) {
                cout << "Aguardando próximo processo" << endl;
                sleep(processos[at].tempoDeChegada - tempo);
                tempo = processos[at].tempoDeChegada;
            }
            while (at < processos.size() and
                   processos[at].tempoDeChegada <= tempo) {
                queue.push(processos[at]);
                at++;
            }
            Processo aux = queue.top();
            execute_processo(aux, quantum);
            queue.pop();
            if (tempo > aux.deadline) {
                aux.estourou = true;
            }
            if (aux.tempoDeExecucao > 0) {
                queue.push(aux);
                tempo += sobrecarga;
                sleep(sobrecarga);
            } else {
                somaDeTurnaround += tempo - aux.tempoDeChegada;
            }
        }
        return somaDeTurnaround / (int)processos.size();
    }
    double rodar() {
        sort(processos.begin(), processos.end(), cmpFIFO);
        if (processos.size() == 0) return 0;
        if (escalonador == "FIFO")
            return FIFO();
        else if (escalonador == "SJF")
            return SJF();
        else if (escalonador == "RoundRobin")
            return RoundRobin();
        else if (escalonador == "EDF")
            return EDF();
    }
};