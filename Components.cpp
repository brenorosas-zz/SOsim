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
        for (int i = 0; i < 100; i++) disco[i] = Pagina(i, -1);
    };
    bool existe(Pagina pagina) {
        for (auto &paginaD : disco)
            if (pagina == paginaD) return true;
        return false;
    }
    void add(Pagina pagina) {
        for (int i = 0; i < 100; i++) {
            if (disco[i] == Pagina(i, -1)) {
                disco[i] = pagina;
                break;
            }
        }
    }
    void remove(Pagina pagina) {
        for (int i = 0; i < disco.size(); i++) {
            if (pagina == disco[i]) {
                disco[i] = Pagina(i, -1);
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

void renderConsole(DISCO disco, RAM ram, vector<vector<char> > &gant) {
    system("clear");
    cout << "GANT" << endl;
    for (int i = 0; i < gant.size(); i++) {
        cout << i << ":";
        for (int j = 0; j < gant[i].size(); j++) {
            cout << gant[i][j];
        }
        cout << endl;
    }
    cout << "E - executando processo, D - executando processo após fim do "
            "deadline, S - sobrecarga, B - buscando paginas no disco, | - "
            "deadline"
         << endl;
    cout << endl << endl;
    cout << "Pagina é identificada pelo processo que a utiliza e a posição "
            "dela nesse processo"
         << endl;
    cout << "RAM" << endl;
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 10; j++) {
            int pos = 5 * j + i;
            cout << pos << " |";
            if (ram.ram[pos].idProcessoAssociado == -1) {
                cout << "-:-|"
                     << " \t";
            } else {
                cout << ram.ram[pos].idProcessoAssociado << ':'
                     << ram.ram[pos].idPagina << "| \t";
            }
        }
        cout << endl;
    }
    cout << endl;

    cout << "DISCO" << endl;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            int pos = 10 * j + i;
            cout << pos << " |";
            if (disco.disco[pos].idProcessoAssociado == -1) {
                cout << "-:-|"
                     << " \t";
            } else {
                cout << disco.disco[pos].idProcessoAssociado << ':'
                     << disco.disco[pos].idPagina << "| \t";
            }
        }
        cout << endl;
    }
    cout << endl;
}

vector<char> aux(150, '.');
struct MaquinaVirtual {
    DISCO disco;
    RAM ram;
    vector<vector<char> > gant;
    int quantum;
    int sobrecarga;
    vector<Processo> processos;
    string escalonador, paginacao;
    int tempo;
    MaquinaVirtual() {
        gant.resize(10, aux);
        ram = RAM();
        disco = DISCO();
        tempo = 0;
    }
    void addProcesso(Processo processo) { processos.push_back(processo); }
    void page(vector<Pagina> &paginas, int processoID) {
        bool buscou = false;
        for (Pagina &pagina : paginas) {
            if (!ram.existe(pagina)) {
                Pagina aux;
                if (paginacao == "FIFO") aux = ram.addFIFO(pagina);
                if (paginacao == "MRU") aux = ram.addMRU(pagina);
                if (aux.idProcessoAssociado != -1) {
                    disco.add(aux);
                }
                if (disco.existe(pagina)) {
                    buscou = true;
                    disco.remove(pagina);
                }
            } else if (paginacao == "MRU") {
                ram.updateMRU(pagina);
            }
        }
        if (buscou) {
            gant[processoID][tempo] = 'B';
            renderConsole(disco, ram, gant);
            sleep(1);
            tempo++;
        }
    }
    void execute_processo(Processo &processo, int tempoDeExecucao) {
        page(processo.paginas, processo.idProcesso);
        processo.paginasProntas = processo.paginas.size();
        for (int i = 0; i < tempoDeExecucao; i++) {
            tempo++;
            if (processo.deadline < tempo) processo.estourou = true;
            if (processo.estourou)
                gant[processo.idProcesso][tempo - 1] = 'D';
            else
                gant[processo.idProcesso][tempo - 1] = 'E';
            renderConsole(disco, ram, gant);
            sleep(1);
        }
        processo.tempoDeExecucao -= tempoDeExecucao;
        if (processo.tempoDeExecucao > 0) {
            gant[processo.idProcesso][tempo] = 'S';
            renderConsole(disco, ram, gant);
            tempo++;
            sleep(1);
        }
    }
    static bool cmpFIFO(Processo a, Processo b) {
        return a.tempoDeChegada < b.tempoDeChegada;
    }
    double FIFO() {
        double somaDeTurnaround = 0;
        for (Processo &processo : processos) {
            if (processo.tempoDeChegada > tempo) {
                sleep(processo.tempoDeChegada - tempo);
                tempo = processo.tempoDeChegada;
            }
            renderConsole(disco, ram, gant);
            execute_processo(processo, processo.tempoDeExecucao);
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
        while (!queue.empty() or at < processos.size()) {
            if (queue.empty() and tempo < processos[at].tempoDeChegada) {
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
        int at = 0;
        while (!fila.empty() or at < processos.size()) {
            if (fila.empty()) {
                sleep(processos[at].tempoDeChegada - tempo);
                tempo = processos[at].tempoDeChegada;
            }
            while (at < processos.size() and
                   processos[at].tempoDeChegada <= tempo) {
                fila.push(processos[at]);
                at++;
            }
            Processo processo = fila.front();
            fila.pop();
            renderConsole(disco, ram, gant);
            execute_processo(processo, min(quantum, processo.tempoDeExecucao));
            if (processo.tempoDeExecucao > 0) {
                fila.push(processo);
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
        while (!queue.empty() or at < processos.size()) {
            if (queue.empty() and tempo < processos[at].tempoDeChegada) {
                sleep(processos[at].tempoDeChegada - tempo);
                tempo = processos[at].tempoDeChegada;
            }
            while (at < processos.size() and
                   processos[at].tempoDeChegada <= tempo) {
                queue.push(processos[at]);
                at++;
            }
            Processo aux = queue.top();
            queue.pop();
            execute_processo(aux, min(quantum, aux.tempoDeExecucao));
            if (aux.tempoDeExecucao > 0) {
                queue.push(aux);
            } else {
                somaDeTurnaround += tempo - aux.tempoDeChegada;
            }
        }
        return somaDeTurnaround / (int)processos.size();
    }
    double rodar() {
        sort(processos.begin(), processos.end(), cmpFIFO);
        for (auto &processo : processos) {
            gant[processo.idProcesso][processo.deadline] = '|';
        }
        if (processos.size() == 0) return 0;

        renderConsole(disco, ram, gant);
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