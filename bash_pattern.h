#ifndef BASH_PATTERN_H
#define BASH_PATTERN_H


#ifdef __cplusplus
extern "C" {
#else
} /* so that editor will match brace */
#endif


struct bash_pattern_t;
typedef struct bash_pattern_t bash_pattern;


bash_pattern *bash_pattern_compile(const char *pattern);
void bash_pattern_free(bash_pattern *pattern);
int bash_pattern_matches(bash_pattern *pattern, char *text);


#ifndef __cplusplus
{ /* so that editor will match brace */
#else
} /* extern "C" */
#endif


#endif
