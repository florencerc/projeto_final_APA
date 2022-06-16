//BIBLIOTECAS
#include<stdio.h>
#include<stdlib.h>

//VARIAVEIS GLOBAIS
int servidores, jobs, custo_fixo, total_tempo_hc, total_custo_hc, total_tempo_vnd, total_custo_vnd = 0; 

//TIPOS ESTRUTURADOS
typedef struct servidor
{
    int indice;
    int capacidade;
}Servidor;

typedef struct jobs
{
    int indice;
    int tempo;
    int custo;
    float custo_tempo;

}Job;

typedef struct resultado
{
    int id_server;
    int custo;
    int tempo;

}Resultado;

//PROTOTIPOS DE FUNÇÕES
void insertion_sort_capacidade(Servidor *server, int tamanho);
Servidor inserir_servidor(int indice, int capacidade);
void inserir_job(Job *job, int indice, int tempo, int custo);
void insertion_sort_capacidade(Servidor *server, int tamanho);
void insertion_sort_custo_tempo(Job *job, int tamanho);
void ordenar(int servidores, int jobs, int tempo[servidores][jobs], int custo[servidores][jobs], Job *job, Servidor *server, Resultado *resultado);
void alocar(Job *job, Resultado *resultado, int tamanho, int capacidade, int indice);
void retorna_resposta(Resultado *resultado, int tamanho, int custo_fixo, int *total_tempo, int *total_custo);
void limpar_resultado(Resultado *resultado);

//FUNÇÕES
Servidor inserir_servidor(int i, int c){
  Servidor s;
  s.indice = i;
  s.capacidade = c;
  return s;
}

void inserir_job(Job *b, int i, int t, int c){
  b[i].indice= i;
  b[i].tempo=t;
  b[i].custo=c;
  b[i].custo_tempo = ((float)c/t); 
  
}

void insertion_sort_capacidade(Servidor *s, int n){
  Servidor aux;
  int i, j;
  for (i=0;i<n;i++){
    aux = s[i];
    for (j = i; (j>0) && (aux.capacidade<s[j-1].capacidade);j--)
      s[j]=s[j-1];
    s[j] = aux;
  }
  
}

void insertion_sort_custo_tempo(Job *b, int n){
  Job aux;
  int i, j;
  for (i=0;i<n;i++){
    aux = b[i];
    for (j = i; (j>0) && (aux.custo_tempo>b[j-1].custo_tempo);j--)
      b[j]=b[j-1];
    b[j] = aux;
  }
  
}

void ordenar(int s, int b, int t[s][b], int c[s][b], Job *job, Servidor *server, Resultado *r){
  for (int i = 0; i < s; i++){
    for(int j=0;j<b;j++)
      inserir_job(job, j, t[server[i].indice][j], c[server[i].indice][j]);
    insertion_sort_custo_tempo(job, b);
    alocar(job, r, b, server[i].capacidade, server[i].indice);
  }
}

void alocar(Job *b, Resultado *r, int n, int capacidade, int id){
  int soma = 0;
  for (int i=0;i<n;i++){
    if (r[b[i].indice].id_server == -1){
      soma = soma + b[i].tempo;
      if(soma <= capacidade){
        r[b[i].indice].id_server = id;
        r[b[i].indice].tempo = b[i].tempo;
        r[b[i].indice].custo = b[i].custo;
      } 
    }  
  }

}

void retorna_resposta(Resultado *r, int n, int p, int *ttt, int *ttc){
  int tempo, custo, i;
  tempo = 0;
  custo= 0;
  for (i=0;i<n;i++){
    if (r[i].id_server != -1){
      tempo = tempo + r[i].tempo;
      custo = custo + r[i].custo;
    } else {
      custo = custo + p; 
    }
  }
  if(*ttc ==0 || custo<=*ttc){
    *ttt=tempo;
    *ttc=custo;
  }  
 
}

void limpar_resultado(Resultado *r){
for (int i = 0; i < jobs; i++)
    r[i].id_server = -1;
}  

int main(){

  FILE *arq;

  if ((arq = fopen("n5m15B.txt","r")) == NULL) {
        printf("\n Arquivo não pode ser aberto \n");
        return 0;
  }

  fscanf(arq, " %d", &jobs);
  fscanf(arq, " %d", &servidores);
  fscanf(arq, " %d", &custo_fixo);

  Servidor capacidade[servidores];
  Job arr_jobs[jobs];
  int capacidades[servidores];
  Resultado resultado[jobs];
  int tempo[servidores][jobs];
  int custo[servidores][jobs];

  //capacidades
  for (int i = 0; i < servidores; i++){
    fscanf(arq, "%d", &capacidades[i]);
    capacidade[i] = inserir_servidor(i, capacidades[i]);
  }
  
  //matriz tempo
  for(int i=0;i<servidores;i++){
    for(int j=0;j<jobs;j++){
      fscanf(arq,"%d ", &tempo[i][j]);
    }
  }

  //custo
  for(int i=0;i<servidores;i++){
    for(int j=0;j<jobs;j++){
        fscanf(arq,"%d ", &custo[i][j]);
    }  
  }
   
  fclose(arq);   
  
  insertion_sort_capacidade(capacidade,servidores);
  
  limpar_resultado(resultado);
  
  ordenar(servidores, jobs, tempo,custo,arr_jobs, capacidade, resultado);
  
  retorna_resposta(resultado, jobs, custo_fixo, &total_tempo_hc, &total_custo_hc);

  limpar_resultado(resultado);
  
  for(int i=0;i<servidores-1;i++){
    for(int j=i+1;j<servidores;j++){
      int aux_id = capacidade[i].indice;
      int aux_c = capacidade[i].capacidade;
      capacidade[i].indice = capacidade[j].indice;
      capacidade[i].capacidade = capacidade[j].capacidade;
      capacidade[j].indice=aux_id;
      capacidade[j].capacidade=aux_c;
            
      ordenar(servidores, jobs, tempo,custo,arr_jobs, capacidade, resultado);

      retorna_resposta(resultado, jobs,custo_fixo,&total_tempo_vnd, &total_custo_vnd);

      limpar_resultado(resultado);
    } 
  }
      
  printf("Valor Solução_HC: %d Tempo_HC:%d \n", total_custo_hc, total_tempo_hc);
  printf("Valor Solução_VND: %d Tempo_VND:%d \n", total_custo_vnd, total_tempo_vnd);
 
  return 0;
 
}
