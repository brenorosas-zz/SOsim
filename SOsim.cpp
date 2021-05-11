#include "Components.cpp"
#include "header.cpp"
int32_t main() {
    int n;
    cin >> n;
    MaquinaVirtual maquina;
    for (int i = 0; i < n; i++) {
        int tempoDeChegada, tempoDeExecucao, deadline prioridade,
            quantidadeDePaginas, idProcesso = i;
        cin >> tempoDeChegada >> tempoDeExecucao >> deadline >> prioridade >>
            quantidadeDePaginas;
        maquina.addProcesso(Processo(tempoDeChegada, TempoDeExecucao, deadline,
                                     prioridade, quantidadeDePaginas,
                                     idProcesso));
    }
}