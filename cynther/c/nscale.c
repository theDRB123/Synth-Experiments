/* nscale.c : Display E.T Frequencies for an N-note octave*/

#include <stdio.h>
#include <math.h>

int main(int argc, char *argv[])
{
    int notes, midinote;
    double frequency, ratio;
    double c0, c5;
    double frequencies[24];

    notes = atof(argv[1]);
    if (notes < 1)
    {
        printf("Error: notes must be positive\n");
        return 1;
    }
    if (notes > 24)
    {
        printf("Error: maximum value for notes is 24\n");
        return 1;
    }
    midinote = atof(argv[2]);
    if (midinote < 0)
    {
        printf("Error: cannot have negative MIDI notes!\n");
        return 1;
    }
    if (midinote > 127)
    {
        printf("Error: maximum MIDInote is 127\n");
        return 1;
    }
    if (argc != 3)
    {
        printf("Usage: nscale notes midinote\n");
        return 1;
    }
    // find the middle c
    ratio = pow(2.0, 1.0 / 12);
    c5 = 220.0 * pow(ratio, 3);
    c0 = c5 * pow(0.5, 5);
    frequency = c0 * pow(ratio, midinote);
    ratio = pow(2.0, 1.0 / notes);
    for (int i = 0; i < notes; i++)
    {
        frequencies[i] = frequency;
        frequency *= ratio;
    }
    for (int i = 0; i < notes; i++)
    {
        printf("%d : %f\n",i, frequencies[i]);
    }
    return 0;
}