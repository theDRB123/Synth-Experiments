#ifndef AMPLITUDE_GEN_H
#define AMPLITUDE_GEN_H

#define SAMPLE_RATE 44100

typedef struct
{
    double startval;
    double endval;
    double duration;
} ENVELOPE_PARAMS;

typedef struct
{
    double duration;
    double *points;
    double *values;
} ENVELOPE;

// Function declarations
void write_to_file(ENVELOPE strip, char *filename);
void print_params(char *type, ENVELOPE_PARAMS params);
void print_strip(ENVELOPE strip);
ENVELOPE natural_exponential_decay(ENVELOPE_PARAMS params);
ENVELOPE linear(ENVELOPE_PARAMS params);
ENVELOPE step(ENVELOPE_PARAMS params, int step_count);
ENVELOPE add_strips(ENVELOPE strip1, ENVELOPE strip2);

#endif