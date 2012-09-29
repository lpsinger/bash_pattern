#include <bash_pattern.h>
#include <stdio.h>
#include <sysexits.h>

int main(int argc, char **argv)
{
    int ret;
    bash_pattern *pattern;

    if (argc < 3)
    {
        puts("Usage: bash_pattern_test 'pattern' 'text'");
        return EX_USAGE;
    }

    fprintf(stderr, "Compling pattern: %s\n", argv[1]);
    pattern = bash_pattern_compile(argv[1]);

    fprintf(stderr, "Matching against text: %s\n", argv[2]);
    ret = bash_pattern_matches(pattern, argv[2]);

    if (ret)
        fprintf(stderr, "Match: YES\n");
    else
        fprintf(stderr, "Match: NO\n");

    fprintf(stderr, "Freeing pattern\n");
    bash_pattern_free(pattern);

    return !ret;
}
