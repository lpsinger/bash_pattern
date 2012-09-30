/*
 * Copyright (C) 2012  Leo Singer <leo.singer@ligo.org>
 *
 * This file is part of bash_pattern.
 *
 * bash_pattern is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * bash_pattern is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with bash_pattern.  If not, see <http://www.gnu.org/licenses/>.
 */


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
    size_t len;
    bash_pattern **nodes;
} bash_pattern_alternatives;


static void bash_pattern_compile1(bash_pattern_alternatives *container, bash_pattern *head, bash_pattern **next, const char **pattern, int escaped);


static void bash_pattern_compile_literal(bash_pattern_alternatives *container, bash_pattern *head, bash_pattern **next, const char **pattern)
{
    bash_pattern_string_literal *string_literal;

    if (head && head->kind == BASH_PATTERN_STRING_LITERAL)
    {
        string_literal = (bash_pattern_string_literal *) head;
        string_literal->text = realloc(string_literal->text, string_literal->len + 1);
        string_literal->text[string_literal->len] = **pattern;
        string_literal->len ++;
        ++*pattern;
        bash_pattern_compile1(container, head, next, pattern, 0);
    } else {
        string_literal = malloc(sizeof(bash_pattern_string_literal));
        ((bash_pattern *) string_literal)->kind = BASH_PATTERN_STRING_LITERAL;
        string_literal->len = 1;
        string_literal->text = malloc(1);
        string_literal->text[0] = **pattern;
        *next = (bash_pattern *) string_literal;
        ++*pattern;
        bash_pattern_compile1(container, (bash_pattern *) string_literal, &((bash_pattern *) string_literal)->next, pattern, 0);
    }
}


static bash_pattern *bash_pattern_new_empty_literal()
{
    bash_pattern_string_literal *string_literal = malloc(sizeof(bash_pattern_string_literal));
    string_literal->len = 0;
    string_literal->text = NULL;
    ((bash_pattern *) string_literal)->kind = BASH_PATTERN_STRING_LITERAL;
    ((bash_pattern *) string_literal)->next = NULL;
    return (bash_pattern *) string_literal;
}


static void bash_pattern_compile1(bash_pattern_alternatives *container, bash_pattern *head, bash_pattern **next, const char **pattern, int escaped)
{
    if (!**pattern)
    {
        *next = NULL;
    } else if (escaped) {
        bash_pattern_compile_literal(container, head, next, pattern);
    } else if (**pattern == '\\') {
        ++*pattern;
        bash_pattern_compile1(container, head, next, pattern, 1);
    } else if (**pattern == '*') {
        bash_pattern *node = malloc(sizeof(bash_pattern));
        node->kind = BASH_PATTERN_ANY_STRING;
        *next = node;
        ++*pattern;
        bash_pattern_compile1(container, node, &node->next, pattern, 0);
    } else if (**pattern == '?') {
        bash_pattern *node = malloc(sizeof(bash_pattern));
        node->kind = BASH_PATTERN_ANY_CHAR;
        *next = node;
        ++*pattern;
        bash_pattern_compile1(container, node, &node->next, pattern, 0);
    } else if (**pattern == '{') {
        bash_pattern_alternatives *alternatives = malloc(sizeof(bash_pattern_alternatives));
        ((bash_pattern *) alternatives)->kind = BASH_PATTERN_ALTERNATIVES;
        alternatives->nodes = malloc(sizeof(bash_pattern *));
        alternatives->len = 0;
        ++*pattern;
        *next = (bash_pattern *) alternatives;
        bash_pattern_compile1(alternatives, NULL, alternatives->nodes, pattern, 0);
        bash_pattern_compile1(container, (bash_pattern *) alternatives, &((bash_pattern *) alternatives)->next, pattern, 0);
    } else if (**pattern == '}') {
        if (container) {
            ++*pattern;
            if (head)
            {
                head->next = NULL;
            } else {
                *next = bash_pattern_new_empty_literal();
            }
            container->len ++;
        } else {
            bash_pattern_compile_literal(container, head, next, pattern);
        }
    } else if (**pattern == ',') {
        if (container)
        {
            if (head)
            {
                head->next = NULL;
            } else {
                *next = bash_pattern_new_empty_literal();
            }
            ++*pattern;
            container->len ++;
            container->nodes = realloc(container->nodes, sizeof(bash_pattern *) * (container->len + 1));
            bash_pattern_compile1(container, NULL, container->nodes + container->len, pattern, 0);
        } else {
            bash_pattern_compile_literal(container, head, next, pattern);
        }
    } else {
        bash_pattern_compile_literal(container, head, next, pattern);
    }
}


static void bash_pattern_fallthrough(bash_pattern *head, bash_pattern *togoto)
{
    if (head)
    {
        bash_pattern_fallthrough(head->next, togoto);
        if (!head->next)
            head->next = togoto;
        if (head->kind == BASH_PATTERN_ALTERNATIVES)
        {
            bash_pattern_alternatives *alternatives = (bash_pattern_alternatives *) head;
            size_t i;
            for (i = 0; i < alternatives->len; i ++)
                bash_pattern_fallthrough(alternatives->nodes[i], head->next);
        }
    }
}


/* FIXME: no error checking yet. */
bash_pattern *bash_pattern_compile(const char *pattern)
{
    bash_pattern *ret;
    bash_pattern_compile1(NULL, NULL, &ret, &pattern, 0);
    bash_pattern_fallthrough(ret, NULL);
    return ret;
}


static void bash_pattern_free1(bash_pattern *head, bash_pattern *sentinel)
{
    if (head != sentinel)
    {
        bash_pattern *next = head->next;

        switch (head->kind)
        {
            case BASH_PATTERN_STRING_LITERAL:
            {
                bash_pattern_string_literal *string_literal = (bash_pattern_string_literal *) head;
                free(string_literal->text);
                string_literal->text = NULL;
                string_literal->len = 0;
            }
                break;
            case BASH_PATTERN_ALTERNATIVES:
            {
                bash_pattern_alternatives *alternatives = (bash_pattern_alternatives *)head;
                size_t i;
                for (i = 0; i < alternatives->len; i ++)
                {
                    bash_pattern_free1(alternatives->nodes[i], next);
                    alternatives->nodes[i] = NULL;
                }
                free(alternatives->nodes);
                alternatives->nodes = NULL;
            }
                break;
            default:
                /* No action needed. */
                break;
        }

        head->next = NULL;
        free(head);
        bash_pattern_free1(next, sentinel);
    }
}


void bash_pattern_free(bash_pattern *head)
{
    return bash_pattern_free1(head, NULL);
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
                size_t i;
                for (i = 0; i < alternatives->len; i ++)
                    if (bash_pattern_matches(alternatives->nodes[i], text))
                        return 1;
                return 0;
            }
                break;
            default:
                /* Should not be reached. */
                abort();
        }
    } else {
        /* There are no tokens left. */
        return !(*text);
    }
}
