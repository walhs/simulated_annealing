#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <numeric>
#include <cmath>

using namespace std;

vector<int> tempoAtendimentoCaminhao;
vector< vector<int> > solucao; //matriz de docas x ordem de atendimento dos caminhoes
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
        //srand(time(NULL));
        doca = rand() % qntdDocas;
        solucao[doca].push_back(i);
    }
}

vector< vector<int> > geraSolucaoVizinha (const vector< vector<int> > &matrizSolucao) {
    vector< vector<int> > matrizRetorno;
    matrizRetorno = copiaMatrizSolucao(matrizSolucao);

    // MOD nao pode ser feito por 0 (uma doca sem cmainhao)
    int docaAtual;
    do
    {
        //srand(time(NULL));
        docaAtual = rand() % qntdDocas;
    }while(matrizSolucao[docaAtual].size() == 0);

    //srand(time(NULL));
    int indiceCaminhaoNaDoca = rand() % matrizSolucao[docaAtual].size();

    //srand(time(NULL));
    int novaDoca = rand() % qntdDocas;

    int idCaminhao = matrizSolucao[docaAtual][indiceCaminhaoNaDoca];
    matrizRetorno[novaDoca].push_back(idCaminhao);

    matrizRetorno[docaAtual].erase(matrizRetorno[docaAtual].begin() + indiceCaminhaoNaDoca);

    return matrizRetorno;
}

int FO (const vector< vector<int> > &matrizSolucao) { //maior tempo de atendimento das docas
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
                //srand(time(NULL));
                float r = ((float) rand() / (RAND_MAX));
                float prob = exp(-(float)delta / T);
                if (r < prob) {
                    solucaoCorrente = solucaoVizinha;
                    tempoSolucaoCorrente = tempoSolucaoVizinha;
                }
            }

        }
        T = alfa*T;
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

    /*
    while (qntdCaminhao > 0) {
        cin >> entrada;
        //cout << entrada << endl;
        tempoAtendimentoCaminhao.push_back(entrada);
        --qntdCaminhao;
    }*/

    inicializaMatrizSolucao();
    SA(T0, SAmax, alfa);

    int tempoTotal = 0;
    for (unsigned int i=0; i < solucao.size(); i++) {
        cout << "Doca " << i+1 << endl;
        for (unsigned int j=0; j < solucao[i].size(); j++) {
            cout << "Caminhao " << solucao[i][j] << endl;
            tempoTotal += tempoAtendimentoCaminhao[solucao[i][j]];
        }
        cout << "Tempo total " << tempoTotal << endl << endl;
        tempoTotal= 0;
    }
    return 0;
}
