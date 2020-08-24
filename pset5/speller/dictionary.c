// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node {
  char word[LENGTH + 1];
  struct node *next;
} node;

// Number of buckets in hash table
const unsigned int N = 85009;

// Hash table
node *table[N];

// Number of words loaded into the table
int word_count = 0;

// Returns true if word is in dictionary else false
bool check(const char *word) {
  int bucket = hash(word);

  node *current_node = table[bucket];
  while (current_node != NULL) {
    if (strcasecmp(current_node->word, word) == 0)
      return true;

    current_node = current_node->next;
  }

  return false;
}

// Hashes word to a number
unsigned int hash(const char *word) {
  // https://gist.github.com/MohamedTaha98/ccdf734f13299efb73ff0b12f7ce429f
  unsigned long hash = 5381;
  int c;
  while ((c = tolower(*word++)))
    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

  return hash % N;
}

// Loads dictionary into memory, returning true if successful else false
bool load(const char *dictionary) {
  FILE *dict = fopen(dictionary, "r");
  if (dict == NULL)
    return false;

  char word[LENGTH + 1];

  while (fscanf(dict, "%s", word) != EOF) {
    node *n = malloc(sizeof(node));
    if (n == NULL)
      return false;

    strcpy(n->word, word);
    int bucket = hash(word);
    n->next = table[bucket];
    table[bucket] = n;

    word_count++;
  }

  fclose(dict);
  return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void) { return word_count; }

// Unloads dictionary from memory, returning true if successful else false
bool unload(void) {
  for (int i = 0; i < N; i++) {
    node *list = table[i];

    while (list != NULL) {
      node *tmp = list->next;
      free(list);
      list = tmp;
    }

    if (i == N - 1 && list == NULL)
      return true;
  }

  return false;
}
