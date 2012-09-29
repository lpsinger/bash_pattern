/*
 * Copyright (C) 2012  Leo Singer <leo.singer@ligo.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */


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
