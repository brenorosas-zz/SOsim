#include "Components.cpp"
#include "pages_algorithms.cpp"
void execute_processo(Processo &processo, int tempo){
    FIFOpage(processo.paginas);
    processo.paginasProntas = processo.paginas.size();
}
void FIFO(vector<Processo> &processos){
    if(processos.size() == 0) return;
    for(Processo &processo : processos){
        execute_processo(processo, processo.tempoDeExecucao);
    }
}
