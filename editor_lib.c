#include "editor_lib.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Functions about ll_text...
// append a line: add to the end of the linked list
ll_text *append_text(ll_text *list, char *text) {
  ll_text *new_node = malloc(sizeof(ll_text));
  if (!new_node) return list;
  
  new_node->text = strdup(text);
  new_node->next = NULL;

  if (!list) {
     return new_node;
  }
  ll_text *temp = list;
  while (temp->next) {
       temp = temp->next;
  }
  temp->next = new_node;

  return list;
}

void save_to_file(ll_text *list, const char *filename) {
  FILE *outfile = fopen(filename, "w");
  for (ll_text *here = list; here; here = here->next) {
    fprintf(outfile, "%s\n", here->text);
  }
  fclose(outfile);
}

int ll_text_length(ll_text *list) {
  int length = 0;
  for (ll_text *current = list; current; current = current->next) {
	  length++;
  }
  return length;
}

// insert a specific line: add to specific place in the linked list; this is the
// new entry at the specified line
ll_text *insert_text(ll_text *list, char *text, int position) {
   ll_text *new_node = malloc(sizeof(ll_text));
   if (!new_node) return list;

   new_node->text = strdup(text);
   new_node->next = NULL;
   if (position == 0) {
      new_node->next = list;
      return new_node;
   }

   ll_text *temp = list;
   for (int i = 0; i < position - 1 && temp->next != NULL; i++) {
       temp = temp->next;

   }
   new_node->next = temp->next;
   temp->next = new_node;

   return list;
}

// delete a specific line: delete the ith entry in the linked list and return
// the new front of the linked list.
ll_text *delete_text(ll_text *list, int position) {
   if (!list) return NULL;

   if (position == 0) {
      ll_text *new_head = list->next;
      free(list->text);
      free(list);
      return new_head;
  }

   ll_text *temp = list;
   for (int i=0; i < position - 1 && temp->next != NULL; i++) {
	temp = temp->next;
   }

   ll_text *to_delete = temp->next;
   if (to_delete) {
      temp->next = to_delete->next;
      free(to_delete->text);
      free(to_delete);
   }

   return list;
}


// replace a line: go to the specific entry, free the text that's there, replace
// it with a copy of the specified text.
ll_text *replace_text(ll_text *list, char *text, int position) {
    if (!list) return NULL;

    ll_text *temp = list;
    for (int i=0;i < position; i++) {
	 if (temp->next == NULL) return list;
         temp = temp->next;
    }

    free(temp->text);
    temp->text = strdup(text);

    return list;
}

// duplicate a list. Returns a copy of the list, including newly copied versions
// of all of the strings.
ll_text *duplicate_ll_text(ll_text *list) {
   if (!list) return NULL;

   ll_text *new_head = malloc(sizeof(ll_text));
   if (!new_head) return NULL;
   new_head->text = strdup(list->text);
   if (!new_head->text) { free(new_head); return NULL; } 

   ll_text *current_new = new_head;
 
   for (ll_text *current_original = list->next; current_original; current_original = current_original->next) {
	 ll_text *new_node = malloc(sizeof(ll_text));
      	 if (!new_node) {
	  	 for (ll_text *node = new_head; node; ) {
	       	     ll_text *temp = node->next;
	    	     free(node->text);
		     free(node);
		     node = temp;
	  }
	    	 return NULL;
	 } 
      	 new_node->text = strdup(current_original->text);
      	 if (!new_node->text) { free(new_node); return NULL; }

      	 current_new->next = new_node;
      	 current_new = new_node;
    }
    current_new->next = NULL;

    return new_head;
}

// functions about the state stack...
ll_text_stack *load_from_file(const char *filename) {
  ll_text *lines = NULL;
  ll_text_stack *stack = calloc(1, sizeof(ll_text_stack));
  char buf[1024];
  FILE *infile = fopen(filename, "r");
  while (fgets(buf, 1024, infile)) {
    chomp(buf);
    lines = append_text(lines, buf);
  }
  stack->lines = lines;
  return stack;
}

ll_text_stack *new_stack_empty_text(void) {
  ll_text_stack *stack = calloc(1, sizeof(ll_text_stack));
  stack->lines = NULL;
  return stack;
}

// Push a new state onto the stack that's an exact copy of the state currently
// at the top.
ll_text_stack *push_duplicate(ll_text_stack *stack) {
  ll_text_stack *new_stack = malloc(sizeof(ll_text_stack));
  if (!new_stack) return NULL;

  new_stack->lines = duplicate_ll_text(stack->lines);
  if (!new_stack->lines) {
	  free(new_stack);
	  return NULL;
  }
  new_stack->next = stack;
  return new_stack;
}

// Push a new entry onto the stack that has an empty ll_text at the top.
ll_text_stack *push_empty(ll_text_stack *stack) {
  
    ll_text_stack *new_stack = malloc(sizeof(ll_text_stack));
    if (!new_stack) return NULL;

    new_stack->lines = NULL;
    new_stack->next = stack;
    return new_stack;
 }

// undo, removing the most recent state
// this has to free all the memory associated with that state
ll_text_stack *pop_stack(ll_text_stack *stack) {
   if (!stack) return NULL;

  ll_text_stack *new_top = stack->next;
  ll_text *current = stack->lines;

  while (current) {
      ll_text *temp = current;
      current = current->next;
      free(temp->text);
      free(temp);
  }

  free(stack);
  return new_top;
}

// Find the first newline in the input and replace it with \0
void chomp(char *s) {
  for (int i = 0; s[i]; i++) {
    if (s[i] == '\n' || s[i] == '\r') {
      // Check for \r in case the line came from a Windows-edited file.
      s[i] = '\0';
      break;
    }
  }
}
