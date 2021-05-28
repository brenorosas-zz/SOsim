#include "Components.cpp"
#include "header.cpp"
int32_t main() {
    MaquinaVirtual maquina = MaquinaVirtual();
    cout << "Algoritmo de paginação entre: FIFO MRU" << endl;
    cin >> maquina.paginacao;
    system("clear");
    cout << "Algoritmo de escalonação entre: FIFO SJF RoundRobin EDF" << endl;
    cin >> maquina.escalonador;
    system("clear");
    if (maquina.escalonador == "RoundRobin" or maquina.escalonador == "EDF") {
        cout << "Quantum:" << endl;
        cin >> maquina.quantum;
        system("clear");
    }
    cout << "Quantos processos:" << endl;
    int n;
    cin >> n;
    system("clear");
    for (int i = 0; i < n; i++) {
        int tempoDeChegada, tempoDeExecucao, deadline, prioridade,
            quantidadeDePaginas, idProcesso = i;
        cout << "Tempo de chegada, Tempo de execução, deadline, prioridade, "
                "quantidade de paginas"
             << endl;
        cin >> tempoDeChegada >> tempoDeExecucao >> deadline >> prioridade >>
            quantidadeDePaginas;
        system("clear");
        maquina.addProcesso(Processo(tempoDeChegada, tempoDeExecucao, deadline,
                                     prioridade, quantidadeDePaginas,
                                     idProcesso));
    }
    cout << maquina.rodar() << " é o turnaround médio" << endl;
}