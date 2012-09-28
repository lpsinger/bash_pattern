#include "bash_pattern.h"

#include <stdlib.h>
#include <string.h>


struct bash_pattern_t {
    enum {
        BASH_PATTERN_STRING_LITERAL,
        BASH_PATTERN_ANY_STRING,
        BASH_PATTERN_ANY_CHAR,
        BASH_PATTERN_ALTERNATIVES
    } kind;
    bash_pattern *next;
};


typedef struct {
    bash_pattern node;
    size_t len;
    char *text;
} bash_pattern_string_literal;


typedef struct {
    bash_pattern node;
    bash_pattern **nodes;
} bash_pattern_alternatives;


void bash_pattern_free(bash_pattern *head)
{
    if (head)
    {
        bash_pattern *next = head->next;

        switch (head->kind)
        {
            case BASH_PATTERN_STRING_LITERAL:
            {
                bash_pattern_string_literal *string_literal = (bash_pattern_string_literal *) head;
                free(string_literal->text);
            }
                break;
            case BASH_PATTERN_ALTERNATIVES:
            {
                bash_pattern_alternatives *alternatives = (bash_pattern_alternatives *)head;
                bash_pattern **node;
                for (node = alternatives->nodes; *node; node++)
                    free(node);
                free(alternatives->nodes);
            }
                break;
            default:
                /* No action needed */
                break;
        }

        free(head);
        head->next = NULL;
        bash_pattern_free(next);
    }
}


int bash_pattern_matches(const bash_pattern *head, const char *text)
{
    if (head)
    {
        switch (head->kind)
        {
            case BASH_PATTERN_STRING_LITERAL:
            {
                const bash_pattern_string_literal *string_literal = (const bash_pattern_string_literal *) head;
                return !strncmp(string_literal->text, text, string_literal->len) && bash_pattern_matches(head->next, text + string_literal->len);
            }
            case BASH_PATTERN_ANY_STRING:
            {
                const char *remaining_text;
                for (remaining_text = text; *remaining_text; remaining_text ++)
                    if (bash_pattern_matches(head->next, remaining_text))
                        return 1;
                return bash_pattern_matches(head->next, remaining_text);
            }
            case BASH_PATTERN_ANY_CHAR:
                return (*text) && bash_pattern_matches(head->next, text + 1);
            case BASH_PATTERN_ALTERNATIVES:
            {
                const bash_pattern_alternatives *alternatives = (const bash_pattern_alternatives *) head;
                bash_pattern **node;
                for (node = alternatives->nodes; *node; node++)
                    if (bash_pattern_matches(*node, text))
                        return 1;
                return 0;
            }
                break;
            default:
                /* Should not be reached */
                abort();
        }
    } else {
        /* There are no tokens left. */
        return !(*text);
    }
}
