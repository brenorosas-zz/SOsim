#include "header.cpp"
struct Pagina {
    int idPagina;
    int idProcessoAssociado;
    Pagina(int idPagina, int idProcesso) { this->idPagina = idPagina; this->idProcessoAssociado = idProcesso;}
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
