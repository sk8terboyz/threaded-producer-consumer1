#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

#define BUFFER_SIZE 10

/* data that is shared by thread(s) */
int buffer[BUFFER_SIZE];
int counter = 0;    //number of occupied cells
/* input array to have all values initialized */
int input[1000000];

void *insert_item()
{
  int next_produced;
  int in = 0;         //next location to insert into
  int runner = 0;
  bool cont = true;
  while(cont == true)
  {
    /* produce an item in next produced */
    next_produced = input[runner];
    runner++;
    while(counter == BUFFER_SIZE);
    buffer[in] = next_produced;
    in = (in + 1) % BUFFER_SIZE;
    counter++;
    if(next_produced == 1000000)
      cont = false;
  }
  pthread_exit(0);
}

void *remove_item()
{
  int next_consumed;
  int out = 0;        //next location to remove value
  int checker = 0;    //used to check all values are inputted correctly
  bool go = true;
  while(go == true)
  {
    while(counter == 0);
    next_consumed = buffer[out];
    out = (out + 1) % BUFFER_SIZE;
    counter--;
    checker++;
    /* consume the item in next consumed */
    if(next_consumed != checker)
    {
      printf("Error: The consumer received the sequence up to %d correctly. The next number (incorrect) number was %d\n", checker, next_consumed);
      exit(-1);
    }
    if(checker == 1000000)
      checker = false;
  }
}

int main()
{
  for(int i = 1; i != 1000001; i++)
  {
    input[i-1] = i;
  }
  pthread_t thread1_id;
  pthread_t thread2_id;
  pthread_create(&thread1_id, NULL, &insert_item, NULL);
  pthread_create(&thread2_id, NULL, &remove_item, NULL);

  pthread_join(thread1_id, NULL);
  pthread_join(thread2_id, NULL);

  printf("completed\n");
  return 0;
}
