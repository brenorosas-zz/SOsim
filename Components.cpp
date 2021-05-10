#include "header.cpp"
struct Pagina {
    int idPagina;
    Pagina(int idPagina) { this->idPagina = idPagina; }
};
struct Processo {
    int tempoDeChegada;
    int tempoDeExecucao;
    int deadline;
    int prioridade;
    int quantidadeDePaginas;
    int idPagina;
    vector<Pagina> paginas;
    Processo(int tempoDeChegada, int tempoDeExecucao, int deadline,
             int prioridade, int quantidadeDePaginas, int idPagina) {
        this->tempoDeChegada = tempoDeChegada;
        this->tempoDeExecucao = tempoDeExecucao;
        this->deadline = deadline;
        this->prioridade = prioridade;
        this->quantidadeDePaginas = quantidadeDePaginas;
        this->idPagina = idPagina;
        for (int i = idPagina; i < idPagina + quantidadeDePaginas; i++) {
            paginas.push_back(Pagina(i));
        }
    }
};
