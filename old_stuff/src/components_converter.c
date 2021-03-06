// Author : E. Orliac
//          DCSR, UNIL
// Date   : 2019/06/04
// Purpose: Converts a binary components file to txt.
//          Allows for sanity check on estimates retrieval
//          during development phase.
// Compil.: icc code.c -o exe
// Run    : ./exe path_to_cpn_file [> out.cpn.txt]
// Note   : To be check against first value of each block as
//          found in the run log file (e.g. slurm output file)
// ----------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

int main(int argc, const char * argv[]) {

    printf("__CONVERT COMPONENTS FILES TO TEXT__\n");

    if (argc != 3) {
        printf("Wrong number of arguments passed: %d; expected 2 (path to .bet file, and number of iterations to convert)!\n", argc - 1);
        exit(1);
    }

    FILE* fh = fopen(argv[1], "rb");
    if (fh == NULL) {
        printf("Error opening file: %s\n", argv[1]);
        exit(1);
    }

    uint niter   = atoi(argv[2]);

    uint M = 0;
    fread(&M, sizeof(uint), 1, fh);
    printf("%d markers were processed.\n", M);

    double cpn;
    uint   itthin;
    size_t offset;

    for (uint iter=0; iter<=niter; ++iter) {

        // Read iteration number (potentially decimated by --thin option)
        offset = sizeof(uint) + iter * (sizeof(uint) + M * sizeof(int));
        fseek(fh, offset, SEEK_SET);
        fread(&itthin, sizeof(uint), 1, fh);
        printf("read iteration number %lu (iter=%lu) at %lu\n", itthin, iter, offset);

        for (uint marker=0; marker<M; ++marker) {
            fseek(fh, sizeof(uint) + sizeof(uint) * (iter+1) + sizeof(int) * (M * iter + marker), SEEK_SET);
            fread(&cpn, sizeof(int), 1, fh);
            printf("%5d/%7d = %2d\n", itthin, marker, cpn);
        }
    }

    fclose(fh);

    return 0;
}
