/* Decaygen.c */
// Multiplte Decay Variations
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Global Parameters
#define SAMPLE_RATE 100 //44100

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

void write_to_file(ENVELOPE strip, char *filename)
{
    int npoints = strip.duration * SAMPLE_RATE;
    FILE *fp = fopen(filename, "w");
    printf("=====Writing to file %s, lines: %i========\n", filename, npoints);
    for (int i = 0; i < npoints; i++)
    {
        fprintf(fp, "%.4f\t%0.8f\n", strip.points[i], strip.values[i]);
    }
    printf("=====Writing Completed ========");
}

void print_params(char *type, ENVELOPE_PARAMS params)
{
    printf("=== %s ===\n", type);
    printf("=== ENVELOPE_PARAMS ===\n");
    printf("SAMPLE_RATE: %d\n", SAMPLE_RATE);
    printf("startval: %f\n", params.startval);
    printf("endval: %f\n", params.endval);
    printf("duration: %f\n", params.duration);
    printf("==============\n");
}

void print_strip(ENVELOPE strip)
{
    int npoints = strip.duration * SAMPLE_RATE;
    printf("=== BREAKPOINT_STRIP ===\n");
    printf("npoints: %d\n", npoints);
    for (int i = 0; i < npoints; i++)
    {
        printf("Point %d: time=%.4f, value=%.8f\n", i, strip.points[i], strip.values[i]);
    }
    printf("========================\n");
}

ENVELOPE natural_exponential_decay(ENVELOPE_PARAMS params)
{
    // x = ae^(kT)
    print_params("natural_exp", params);
    double n = params.duration * SAMPLE_RATE;
    double T = params.duration;
    double step = T / n;
    double epsilon = 1.0e-4;
    double k = -log(epsilon) / T;
    double ratio = exp(-k * step);

    double range = params.startval - params.endval;

    double *points = (double *)malloc((n + 1) * sizeof(double));
    double *values = (double *)malloc((n + 1) * sizeof(double));
    double t = 0.0;
    double decay = 1.0;
    for (int i = 0; i <= n; i++)
    {
        points[i] = t;
        // scaled values
        values[i] = params.endval + decay * range;
        t += step;
        decay *= ratio;
    }

    ENVELOPE result = {params.duration, points, values};
    return result;
}

ENVELOPE linear(ENVELOPE_PARAMS params)
{
    print_params("linear", params);
    double T = params.duration;
    int n = params.duration * SAMPLE_RATE;
    double step = T / n;
    double start = params.startval;
    double diff = (params.endval - params.startval) / n;
    double *points = (double *)malloc((n + 1) * sizeof(double));
    double *values = (double *)malloc((n + 1) * sizeof(double));
    double t = 0.0;

    for (int i = 0; i <= n; i++)
    {
        points[i] = t;
        values[i] = start;
        t += step;
        start += diff;
    }

    ENVELOPE result = {params.duration, points, values};
    return result;
}

ENVELOPE step(ENVELOPE_PARAMS params, int step_count)
{
    print_params("STEP", params);
    double T = params.duration;
    double n = params.duration * SAMPLE_RATE;

    double step_size = (params.endval - params.startval) / step_count;
    double step_length = n / step_count;

    double *points = (double *)malloc((n + 1) * sizeof(double));
    double *values = (double *)malloc((n + 1) * sizeof(double));

    double t = 0;

    for (int i = 0; i <= n; i++)
    {
        points[i] = t;
        values[i] = params.startval + ceil(i / step_length) * step_size;
        t += T / n;
    }
    ENVELOPE result = {params.duration, points, values};
    return result;
}

ENVELOPE add_strips(ENVELOPE strip1, ENVELOPE strip2)
{
    int totalpoints = (strip1.duration + strip2.duration) * SAMPLE_RATE;
    int n1 = strip1.duration * SAMPLE_RATE;
    int n2 = strip2.duration * SAMPLE_RATE;
    double *new_points = (double *)malloc(totalpoints * sizeof(double));
    double *new_values = (double *)malloc(totalpoints * sizeof(double));

    for (int i = 0; i < n1; i++)
    {
        new_points[i] = strip1.points[i];
        new_values[i] = strip1.values[i];
    }
    double point_offset = strip1.points[n1 - 1];
    for (int i = 0; i < n2; i++)
    {
        new_points[i + n1] = strip2.points[i] + point_offset;
        new_values[i + n1] = strip2.values[i];
    }
    ENVELOPE result = {strip1.duration + strip2.duration, new_points, new_values};
    return result;
}

int main(int arc, char **argv)
{
    ENVELOPE_PARAMS params1 = {0, 1, 10};
    ENVELOPE linear1 = linear(params1);
    ENVELOPE_PARAMS params2 = {1, 0, 30};
    ENVELOPE linear2 = linear(params2);
    ENVELOPE_PARAMS params3 = {0, 0.5, 50};
    ENVELOPE linear3 = linear(params3);
    ENVELOPE_PARAMS nd1 = {0.5, 0, 30};
    ENVELOPE n_decay = natural_exponential_decay(nd1);
    write_to_file(n_decay, "n_decay_strip.bpf");
    ENVELOPE_PARAMS stp1 = {0, 1, 10};
    ENVELOPE stp = step(stp1, 5);
    printf("Created strips\n");
    ENVELOPE combined_strip;
    combined_strip = add_strips(linear1, linear2);
    combined_strip = add_strips(combined_strip, linear3);
    combined_strip = add_strips(combined_strip, n_decay);
    combined_strip = add_strips(combined_strip, stp);
    write_to_file(combined_strip, "strip.bpf");
}