#include "Components.cpp"
#include "header.cpp"
int32_t main()
{
    int n;
    cin >> n;
    MaquinaVirtual maquina = MaquinaVirtual();
    for (int i = 0; i < n; i++)
    {
        int tempoDeChegada, tempoDeExecucao, deadline, prioridade,
            quantidadeDePaginas, idProcesso = i;
        cin >> tempoDeChegada >> tempoDeExecucao >> deadline >> prioridade >>
            quantidadeDePaginas;
        maquina.addProcesso(Processo(tempoDeChegada, tempoDeExecucao, deadline,
                                     prioridade, quantidadeDePaginas,
                                     idProcesso));
    }
    cin >> maquina.paginacao >> maquina.escalonador;
    if (maquina.escalonador == "RoundRobin")
    {
        cin >> maquina.quantum;
    }
    maquina.rodar();
}