#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    FILE *fp = fopen("amplitude.bpf", "w");
    if (!fp) {
        perror("Error opening file");
        return 1;
    }
    // Simple ADSR-like envelope
    for (double i = 0; i < 30; i+=0.5)
    {
       fprintf(fp, "%.1f %d\n", i, rand() % 5); 
    }

    fclose(fp);
    return 0;
}
