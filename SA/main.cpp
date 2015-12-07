#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <cmath>

using namespace std;

vector<int> tempoAtendimentoCaminhao; // entrada: vetor de tempos de cada caminhão (posição do vetor = ID do caminhão)
vector< vector<int> > solucao; //matriz de docas x caminhoes alocados em cada doca
int qntdCaminhao, qntdDocas;

vector< vector<int> > copiaMatrizSolucao (const vector< vector<int> > &matrizSolucao) {
    vector< vector<int> > matrizRetorno;
    for (unsigned int i=0; i < matrizSolucao.size(); i++){
        matrizRetorno.push_back(vector<int>());
    }

    for (unsigned int i=0; i < matrizSolucao.size(); i++){
        matrizRetorno[i] = matrizSolucao[i];
    }

    return matrizRetorno;
}

void geraSolucaoInicial () {
    int doca;
    for (int i=0; i < qntdCaminhao; i++) {
        doca = rand() % qntdDocas;
        solucao[doca].push_back(i);
    }
}

vector< vector<int> > geraSolucaoVizinha (const vector< vector<int> > &matrizSolucao) {
    vector< vector<int> > matrizRetorno;
    matrizRetorno = copiaMatrizSolucao(matrizSolucao);

    // MOD nao pode ser feito por 0 (uma doca sem caminhao)
    int docaAtual;
    do
    {
        docaAtual = rand() % qntdDocas; // escolhe aleatoriamente uma doca
    }while(matrizSolucao[docaAtual].size() == 0);

    // nessa doca escolhida aleatoriamente, escolhe-se um caminhão aleatoriamente
    int indiceCaminhaoNaDoca = rand() % matrizSolucao[docaAtual].size();

    // escolhe-se aleatoriamente a nova doca em que o caminhão será alocado
    int novaDoca = rand() % qntdDocas;

    int idCaminhao = matrizSolucao[docaAtual][indiceCaminhaoNaDoca];
    // insere-se o caminhão na nova doca
    matrizRetorno[novaDoca].push_back(idCaminhao); // a ordem dos caminhões nas docas não importam no nosso problema
    // retira-se o caminhão da doca antiga
    matrizRetorno[docaAtual].erase(matrizRetorno[docaAtual].begin() + indiceCaminhaoNaDoca);
    //retorna nova solução = novo vizinho
    return matrizRetorno;
}

// retorna função objetivo para cada solução: o maior tempo de atendimento dentre as docas
int FO (const vector< vector<int> > &matrizSolucao) {
    int maiorTempo = 0;
    int tempo = 0;
    for (unsigned int i=0; i < matrizSolucao.size(); i++){
        for(unsigned int j = 0; j < matrizSolucao[i].size(); ++j){
            tempo += tempoAtendimentoCaminhao[matrizSolucao[i][j]];
        }

        if (tempo > maiorTempo) maiorTempo = tempo;
        tempo = 0;
    }
    return maiorTempo;
}

void SA (float T0, int SAmax, float alfa) {
    int iteracao = 0;
    float T = T0;
    int delta;
    geraSolucaoInicial();
    vector< vector<int> > solucaoCorrente = copiaMatrizSolucao(solucao);
    vector< vector<int> > solucaoVizinha;

    int tempoSolucao = FO(solucao);
    int tempoSolucaoCorrente = tempoSolucao;
    int tempoSolucaoVizinha;

    while (T > 0.001) {

        while (iteracao < SAmax) {
            iteracao++;
            solucaoVizinha = geraSolucaoVizinha(solucaoCorrente);
            tempoSolucaoVizinha = FO(solucaoVizinha);
            delta = tempoSolucaoVizinha - tempoSolucaoCorrente;
            if(delta < 0) {
                solucaoCorrente = solucaoVizinha;
                tempoSolucaoCorrente = tempoSolucaoVizinha;
                if (tempoSolucaoCorrente < tempoSolucao) {
                    solucao = solucaoCorrente;
                    tempoSolucao = tempoSolucaoCorrente;
                }
            }

            else {
                float r = ((float) rand() / (RAND_MAX)); //rand entre 0 e 1
                float prob = exp(-(float)delta / T);
                if (r < prob) { // aceita uma solução pior
                    solucaoCorrente = solucaoVizinha;
                    tempoSolucaoCorrente = tempoSolucaoVizinha;
                }
            }

        }
        T = alfa*T; //diminui temperarura de acordo com o alfa
        iteracao = 0;
    }
}

void inicializaMatrizSolucao(){
    for(int i = 0; i < qntdDocas; ++i){
        solucao.push_back(vector<int>());
    }
}

int main()
{
    srand(time(NULL));
    int entrada, SAmax;
    float T0, alfa;
    cin >> T0;
    cin >> SAmax;
    cin >> alfa;
    cin >> qntdDocas;
    cin >> qntdCaminhao;

    for(int i = 0; i < qntdCaminhao; ++i){
        cin >> entrada;
        tempoAtendimentoCaminhao.push_back(entrada);
    }

    inicializaMatrizSolucao();
    SA(T0, SAmax, alfa);

    int tempoTotal = 0;
    int maiorTempo = 0;
    for (unsigned int i=0; i < solucao.size(); i++) {
        cout << "Doca " << i+1 << endl;
        for (unsigned int j=0; j < solucao[i].size(); j++) {
            cout << "Caminhao " << solucao[i][j] << endl;
            tempoTotal += tempoAtendimentoCaminhao[solucao[i][j]];
        }
        if (tempoTotal > maiorTempo) {
            maiorTempo = tempoTotal;
        }
        cout << "Tempo total " << tempoTotal << endl << endl;
        tempoTotal= 0;
    }
    cout << "Valor da Solucao: " << maiorTempo << endl;
    return 0;
}
