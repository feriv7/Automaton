#include <stdio.h>
#include <stdlib.h>

#define MAX_STATES 100
#define MAX_TRANSITIONS 100

typedef struct {
    char input; // Símbolo de entrada
    int targetStateIndex; // Índice del estado destino
} Transition;

typedef struct {
    char name; // Nombre del estado
    Transition transitions[MAX_TRANSITIONS]; // Transiciones desde este estado
    int num_transitions; // Número de transiciones válidas
} State;

typedef struct {
    State states[MAX_STATES]; // Estados del autómata
    int num_states; // Número de estados válidos
} Automaton;

void add_state(Automaton *automaton, char state_name) {
    if (automaton->num_states < MAX_STATES) {
        automaton->states[automaton->num_states].name = state_name;
        automaton->states[automaton->num_states].num_transitions = 0;
        automaton->num_states++;
    }
}

void add_transition(Automaton *automaton, char from_state, char input, char to_state) {
    for (int i = 0; i < automaton->num_states; i++) {
        if (automaton->states[i].name == from_state) {
            int transitionIndex = automaton->states[i].num_transitions;
            if (transitionIndex < MAX_TRANSITIONS) {
                automaton->states[i].transitions[transitionIndex].input = input;
                for (int j = 0; j < automaton->num_states; j++) {
                    if (automaton->states[j].name == to_state) {
                        automaton->states[i].transitions[transitionIndex].targetStateIndex = j;
                        break;
                    }
                }
                automaton->states[i].num_transitions++;
            }
            break;
        }
    }
}

int validate_chain(Automaton *automaton, char *chain) {
    if (automaton->num_states == 0 || chain[0] == '\0') return 0; // No states or empty chain

    int current_state_index = 0; // Assuming the initial state is always at index 0
    for (int i = 0; chain[i] != '\0'; i++) {
        int found = 0;
        State current_state = automaton->states[current_state_index];
        for (int k = 0; k < current_state.num_transitions; k++) {
            if (current_state.transitions[k].input == chain[i]) {
                current_state_index = current_state.transitions[k].targetStateIndex;
                found = 1;
                break;
            }
        }
        if (!found) return 0; // If no transition found, the chain is invalid
    }
    return 1; // If all characters in the chain have valid transitions, the chain is valid
}

void print_automaton(Automaton *automaton) {
    printf("State Transition Table:\n");
    for (int i = 0; i < automaton->num_states; i++) {
        printf("State %c: ", automaton->states[i].name);
        for (int j = 0; j < automaton->states[i].num_transitions; j++) {
            printf("(%c -> %c) ", automaton->states[i].transitions[j].input, automaton->states[automaton->states[i].transitions[j].targetStateIndex].name);
        }
        printf("\n");
    }
}

int main() {
    Automaton automaton = { .num_states = 0 };
    int num_states, num_transitions;
    char state_name, from_state, input, to_state;
    char chain[100]; // Assuming the maximum length of the chain is 99 characters + null terminator
    char continue_program;

    printf("Enter the number of states: ");
    scanf("%d", &num_states);
    for (int i = 0; i < num_states; i++) {
        printf("Enter state %d name: ", i + 1);
        scanf(" %c", &state_name);
        add_state(&automaton, state_name);
    }

    printf("Enter the number of transitions: ");
    scanf("%d", &num_transitions);
    for (int i = 0; i < num_transitions; i++) {
        printf("Enter transition %d (from_state input to_state): ", i + 1);
        scanf(" %c %c %c", &from_state, &input, &to_state);
        add_transition(&automaton, from_state, input, to_state);
    }

    print_automaton(&automaton);

    do {
        printf("Enter the chain to validate: ");
        scanf("%s", chain);

        if (validate_chain(&automaton, chain)) {
            printf("The chain is valid.\n");
        } else {
            printf("The chain is invalid.\n");
        }

        printf("Do you want to validate another chain? (Y/N): ");
        scanf(" %c", &continue_program);
    } while (continue_program == 'Y' || continue_program == 'y');

    return 0;
}