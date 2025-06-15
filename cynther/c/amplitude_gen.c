/* Decaygen.c */
// Multiplte Decay Variations
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct
{
    int npoints;
    double startval;
    double endval;
    double duration;
} PARAMS;

typedef struct
{
    int npoints;
    double *points;
    double *values;
} STRIP;

void write_to_file(STRIP strip, char *filename)
{
    FILE *fp = fopen(filename, "w");
    printf("Writing to file %s, lines: %i", filename, strip.npoints);
    for (int i = 0; i < strip.npoints; i++)
    {
        fprintf(fp, "%.4f\t%0.8f\n", strip.points[i], strip.values[i]);
    }
}

void print_params(char *type, PARAMS params)
{
    printf("=== %s ===\n", type);
    printf("=== PARAMS ===\n");
    printf("npoints: %d\n", params.npoints);
    printf("startval: %f\n", params.startval);
    printf("endval: %f\n", params.endval);
    printf("duration: %f\n", params.duration);
    printf("==============\n");
}

void print_strip(STRIP strip)
{
    printf("=== BREAKPOINT_STRIP ===\n");
    printf("npoints: %d\n", strip.npoints);
    for (int i = 0; i < strip.npoints; i++)
    {
        printf("Point %d: time=%.4f, value=%.8f\n", i, strip.points[i], strip.values[i]);
    }
    printf("========================\n");
}

STRIP natural_exponential_decay(PARAMS params)
{
    // x = ae^(kT)
    print_params("natural_exp", params);
    double n = params.npoints;
    double T = params.duration;
    double step = T / n;
    double epsilon = 1.0e-4;
    double k = -log(epsilon) / T;
    double ratio = exp(-k * step);

    double range = params.startval - params.endval;

    double *points = (double *)malloc((params.npoints + 1) * sizeof(double));
    double *values = (double *)malloc((params.npoints + 1) * sizeof(double));
    double t = 0.0;
    double decay = 1.0;
    for (int i = 0; i <= params.npoints; i++)
    {
        points[i] = t;
        // scaled values
        values[i] = params.endval + decay * range;
        t += step;
        decay *= ratio;
    }

    STRIP result = {n + 1, points, values};
    return result;
}

STRIP linear(PARAMS params)
{
    print_params("linear", params);
    double step = params.duration / params.npoints;
    double start = params.startval;
    double diff = (params.endval - params.startval) / params.npoints;
    double *points = (double *)malloc((params.npoints + 1) * sizeof(double));
    double *values = (double *)malloc((params.npoints + 1) * sizeof(double));
    double thisstep = 0.0;

    for (int i = 0; i <= params.npoints; i++)
    {
        points[i] = thisstep;
        values[i] = start;
        thisstep += step;
        start += diff;
    }

    STRIP result = {params.npoints + 1, points, values};
    return result;
}

STRIP step(PARAMS params, int step_count)
{
    print_params("STEP", params);
    double T = params.duration;
    double n = params.npoints;

    double step_size = (params.endval - params.startval) / step_count;
    double step_length = n / step_count;

    double *points = (double *)malloc((params.npoints + 1) * sizeof(double));
    double *values = (double *)malloc((params.npoints + 1) * sizeof(double));

    double t = 0;

    for (int i = 0; i <= n; i++)
    {
        points[i] = t;
        values[i] = params.startval + ceil(i / step_length) * step_size;
        t += T / n;
    }
    STRIP result = {n + 1, points, values};
    return result;
}

STRIP add_strips(STRIP strip1, STRIP strip2)
{
    int totalpoints = strip1.npoints + strip2.npoints;

    double *new_points = (double *)malloc(totalpoints * sizeof(double));
    double *new_values = (double *)malloc(totalpoints * sizeof(double));

    for (int i = 0; i < strip1.npoints; i++)
    {
        new_points[i] = strip1.points[i];
        new_values[i] = strip1.values[i];
    }
    double point_offset = strip1.points[strip1.npoints - 1];
    for (int i = 0; i < strip2.npoints; i++)
    {
        new_points[i + strip1.npoints] = strip2.points[i] + point_offset;
        new_values[i + strip1.npoints] = strip2.values[i];
    }
    STRIP result = {totalpoints, new_points, new_values};
    return result;
}

int main(int arc, char **argv)
{
    PARAMS params1 = {
        5,
        0,
        1,
        10};
    STRIP linear1 = linear(params1);
    PARAMS params2 = {
        200,
        1,
        0,
        30};
    STRIP linear2 = linear(params2);
    PARAMS params3 = {200, 0, 0.5, 50};
    STRIP linear3 = linear(params3);

    PARAMS nd1 = {
        200,
        0.5,
        0,
        30};
    STRIP n_decay = natural_exponential_decay(nd1);
    PARAMS stp1 = {
        200,
        0,
        1,
        10};
    STRIP stp = step(stp1, 5);
    STRIP combined_strip;
    combined_strip = add_strips(linear1, linear2);
    combined_strip = add_strips(combined_strip, linear3);
    combined_strip = add_strips(combined_strip, n_decay);
    combined_strip = add_strips(combined_strip, stp);
    write_to_file(combined_strip, "strip.bpf");
}