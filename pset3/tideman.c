#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];
bool lock = true;

// Each pair has a winner, loser
typedef struct {
  int winner;
  int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[]) {
  // Check for invalid usage
  if (argc < 2) {
    printf("Usage: tideman [candidate ...]\n");
    return 1;
  }

  // Populate array of candidates
  candidate_count = argc - 1;
  if (candidate_count > MAX) {
    printf("Maximum number of candidates is %i\n", MAX);
    return 2;
  }
  for (int i = 0; i < candidate_count; i++) {
    candidates[i] = argv[i + 1];
  }

  // Clear graph of locked in pairs
  for (int i = 0; i < candidate_count; i++) {
    for (int j = 0; j < candidate_count; j++) {
      locked[i][j] = false;
    }
  }

  pair_count = 0;
  int voter_count = get_int("Number of voters: ");

  // Query for votes
  for (int i = 0; i < voter_count; i++) {
    // ranks[i] is voter's ith preference
    int ranks[candidate_count];

    // Query for each rank
    for (int j = 0; j < candidate_count; j++) {
      string name = get_string("Rank %i: ", j + 1);

      if (!vote(j, name, ranks)) {
        printf("Invalid vote.\n");
        return 3;
      }
    }

    record_preferences(ranks);

    printf("\n");
  }

  add_pairs();
  sort_pairs();
  lock_pairs();
  print_winner();
  return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[]) {
  for (int i = 0; i < candidate_count; i++) {
    if (strcmp(name, candidates[i]) == 0) {
      ranks[rank] = i;
      return true;
    }
  }
  return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[]) {
  for (int i = 0; i < candidate_count; i++)
    for (int j = i + 1; j < candidate_count; j++)
      preferences[ranks[i]][ranks[j]]++;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void) {
  for (int i = 0; i < candidate_count; i++) {
    for (int j = i + 1; j < candidate_count; j++) {
      if (preferences[i][j] > preferences[j][i]) {
        pairs[pair_count].winner = i;
        pairs[pair_count].loser = j;
        pair_count++;
      } else if (preferences[i][j] < preferences[j][i]) {
        pairs[pair_count].winner = j;
        pairs[pair_count].loser = i;
        pair_count++;
      }
    }
  }
}

// Sort helper
void swap(pair arr[], int idx1, int idx2) {
  pair temp = arr[idx1];
  arr[idx1] = arr[idx2];
  arr[idx2] = temp;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void) {
  bool is_sorted = false;

  while (!is_sorted) {
    is_sorted = true;

    for (int i = 0; i < pair_count - 1; i++) {
      int diff1 = pairs[i].winner - pairs[i].loser;
      int diff2 = pairs[i + 1].winner - pairs[i + 1].loser;
      if (diff1 > diff2)
        swap(pairs, i, i + 1);
    }
  }
}

// All the credit to
// https://github.com/Federico-abss/CS50-intro-course/blob/master/C/pset3/tideman/tideman.c
void validateLock(int j) {
  if (j == 0) {
    return;
  }

  int r = 0;
  bool rank[j];
  for (int i = 0; i < j; i++) {
    rank[i] = false;
  }

  // checks all the submatrixes up to a single square using recursion
  validateLock(j - 1);

  for (int i = 0; i < j; i++) {
    for (int k = 0; k < j; k++) {
      if (locked[i][k] == true) {
        rank[i] = true;
      }
    }
  }

  for (int i = 0; i < j; i++) {
    if (rank[i] == true) {
      r++;
    }
  }

  // if the rank is max the lock is canceled
  if (r == j) {
    lock = false;
  }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void) {
  for (int i = 0; i < pair_count; i++) {
    locked[pairs[i].winner][pairs[i].loser] = true;

    validateLock(candidate_count);
    // if the validateLock function found a cycle we reverse the lock
    if (!lock) {
      locked[pairs[i].winner][pairs[i].loser] = false;
    }
    lock = true;
  }
}

// Print the winner of the election
void print_winner(void) {
  for (int i = 0; i < candidate_count; i++) {
    int counter = 0;
    for (int j = 0; j < candidate_count; j++) {
      if (!locked[j][i])
        counter++;

      if (counter >= pair_count)
        printf("%s\n", candidates[i]);
    }
  }
}
