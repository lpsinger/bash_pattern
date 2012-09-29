#ifndef BASH_PATTERN_H
#define BASH_PATTERN_H


#ifdef __cplusplus
extern "C" {
#endif
#if 0
} /* so that editor will match brace */
#endif


struct bash_pattern_t;
typedef struct bash_pattern_t bash_pattern;


bash_pattern *bash_pattern_compile(const char *);
void bash_pattern_free(bash_pattern *);
int bash_pattern_matches(const bash_pattern *, const char *);


#if 0
{ /* so that editor will match brace */
#endif
#ifdef __cplusplus
} /* extern "C" */
#endif


#endif
