#include <unistd.h>
#include <sys/stat.h>

int main(int argc, char* argv[]) {
       if (argc<2) return 1;

       mkdir(argv[1],0755);
       return 0;
}
