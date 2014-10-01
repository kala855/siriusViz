#include <stdlib.h>
#include <math.h>

/*Esta función retorna el valor absoluto mayor entre un par de numeros
 * tipo float*/
float max2Numbers(float a, float b){

  if (fabs(a) > fabs(b))
    return fabs(a);
  else
    return fabs(b);

}

float max(float *vec, int size){
  float maximo;
  maximo = vec[0];
  for(int i = 0; i < size; ++i){
    if(vec[i] > maximo)
      maximo = vec[i];
  }

  return maximo;

}


float min(float *vec, int size){
  float minimo;
  minimo = 100000.0;
  for(int i = 0; i < size; ++i){
    if(vec[i] < minimo){
      minimo = vec[i];
    }

  }
  return minimo;
}
